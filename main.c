#include <8051.h>

#include "lib/7segx4/7segx4.h"
#include "lib/keypad4x4/keypad4x4.h"
#include "tone.h"

// 10ms
#define TR1_H ((65536 - 10000) >> 8);
#define TR1_L ((65536 - 10000) % 256);

// expended memory data
__xdata unsigned char toneRecord[500];
__xdata unsigned char toneRange[500];
__xdata unsigned char seg[4] = {0};

signed char key = -1;

/*
  0: normal mode
  1: record mode
  2: play mode
*/
unsigned char recordFlag = 0;
/*
  八度音域 3 - 5
  index range: 0 - 2
*/
unsigned char range = 1;
unsigned int recordLength = 0;
// counter
unsigned int accumulator = 0;
unsigned int count = 0;
// indexer
unsigned char toneIndex = 0, digit = 0;

int main() {
  /*
    P0: keypad
    P1: led
    P2: speaker
    P3: led digit

    keypad button:
    0: toggle mode
    1 - 8: tones
    8: 八度音 3
    9: 八度音 4
    A: 八度音 5
  */
  TMOD = 0x11;  // 16-bit timer/counter
  TR0 = 1;      // timer 0 run control bit
  TR1 = 1;      // timer 1 run control bit
  IE = 0x8A;    // Timer 0, 1 overflow interrupt enable
  seg[0] = 0;
  seg[1] = 0;
  seg[2] = 4;
  seg[3] = 0;

  while (1) {
  }
}

void timer0_isr(void) __interrupt(1) __using(1) {
  TH0 = tones[range][toneIndex] >> 8;
  TL0 = tones[range][toneIndex] % 256;

  if ((key > 0 && key <= 12) || recordFlag == 2) {
    P2 = ~P2;
    seg[1] = toneIndex;
  }
}
void timer1_isr(void) __interrupt(3) __using(3) {
  // 10ms
  TH1 = TR1_H;
  TL1 = TR1_L;

  if ((count % 5) == 0) {  // 50ms for detect key press
    key = useKeypad();

    // operation mode (0) or record mode (1) play tone that key was pressed
    if (key > 0 && key <= 12 && recordFlag != 2) {
      toneIndex = key;
    } else if (key == 13) {
      range = 0;
      seg[2] = 3;
    } else if (key == 14) {
      range = 1;
      seg[2] = 4;
    } else if (key == 15) {
      range = 2;
      seg[2] = 5;
    }

    /* start handle record and play tone */
    if (recordFlag == 1) {  // recording
      toneRecord[recordLength] = key > 0 && key <= 7 ? key : 0;
      toneRange[recordLength] = range;
      recordLength++;
    } else if (recordFlag == 2) {  // play record tone
      toneIndex = toneRecord[accumulator];
      range = toneRange[accumulator];
      seg[0] = range + 3;
      accumulator++;
      if (accumulator == recordLength - 1) accumulator = 0;
    }
    /* end handle record and play tone */
  }

  if (count > 30) {  // 300ms for debounce
    /* start key 0 pressed handler */
    if (key == 0) {
      if (recordFlag == 0) {  // next state is 1
        toneIndex = 0;
        recordLength = 0;
        recordFlag = 1;
        seg[3] = 1;
        seg[1] = 0;
        seg[0] = 0;
      } else if (recordFlag == 1) {  // next state is 2
        accumulator = 0;
        recordFlag = 2;
        seg[3] = 2;
        seg[1] = 0;
        seg[0] = 0;
      } else if (recordFlag == 2) {  // next state is 0
        recordFlag = 0;
        seg[3] = 0;
        seg[1] = 0;
        seg[0] = 0;
      }
    }
    /* end key 0 pressed handler */
    count = 0;
  } else
    count++;

  /* start control led section */
  use7Seg(digit, seg[digit]);

  if (digit > 3)
    digit = 0;
  else
    digit++;
  /* end control led section */
}
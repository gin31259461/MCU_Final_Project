#include "7segx4.h"

/*
 7 6 5 4 3 2 1 0
 p g f e d c b a
*/
const unsigned char segTable[] = {
    0b1000000,  // 0
    0b1111001,  // 1
    0b0100100,  // 2
    0b0110000,  // 3
    0b0011001,  // 4
    0b0010010,  // 5
    0b0000010,  // 6
    0b1011000,  // 7
    0b0000000,  // 8
    0b0010000,  // 9
    0b0001000,  // A
    0b0000011,  // b
    0b1000110,  // C
    0b0100001,  // d
    0b0000110,  // E
    0b0001110,  // F
};

void use7Seg(int digit, int count) {
  P3 = 0x00 | (1 << digit);
  // led a - f and p
  P1 = segTable[count];
  return;
}

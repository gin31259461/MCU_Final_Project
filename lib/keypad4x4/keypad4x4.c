#include "keypad4x4.h"

const unsigned keys[4][4] = {
    {0, 4, 8, 12}, {1, 5, 9, 13}, {2, 6, 10, 14}, {3, 7, 11, 15}};

signed char useKeypad() {
  for (char c = 0; c < 4; c++) {
    /*
      higher 4 bits scan each column, v in low
      1110 1101 1011 0111
    */
    P0 = ~(0x10 << c);

    for (char r = 0; r < 4; r++) {
      /*
        lower 4 bits set 111
        if one bit is transferred to 0, button was pushed
      */
      if (!(P0 & (1 << r) /* compare each row if has 0 bit */)) {
        return keys[r][c];
      }
    }
  }
  return -1;
}

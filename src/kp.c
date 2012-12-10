#include <kp.h>

static const char keys[][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

void kp_init() {
    COLDIR |= COLMASK;  // Columns are outputs
    COLOUT &= ~COLMASK; // Set them all to 0
    ROWDIR &= ~ROWMASK; // Rows are inputs
    ROWREN |= ROWMASK;  // Resistors enabled
    ROWOUT &= ~ROWMASK; // Resistors pull down
}

#ifndef uint
#define uint unsigned int
#endif

char kp_getc() {
  uint a;
  uint read = 0;
  char ret = 0;
  for(a = 0; a < 4; a++) {
    COLOUT = ((1 << (a+COLBASE))) | (COLOUT & (~COLMASK));
    __delay_cycles(20); // Keypad and 12 MHz signals are not friends.
    read = (ROWIN & ROWMASK);
    read >>= ROWBASE;
    if(read) {
      int nread = 0;
      while(nread < 4 && !((1<<nread) & read)) nread++;
      ret = keys[nread][a];
      break;
    }
  }
  COLOUT &= (~COLMASK);
  return ret;
}

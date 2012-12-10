#include <msp430.h>

#define ROWIN   (P1IN)
#define ROWOUT  (P1OUT)
#define ROWDIR  (P1DIR)
#define ROWREN  (P1REN)
#define ROWIE   (P1IE)
#define ROWIES  (P1IES)
#define ROWIFG  (P1IFG)
#define ROWBASE (4)         // P1.? is the first row
#define ROWMASK (0xF0)      // Bit mask for rows
#define COLOUT  (P1OUT)
#define COLDIR  (P1DIR)
#define COLBASE (0)         // P1.? is the fist column
#define COLMASK (0x0F)      // Bit mask for columns

void kp_init();

char kp_getc();
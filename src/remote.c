#include <msp430.h>
#include "kp.h"

#define uint unsigned int
#define IRLED (BIT6|BIT7)

volatile unsigned int ovfcount;
volatile unsigned int ir_enable;

// Wait 600us. We count 48 TimerA overflows (which occur at ~40*2 KHz).
void
wait_600(uint n) {
    ovfcount = 0;
    while(ovfcount < ((n<<4) + (n<<5)));
}

void
tx_code(uint addr, uint cmd) {
    int n = 1;
    ir_enable = 1;
    wait_600(4); // 2400us header
    ir_enable = 0;
    wait_600(1); // 600us down    
    
    //tx cmd, lsb first
    while(n < (1<<7)) {
        ir_enable = 1;
        if(cmd & n) {
            wait_600(2);
        } else {
            wait_600(1);
        }
        ir_enable = 0;
        wait_600(1);
        n <<= 1;
    }
    
    n = 1;
    //tx addr, lsb first
    while(n < (1<<5)) {
        ir_enable = 1;
        if(addr & n) {
            wait_600(2);
        } else {
            wait_600(1);
        }
        ir_enable = 0;
        wait_600(1);
        n <<= 1;
    }
    
    wait_600(23);
}


int
main() {
    WDTCTL = WDTPW | WDTHOLD;

    // You'll have to tune your DCO values to get around 12 MHz
    DCOCTL = 0x80;
    BCSCTL1 = 0x0E;
    
    // IR LED is output. Using LF crystal pins for I/O.
    P2SEL &= ~IRLED;
    P2DIR |= IRLED;

    // Init keypad: set direction, etc.
    kp_init();
    
    // Preparing interrupts for waking up from LPM4
    ROWIE &= ~ROWMASK;  // Disable keypad interrupts
    ROWIES &= ~ROWMASK; // Rising edge interrupt
    ROWIFG &= ~ROWMASK; // Clear int flags just in case
        
    // TimerA from SMCLK, Count up to CCR0, interrupt enabled
    TACCR0 = 150;       // At 12 MHz clock, this gets us 80 KHz.
    TACTL = TASSEL_2 | ID_0 | MC_1 | TAIE; 
    
    _BIS_SR(GIE);       // Enable interrupts
    
    char code, key;
    while(1) {
        // Read
        key = kp_getc();
        if(key) {
            // Select code, then transmit it.
            if(key >= '1' && key <= '9') {
                code = key-'1';
            } else {
                switch(key) {
                case '0':
                    code = 9; break; // 0
                case '#':
                    code = 37; break; // TV / Video
                case '*':
                    code = 21; break; // POWER
                case 'A':
                    code = 18; break; // VOL+
                case 'B':
                    code = 19; break; // VOL-
                case 'C':
                    code = 16; break; // CHAN+
                case 'D':
                    code = 17; break; // CHAN-
                default:
                    code = 20; // Mute. Never reached.
                    break;
                }
            }
            // Addr 1: Sony TV
            tx_code(1,code);
        } else {
            // Turn off the IR LED
            P2OUT &= ~IRLED;
            // Set interrupt on any button, then LPM4
            COLOUT |= COLMASK;  // All outputs high
            ROWIFG &= ~ROWMASK; // Clear int flags just in case
            ROWIE |= ROWMASK;   // Enable interrupts on all rows
            LPM4; // zzz
            // When reaching this line, I've been awoken by a keypress
            ROWIE &= ~ROWMASK; // Disable interrupts
            ROWIFG &= ~ROWMASK; // Clear int flags if any pending
            COLOUT &= ~COLMASK; // Outputs low
            // During next cycle, we'll read which button is pressed, until
            // it's released, then we'll go to sleep again.
        }
    }
    return 23;
}

// Port1 interrupt: wake up! somebody pressed something!
void
__attribute__((interrupt (PORT1_VECTOR)))
port1_handler() {
    ROWIFG &= ~ROWMASK; // Clear int flags if any
    LPM4_EXIT;
}

// Oscilating for the 40 KHz modulated signal.
void
__attribute__((interrupt (TIMERA1_VECTOR)))
ta_handler() {
    TACTL &= ~TAIFG;
    if (ir_enable) {
        // If I'm transmitting, toggle LED.
        P2OUT ^= IRLED;
    } else {
        // Else, LED is off.
        P2OUT &= ~IRLED;
    }
    ovfcount++; // One more "overflow" occurred (not really).
}
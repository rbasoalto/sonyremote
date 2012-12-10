# Sony IR Remote for MSP430G* #

An experimental remote control based on MSP430G2211, an IR LED, and a 4x4 matrix keypad.

The keypad code is very suboptimal, I reused to some extent [my previous keypad project](https://github.com/rbasoalto/kptest).

Keypad should be connected to Port 1 (4 columns first, then 4 rows). IR LED to P2.6 or P2.7 (no crystal is needed, we can use those pins).

This remote, with light use, should get you many years of battery life on a CR2032 or similar coin cell.

## Schematic ##


       ^
      / \
     /3v3\
     -----
       |
       |             +----------------------------+
       |             |                            |
       +-------------+VDD (1)            (2)  P1.0+------------------+
       |             |                            |                  |
       |             |                   (3)  P1.1+---------------+  |
       |             |                            |               |  |
       |             |                   (4)  P1.2+------------+  |  |
       |             |                            |            |  |  |
       |             |                   (5)  P1.3+---------+  |  |  |
       |             |                            |         |  |  |  |
       |             |                            |      +--|--|--|--|--+
       |             |                            |      |  |  |  |  |  |
       |             |                   (6)  P1.4+--------[1][2][3][A] |
       |  +-------+  |                            |      |  |  |  |  |  |
       +--+ 10K   +--+RST (10)           (7)  P1.5+--------[4][5][6][B] |
       |  +-------+  |                            |      |  |  |  |  |  |
       |             |                   (8)  P1.6+--------[7][8][9][C] |
       |             |                            |      |  |  |  |  |  |
     -----           |                   (9)  P1.7+--------[*][0][#][D] |
     ----- 10nF      |                            |      |              |
       |             |                            |      +--------------+
       |             |         MSP430G2211        |     4x4 Matrix Keypad
       |             |                            |
       |             |                            |
       +-------------+VSS (14)           (13) P2.7+---------------+
       |             |                            |               |
       |             |                            |             -----
       |             +----------------------------+      IR LED \   /
       |                                                         \ /
       |                                                        -----
       |                                                          |
       |                                                          |
     -----                                                      -----
     \GND/                                                      \GND/
      \ /                                                        \ /
       V                                                          V


# Acknowledgements #

Thanks to [Sergio Campamá](https://github.com/sergiocampama) for providing (unknowingly) the Rakefile ;)

Very good IR protocol explanation from [Ken Shirriff's Blog](http://www.arcfn.com/2010/03/understanding-sony-ir-remote-codes-lirc.html).

Sony remote codes from [HiFi-Remote.com](http://www.hifi-remote.com/sony/Sony_tv.htm).

# License #

This project is released under the MIT License:

Copyright (C) 2012 Rodrigo Basoalto

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

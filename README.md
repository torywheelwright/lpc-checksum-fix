# lpc-checksum-fix
NXP's ARM Cortex based LPC microcontrollers determine code validity base on a checksum at 0x1C. When building programs with the GNU toolchain, this checksum is not generated (the location is zeroed). This command line tool generates a new binary image with the correct checksum.

Usage
=====

1. `make`
2. `./lpc-checksum-fix inputimage.bin outputimage.bin`

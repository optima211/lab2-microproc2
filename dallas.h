#ifndef __DALLAS_H
#define __DALLAS_H

sbit DQ  = 0xB3;		// P3.3

static void delay (int useconds);
static unsigned char ow_reset (void);
static unsigned char read_bit (void);
static void write_bit (char bitval);
static unsigned char read_byte (void);
static void write_byte (unsigned char val);

extern char GetTemp(void);

#endif

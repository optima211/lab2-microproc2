#include "dallas.h"

// Calling the routine takes about 24us, and then
// each count takes another 16us.
void delay(int useconds) {
  int s;
  for (s=0; s<useconds; s++);
}

// OW_RESET - performs a reset on the one-wire bus and
// returns the presence detect. Reset is 480us, so delay
// value is (480-24)/16 = 28.5 - we use 29. Presence checked
// another 70us later, so delay is (70-24)/16 = 2.875 - we use 3.
unsigned char ow_reset (void) {
  unsigned char presence;

  DQ = 0; 			// pull DQ line low
  delay(29);		// leave it low for 480us
  DQ = 1; 			// allow line to return high
  delay(3); 		// wait for presence
  presence = DQ; 	// get presence signal
  delay(25); 		// wait for end of timeslot
  return(presence); // presence signal returned
} 					// 0=presence, 1 = no part

// READ_BIT - reads a bit from the one-wire bus. The delay
// required for a read is 15us, so the DELAY routine won't work.
// We put our own delay function in this routine in the form of a
// for() loop.
unsigned char read_bit(void) {
  unsigned char i;

  DQ = 0; 				// pull DQ low to start timeslot
  DQ = 1; 				// then return high
  for (i=0; i<3; i++); 	// delay 15us from start of timeslot
  return(DQ); 			// return value of DQ line
}


// WRITE_BIT - writes a bit to the one-wire bus, passed in bitval.
void write_bit(char bitval) {
  DQ = 0; 				// pull DQ low to start timeslot
  if(bitval==1) DQ =1; 	// return DQ high if write 1
  delay(5); 			// hold value for remainder of timeslot
  DQ = 1;				// Delay provides 16us per loop, plus 24us. Therefore delay(5) = 104us
}						

// READ_BYTE - reads a byte from the one-wire bus.
unsigned char read_byte(void) {
  unsigned char i;
  unsigned char value = 0;

  for (i=0;i<8;i++) {
    if(read_bit()) value|=0x01<<i; 	// reads byte in, one byte at a time and then
									// shifts it left
	delay(6); 						// wait for rest of timeslot
  }
  return(value);
}

// WRITE_BYTE - writes a byte to the one-wire bus.
void write_byte(char val) {
  unsigned char i;
  unsigned char temp;

  for (i=0; i<8; i++) {		// writes byte, one bit at a time
    temp = val>>i; 			// shifts val right 'i' spaces
    temp &= 0x01; 			// copy that bit to temp
    write_bit(temp); 		// write bit in temp into
  }
  delay(5);
}

char GetTemp(void) {
  unsigned char get[10];
  int k;

  ow_reset();
  write_byte(0xCC); 									// Skip ROM
  write_byte(0x44); 									// Start Conversion
  delay(5);
  while (!DQ);

  ow_reset();
  write_byte(0xCC); 									// Skip ROM
  write_byte(0xBE); 									// Read Scratch Pad
  for (k=0;k<9;k++) {get[k]=read_byte();}
  return (get[1] << 4) | (get[0] >> 4);					// Integer part of Temperature
}

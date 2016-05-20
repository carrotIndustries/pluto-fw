#pragma once

/* defines *******************************************************************/
#define IN 0b000
#define INPD 0b010
#define INPU 0b011
#define OUL 0b100
#define OUH 0b101
#define FUNC0 0b00000
#define FUNC1 0b01000
#define FUNC2 0b10000
#define FUNC3 0b11000

#define PCONF(PORT, PIN, MODE) \
	P##PORT##SEL1 = (P##PORT##SEL1 & ~(1 << PIN)) | (((MODE >> 4) & 1) << PIN); \
	P##PORT##SEL0 = (P##PORT##SEL0 & ~(1 << PIN)) | (((MODE >> 3) & 1) << PIN); \
	P##PORT##DIR = (P##PORT##DIR & ~(1 << PIN)) | (((MODE >> 2) & 1) << PIN); \
	P##PORT##REN = (P##PORT##REN & ~(1 << PIN)) | (((MODE >> 1) & 1) << PIN); \
	P##PORT##OUT = (P##PORT##OUT & ~(1 << PIN)) | ((MODE & 1) << PIN)

#define PLOW(PORT, PIN) (P##PORT##OUT &= ~(1 << PIN))
#define PHIGH(PORT, PIN) (P##PORT##OUT |= (1 << PIN))
#define PGET(PORT, PIN) (!!(P##PORT##IN & (1 << PIN)))

/* prototypes ****************************************************************/
void io_init(void);

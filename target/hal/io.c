#include <msp430.h>
#include "io.h"

void io_init(void)
{
	/*	initially set everything to output and low for lowest power consumption */
	P1DIR = 0xff;
	P1OUT = 0;
	P2DIR = 0xff;
	P2OUT = 0;
	P3DIR = 0xff;
	P3OUT = 0;
	P4DIR = 0xff;
	P4OUT = 0;
	P6DIR = 0xff;
	P6OUT = 0;
	P7DIR = 0xff;
	P7OUT = 0;
	P9DIR = 0xff;
	P9OUT = 0;
	PJDIR = 0xff;
	PJOUT = 0;

	PCONF(7, 0, OUL); /* IR PWR */
	PCONF(J, 3, IN); /* IR in */


	

	PCONF(9, 5, OUL); /* dbg */

	/* unused pin as output driving low */
	PCONF(1, 1, OUL);
	PCONF(1, 2, OUL);
	PCONF(1, 3, OUL);
	PCONF(6, 0, OUL);
	PCONF(6, 1, OUL);
	PCONF(6, 2, OUL);
	PCONF(7, 1, OUL);
	PCONF(7, 2, OUL);
	PCONF(7, 4, OUL);
}

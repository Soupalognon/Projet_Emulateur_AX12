#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>
#include <p30f2010.h>
#include <xc.h>
#include <math.h>

#define FOSC            64000000LL
//#define FOSC            120000000LL
#define FCY             (FOSC/4)
#define delay_us(x) __delay32(((x*FCY)/1000000L)) // delays x us
#define delay_ms(x) __delay32(((x*FCY)/1000L))      // delays x ms
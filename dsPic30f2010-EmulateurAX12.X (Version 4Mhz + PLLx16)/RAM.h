/* 
 * File:   RAM.h
 * Author: user
 *
 * Created on 18 octobre 2015, 18:06
 */
#include "Include_Library.h"

void RAM_init();
void ecritureRAM(unsigned short adresse, unsigned short value);
unsigned short lectureRAM(unsigned short adresse);



/*

 Attribution des cases mémoires RAM:
 * 24 --> Torque Enable
 * 25 --> LED
 * 30 --> Goal Position (L)  
 * 31 --> Goal Position (H)
 * 32 --> Moving Speed (L)
 * 33 --> Moving Speed (H)
 * 36 --> Present Position (L)
 * 37 --> Present Position (H)
 * 42 --> Present Voltage
 * 43 --> Present Temperature
 * 44 --> Registered Instruction
 * 47 --> Lock
 * 48 --> Punch (L)
 * 49 --> Punch (H)
 * 50 --> Prescaler PWM
 * 51 --> Periode PWM
 * 52 --> Rapport Cyclique PWM
 */
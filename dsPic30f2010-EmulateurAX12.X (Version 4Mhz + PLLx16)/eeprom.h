/* 
 * File:   eeprom.h
 * Author: azerty
 *
 * Created on 16 octobre 2015, 17:41
 */
#include "Include_Library.h"



#define ERASE_WORD 0x4044
#define WRITE_WORD 0x4004
#define ADDRESS_HI 0x7F
//#define AdresseEE0 0x7FFC00

unsigned short Eeprom_ReadWord(_prog_addressT pushAddress);
void Eeprom_WriteWord(_prog_addressT Address, unsigned short Value);
void ResetEpprom(void);


/*

 Attributions des cases m�moires EEPROM:
 * 3 --> ID                     D�fault : 12
 * 4 --> Baud Rate              D�fault : 207
 * 5 --> Return Delay           D�fault : 250
 * 14 --> Max Torque (L)        D�fault : 255
 * 15 --> Max Torque (H)        D�fault : 3
 * 16 --> Status return level   D�fault : 2
 * 17 --> Alarm Led             D�fault : 4
 * 18 --> Alarme Shutdown       D�fault : 4

 */
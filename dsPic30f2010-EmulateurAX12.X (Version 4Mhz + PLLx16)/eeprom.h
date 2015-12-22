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

 Attributions des cases mémoires EEPROM:
 * 3 --> ID                     Défault : 12
 * 4 --> Baud Rate              Défault : 207
 * 5 --> Return Delay           Défault : 250
 * 14 --> Max Torque (L)        Défault : 255
 * 15 --> Max Torque (H)        Défault : 3
 * 16 --> Status return level   Défault : 2
 * 17 --> Alarm Led             Défault : 4
 * 18 --> Alarme Shutdown       Défault : 4

 */
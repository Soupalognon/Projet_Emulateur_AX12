#include "eeprom.h"
#include "Gestion_UART.h"
 
extern unsigned short _EEDATA(2) ushEepromData[]  
   #ifdef extern 
      = {0xAA55, 0x55AA} // Initialize the eeprom values (programming time only)
   #endif
   ;

char __attribute__((space(eedata), aligned(_EE_WORD))) dat[_EE_WORD];
/*
 * Le mot clé __attribute__ permet de déclarer un attribue spécial.
 * il doit être suivit par une spécificité (comme space ou aligned) et être en entre parentheses.
 * voir les spécificités dans XC16 c compiler User's Guide  -->  13.2.2 Function Attributes.
 * 
 * 
 * 
 * 
 * 
 * 
 * Le double underscore __ permet d'appler la variable dans le fichier header et de ne
 * pas être embêter d'un possible macro nommé pareil.
 */

 
 unsigned short Eeprom_ReadWord(_prog_addressT pushAddress) 
 { 
   _prog_addressT AdresseEE0;
   _init_prog_address(AdresseEE0, dat); /* get address in program space */
   unsigned short ushResult;
   _prog_addressT eedata_addr = AdresseEE0 + 2*pushAddress; // __builtin_tbloffset(); 
   register int eedata_val; 
   
   TBLPAG = ADDRESS_HI; // __builtin_tblpage()
   __asm__("TBLRDL [%[addr]], %[val]" : [val]"=r"(eedata_val) : [addr]"r"(eedata_addr)); 
  //_wait_eedata();
   ushResult = eedata_val; 
   return ushResult;  
 }
 
 void Eeprom_WriteWord(_prog_addressT Adresse, unsigned short Value) 
 {
     
    _prog_addressT AdresseEE0;
    _init_prog_address(AdresseEE0, dat); /* get address in program space */
    _erase_eedata(AdresseEE0 + 2*Adresse, _EE_WORD);
    _wait_eedata(); /* wait for operation to complete */
    _write_eedata_word(AdresseEE0 + 2*Adresse, Value); /* write a word */
    _wait_eedata(); /* wait for operation to complete */
        
     //On multiplie par 2 pour empecher les effacement des adresses paires
    //Car adresse codée sur 8bit --> eeprom codée sur 16 bits
   
    
    
    /*
   _prog_addressT AdresseEE0;
   _init_prog_address(AdresseEE0, dat);
   
   TBLPAG = ADDRESS_HI;
   NVMADRU = ADDRESS_HI; // Write address of word to be erased into NVMADRU, NVMADR registers.
   NVMADR = Adresse;
   NVMCON = ERASE_WORD; // Setup NVMCON register to erase one EEPROM word.
   
   //asm volatile ("disi #0x3FFF");
   //PROTECT_CODE_FROM_INTERRUPTS_START // Disable interrupts while the KEY sequence is written
      NVMKEY = 0x55; // Write the KEY sequence step1
      NVMKEY = 0xAA; // step2
      NVMCONbits.WR = 1; // Start the erase cycle
   //PROTECT_CODE_FROM_INTERRUPTS_STOP // Enable interrupts
   //asm volatile ("disi #0");
   
   while(NVMCONbits.WR == 1); // wait for the EEPROM
   
   NVMCON = WRITE_WORD; // Setup NVMCON register to write one EEPROM word. 
   {
      register int eedata_addr; 
      register int eedata_val; 
      
      eedata_addr = AdresseEE0 + 2*Adresse; // write low word of address
      eedata_val = Value; // write data
      
      __asm__ volatile ("TBLWTL %[val], [%[addr]]" : [val]"+r"(eedata_val) : [addr]"r"(eedata_addr));
   }
   */
 }
 
 
void ResetEpprom(void)
{
    //_prog_addressT temp = AdresseEE0;
    
    Eeprom_WriteWord(3, 12);  //ID
    Eeprom_WriteWord(4, Baud_9600);  //Baud Rate
    Eeprom_WriteWord(5, 250);  //Return Delay
    Eeprom_WriteWord(14, 255); //Max Torque (L)
    Eeprom_WriteWord(15, 3);   //Max Torque (H))
    Eeprom_WriteWord(16, 2);   //Status return level
    Eeprom_WriteWord(17, 4);   //Alarm Led
    Eeprom_WriteWord(18, 4);   //Alarme Shutdown
}
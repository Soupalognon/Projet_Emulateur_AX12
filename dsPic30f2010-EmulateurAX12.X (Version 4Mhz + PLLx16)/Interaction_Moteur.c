#include "Interaction_Moteur.h"
#include "RAM.h"
#include "eeprom.h"
#include "Gestion_UART.h"
#include "PWM.h"

void Verifications()
{
    calculErreur(RESET_ERR);    //Reset les erreurs
    
    //Mettre les vérifs de voltage/courant et autre...
}


void Interaction_LED()
{
    unsigned short tempAlarmLED = Eeprom_ReadWord(17);      //17--> Registre Alarme LED
    unsigned short erreur = get_Erreur();
    
    if(lectureRAM(25) == 1 && T2CONbits.TON != 1)   //T2CONbits.TON != 1 car peut etre que l'interupt est activé donc il y a un probleme
        PORTBbits.RB0 = 1;
    else if(lectureRAM(25) == 0 && T2CONbits.TON != 1)
        PORTBbits.RB0 = 0;
    
    
    if((tempAlarmLED & 1) == 1)      //Si on active Input Voltage error
    {
        if((erreur & 1) == 1)   //Alors lancer Timer pour faie clignoter la led
            T2CONbits.TON = 1;  //Active le bip de la LED
            
    }
    if((tempAlarmLED & 2) == 2)      //Si on active Angle limit error
    {
        
    }
    if((tempAlarmLED & 4) == 4)      //Si on active Overheating error
    {
        if((erreur & 2) == 2)
            T2CONbits.TON = 1;  //Active le bip de la LED
            //Alors lancer Timer pour faie clignoter la led
    }
    if((tempAlarmLED & 8) == 8)      //Si on active Range error
    {

    }
    if((tempAlarmLED & 16) == 16)      //Si on active CheckSum error
    {
        if((erreur & 16) == 16)
            T2CONbits.TON = 1;  //Active le bip de la LED
            //Alors lancer Timer pour faie clignoter la led
    }
    if((tempAlarmLED & 32) == 32)      //Si on active Overload error
    {

    }
    if((tempAlarmLED & 64) == 64)      //Si on active Instruction error
    {
        if((erreur & 64) == 64)
            T2CONbits.TON = 1;  //Active le bip de la LED
            //Alors lancer Timer pour faie clignoter la led
    }
}


void Interaction_Position()
{
    if(lectureRAM(24) == 1)   //Si le couple est activé
    {
        //Alors on controle ici le moteur
        PWM_rapportCyclique(lectureRAM(52));
        PTCONbits.PTEN = 1;
        while(PTCONbits.PTEN == 0); //Attend que la PWM soit activé
    }
    else
    {
        PTCONbits.PTEN = 0;
    }
}

void Interaction_AlarmShutdown()
{
    unsigned short tempAlarmShutdown = Eeprom_ReadWord(18);      //18--> Registre Alarme Shutdown
    unsigned short erreur = get_Erreur();
    
    if((tempAlarmShutdown & 1) == 1)      //Si on active Input Voltage error
    {
        if((erreur & 1) == 1)
            ecritureRAM(24, 0); //Alors on coupe la stifness
    }
    if((tempAlarmShutdown & 2) == 2)      //Si on active Angle limit error
    {
        //ecritureRAM(24, 0); //Alors on coupe la stifness
    }
    if((tempAlarmShutdown & 4) == 4)      //Si on active Overheating error
    {
        if((erreur & 2) == 2)
            ecritureRAM(24, 0); //Alors on coupe la stifness
    }
    if((tempAlarmShutdown & 8) == 8)      //Si on active Range error
    {
        //ecritureRAM(24, 0); //Alors on coupe la stifness
    }
    if((tempAlarmShutdown & 16) == 16)      //Si on active CheckSum error
    {
        if((erreur & 16) == 16)
            ecritureRAM(24, 0); //Alors on coupe la stifness
    }
    if((tempAlarmShutdown & 32) == 32)      //Si on active Overload error
    {
        //ecritureRAM(24, 0); //Alors on coupe la stifness
    }
    if((tempAlarmShutdown & 64) == 64)      //Si on active Instruction error
    {
        if((erreur & 64) == 64)
            ecritureRAM(24, 0); //Alors on coupe la stifness
    }
}
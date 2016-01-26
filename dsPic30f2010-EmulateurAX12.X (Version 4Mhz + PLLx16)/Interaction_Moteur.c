#include "Interaction_Moteur.h"
#include "RAM.h"
#include "eeprom.h"
#include "Gestion_UART.h"
#include "PWM.h"
#include "CAN10bits.h"


void Verifications()
{
    calculErreur(RESET_ERR);    //Reset les erreurs
    
    Verif_CourantTension();
    //Mettre les vérifs de voltage/courant et autre...
}

void Verif_CourantTension()
{
    float Tension = LectureTensionMoteur();
    float Courant = Tension / 1000; //Il y a une resistance de 1k après la lecture CAN =>  U=RI
    
    ecritureRAM(Voltage, Tension);  //Actualise les valeur de V et I
    ecritureRAM(Current, Courant);
    
    if(Tension > lectureRAM(TensionMax))
    {
        calculErreur(VOLT_ERR);
    }
    else if(Tension < lectureRAM(TensionMin))
    {
        calculErreur(VOLT_ERR);
    }
    
    if(Courant > lectureRAM(CourantMax))
    {
        calculErreur(CURRENT_ERR);
    }
}

void Interaction_Position()
{
    if(lectureRAM(Couple) == Activer)   //Si le couple est activé
    {
        //Alors on controle ici le moteur
        PWM_Position();
        PWM = Activer;
        while(PWM == Desactiver); //Attend que la PWM soit activé
    }
    else
    {
        PWM = Desactiver;
    }
}

void Interaction_LED()
{
    unsigned short tempAlarmLED = Eeprom_ReadWord(Alarm_LED);      //17--> Registre Alarme LED
    unsigned short erreur = get_Erreur();
    
    if(lectureRAM(LED) == 1 && Avertissement_LED != Allumer)   //T2CONbits.TON != 1 car peut etre que l'interupt est activé donc il y a un probleme
        PORT_LED = Allumer;
    else if(lectureRAM(LED) == 0 && Avertissement_LED != Allumer)
        PORT_LED = Eteind;
    
    
    if((tempAlarmLED & 1) == 1)      //Si on active Input Voltage error
    {
        if((erreur & 1) == 1)   //Alors lancer Timer pour faie clignoter la led
            Avertissement_LED = Allumer;  //Active le bip de la LED   
    }
    if((tempAlarmLED & 2) == 2)      //Si on active Angle limit error
    {
        
    }
    if((tempAlarmLED & 4) == 4)      //Si on active Overheating error
    {
        if((erreur & 2) == 2)
            Avertissement_LED = Allumer;  //Active le bip de la LED
            //Alors lancer Timer pour faie clignoter la led
    }
    if((tempAlarmLED & 8) == 8)      //Si on active Range error
    {

    }
    if((tempAlarmLED & 16) == 16)      //Si on active CheckSum error
    {
        if((erreur & 16) == 16)
            Avertissement_LED = Allumer;  //Active le bip de la LED
            //Alors lancer Timer pour faie clignoter la led
    }
    if((tempAlarmLED & 32) == 32)      //Si on active Overload error
    {

    }
    if((tempAlarmLED & 64) == 64)      //Si on active Instruction error
    {
        if((erreur & 64) == 64)
            Avertissement_LED = Activer;  //Active le bip de la LED
            //Alors lancer Timer pour faie clignoter la led
    }
}

void Interaction_AlarmShutdown()
{
    unsigned short RegistreAlarmShutdown = Eeprom_ReadWord(Alarm_Shutdown);      //18--> Registre Alarme Shutdown
    unsigned short erreur = get_Erreur();
    
    if((RegistreAlarmShutdown & 1) == 1)      //Si on active Input Voltage error
    {
        if((erreur & 1) == 1)
            ecritureRAM(Couple, Desactiver); //Alors on coupe la stifness
    }
    if((RegistreAlarmShutdown & 2) == 2)      //Si on active Angle limit error
    {
        //ecritureRAM(Couple, 0); //Alors on coupe la stifness
    }
    if((RegistreAlarmShutdown & 4) == 4)      //Si on active Overheating error
    {
        if((erreur & 2) == 2)
            ecritureRAM(Couple, Desactiver); //Alors on coupe la stifness
    }
    if((RegistreAlarmShutdown & 8) == 8)      //Si on active Range error
    {
        //ecritureRAM(Couple, 0); //Alors on coupe la stifness
    }
    if((RegistreAlarmShutdown & 16) == 16)      //Si on active CheckSum error
    {
        if((erreur & 16) == 16)
            ecritureRAM(Couple, Desactiver); //Alors on coupe la stifness
    }
    if((RegistreAlarmShutdown & 32) == 32)      //Si on active Overload error
    {
        //ecritureRAM(Couple, 0); //Alors on coupe la stifness
    }
    if((RegistreAlarmShutdown & 64) == 64)      //Si on active Instruction error
    {
        if((erreur & 64) == 64)
            ecritureRAM(Couple, Desactiver); //Alors on coupe la stifness
    }
}
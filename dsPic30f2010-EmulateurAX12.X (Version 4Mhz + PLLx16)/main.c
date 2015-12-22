/* 
 * File:   main.c
 * Author: azerty
 *
 * Created on 16 octobre 2015, 16:43
 */
/*
 
 * Fonction qui permet de connaitre la position pour initialiser PPL et PPH
 * Pas de gestion des RANGE ERROR (p.12 datasheet AX12)
 * Pas de gestion des ANGLE LIMIT ERROR
 * Pas de gestion des OVERLOAD ERROR
 * Continuer fonction calculErreur quand on connaitra la fonction calcul Voltage et temp�rature
 * Definir toutes les fonctions utilis�s seulement dans LE MEME fichier comme "static" 
 * Faire SYNC_WRITE dans la fonction lectureTrame 
 * Chercher un moyen d'utiliser le "DMA".   Sert � �viter d'ecrire directement dans l'eeprom car c'est tr�s long
 *      Donc copie dans RAM puis d�s qu'il a le temps il �crira.
 * 
 * 
 * 
 * 
 * XC16 USER's GUIDE : 
 *-> (13.5 Function allocation : ) Cases may arise when a specific function must be located at a specific address, or
within some range of addresses. The easiest way to accomplish this is by using the
address attribute, described in Section 13.2.1 ?Function Specifiers?. For example,
to locate function PrintString at address 0x8000 in program memory:
int __attribute__ ((address(0x8000))) PrintString (const char *s);
 * 
 *-> (13.6 Inline Function : ) By declaring a function inline, you can direct the compiler to integrate that function?s
code into the code for its callers. This usually makes execution faster by eliminating the
function-call overhead. In addition, if any of the actual argument values are constant,
their known values may permit simplifications at compile time, so that not all of the
inline function?s code needs to be included. The effect on code size is less predictable.
Machine code may be larger or smaller with inline functions, depending on the
particular case. 
 * 
 */

#include "Include_Library.h"
#include "Config.h"
#include "eeprom.h"
#include "Gestion_UART.h"
#include "RAM.h"
#include "Interaction_Moteur.h"
#include "Gestion_Interruption.h"
#include "PWM.h"

int main(int argc, char** argv) 
{
    Eeprom_WriteWord(4, 207);  //Baud Rate
    UART_init();
    Interrupts_init();
    RAM_init();
    PWM_init();   //Prescaleur 1:16   // Periode � 20ms
    //ResetEpprom();
    TRISB = 0;
    T2CONbits.TON = 1;  //Fait clignoter la led 2 fois au d�marrage
    
    /*
    PWM_rapportCyclique(25);
        PTCONbits.PTEN = 1;
        while(PTCONbits.PTEN == 0); //Attend que la PWM soit activ�
    */
    while(lectureRAM(47) == 0)  //Tand que le "lock" n'est pas activ�.
    {
        //PORTBbits.RB0 = 1;
        Verifications();    //V�rifie la tension et le courant max
        Interaction_LED();  //Fait clignoter la led si une erreur est trouv�
        Interaction_AlarmShutdown();
        Interaction_Position(); //Lance le moteur
        
        
        
        /*
        if(Eeprom_ReadWord(3) == 12)
            PORTEbits.RE2 = 1; //Allume la led
        else if (Eeprom_ReadWord(3) != 12)
            PORTEbits.RE2 = 0;   //Etteind la led
        */
    }
    
    RAM_init(); //On reset toutes les donn�es
    
    return (EXIT_SUCCESS);
}

/*
 PORTBbits.RB0 = 1;
delay_ms(1000);
PORTBbits.RB0 = 0;
delay_ms(1000);
 */
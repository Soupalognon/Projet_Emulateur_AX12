#include "Gestion_Interruption.h"
#include "Gestion_UART.h"


_Bool tempLED = 1;
unsigned short bipLed = 0;


void Interrupts_init()
{
    //Config Multi interruptions
    INTCON1bits.NSTDIS = 0;
    
    //Gestion des priorités
        //Si on met sa il faudra creer une fct qui active/desactive la priorité du CPU (passe de 7 à 0 et inversement)
    //IPC1bits.T2IP = 4;   //Priorité Timer 2
    //IPC2bits.U1RXIP = 7; //Priorité RX
    //IPC2bits.U1TXIP = 6; //Priorité TX
    //SRbits.IPL = 4;     //Priorité CPU
    
    //Interruptions TX et RX
    IEC0bits.U1TXIE = 0;
    IEC0bits.U1RXIE = 1;
    
    //Interruptions Timer 2
    PR2 = 0x7A12;   //500ms entre chaque inter
    T2CONbits.TCKPS = 3;    //Prescaler 1:256
    T2CONbits.TCS = 0;      //Utilise Fosc / 4 comme horloge
    T2CONbits.T32 = 0;      //Utilise un timer 16 bits
    T2CONbits.TGATE = 0;
    T2CONbits.TSIDL = 0;
    T2CONbits.TON = 0;  //Désactive le timer 2
    IEC0bits.T2IE = 1;  //Active l'interruption du timer 2
    
    
    //Interruption Timer 3
    PR3 = 0x186A; //100ms entre chaque inter.
    T3CONbits.TCKPS = 3;    //Prescaler 1:256
    T3CONbits.TCS = 0;      //Utilise Fosc / 4 comme horloge
    T3CONbits.TGATE = 0;
    T3CONbits.TSIDL = 0;
    T3CONbits.TON = 0;  //Désactive le timer 3
    IEC0bits.T3IE = 1;  //Active l'interruption du timer 3
    
    
    //Reset des Flags
    IFS0bits.U1TXIF = 0;
    IFS0bits.U1RXIF = 0;
    IFS0bits.T2IF = 0;
    IFS0bits.T3IF = 0;
}


void __attribute__((interrupt,no_auto_psv)) _T2Interrupt(void)
{
    //Faite bipper la led 2 fois si il voit une erreur lié à la variable eeprom Alarm LED
    IFS0bits.T2IF = 0;
    
    PORTBbits.RB0 = tempLED;
    tempLED = !tempLED;
    bipLed++;
    
    if(bipLed > 3)
    {
        bipLed = 0;
        T2CONbits.TON = 0;  //Désactive le timer
    }
}

void __attribute__((interrupt,no_auto_psv)) _T3Interrupt(void)
{
    //Vérifie le temps entre les octets pour la réception
    //Si delai > 100ms alors on reset itr
    IFS0bits.T3IF = 0;
    
    T3CONbits.TON = 0;  //Désactive le timer 3
    TMR3 = 0x00;
    
    reset_itr();
}

void __attribute__((interrupt,no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0;
    
    
    //PORTBbits.RB0 = 1;
}

void __attribute__((interrupt,no_auto_psv)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;
    
    UART_RX();
    
    //PORTBbits.RB0 = 1;
}

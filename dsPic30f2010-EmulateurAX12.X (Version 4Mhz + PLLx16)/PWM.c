#include "PWM.h"
#include "RAM.h"

/*
 Single-Event Mode
In the Single Event Counting mode, the PWM time base will begin counting upwards when the
PTEN bit is set. When the PTMR value matches the PTPER register, the PTMR register will be
reset on the following input clock edge and the PTEN bit will be cleared by the hardware to halt
the time base.
 */

unsigned int calculPrescaler(unsigned int ptmrPrescaler)
{
    if(ptmrPrescaler == 64)
        return 0b11;
    else if(ptmrPrescaler == 16)
        return 0b10;
    else if(ptmrPrescaler == 4)
        return 0b01;
    else
        return 0b00;
}


void PWM_init()
{ 
    PTCONbits.PTEN = 0; //Désactive les PWM
    
    //50 --> Prescaler      51 --> Periode
    float Periode = lectureRAM(Periode_PWM); 
    Periode /= 1000;    //On doit passer de sec à msec
    unsigned int Prescaler = lectureRAM(Prescaler_PWM);
    
    PTPER = (FCY * Periode / Prescaler) - 1; //Configuration Periode PWM
    //PTPER = 19999; //Configuration Periode PWM à 20ms avec pescaler 1:16 
    PTMR = 0;   //Reset du conteur PWM
    PDC1 = 0;   //De base la PWM a un rapport cyclique de 0%
    
    PTCONbits.PTSIDL = 0;   //La base de temps du PWM (PTMR) continu même en idle mode
    //PTCONbits.PTMOD = 0b01;     //Config du mode en Single Event Mode
    PTCONbits.PTMOD = 0b00;     //Config du mode en Free Running Mode
    PTCONbits.PTOPS = 0b0000;   //Postscaler à 1:1
    PTCONbits.PTCKPS = calculPrescaler(Prescaler);    //Prescaler configurer dans la fonction calculPrescaler()
    //PTCONbits.PTCKPS = 0b10;    //Prescaler configurer dans la fonction calculPrescaler()
    
    PWMCON1bits.PMOD1 = 1;  //Met la pin PWM1 en sortie indépendante
    PWMCON1bits.PMOD2 = 1;  //Met la pin PWM2 en sortie indépendante   
    PWMCON1bits.PMOD3 = 1;  //Met la pin PWM3 en sortie indépendante
    
    PWMCON1bits.PEN1H = 1;  //Active la pin PWM1
    PWMCON1bits.PEN1L = 1;  //Active la pin PWM1
    //Configure les autres pins PWM en I/O
    PWMCON1bits.PEN2H = 0;
    PWMCON1bits.PEN2L = 0;
    PWMCON1bits.PEN3H = 0;
    PWMCON1bits.PEN3L = 0;    
    
    PWMCON2bits.IUE = 0;    //La mise à jour le PDC (en cas de changement) est synchronisé avec la PTMR
    PWMCON2bits.OSYNC = 1;  //L'utilisation du PWM manuel est synchrone avec le PTMR
    PWMCON2bits.UDIS = 0;   //Permet d'activer la mise à jour du PDC et PTPER
    
    //DTCON1bits; //Dead time control
    //FLTACONbits;    //FLTA Pin Control      //Je sais pas ce que sais
    
    //Permet de controler les PWM grâce au generateur de PWM
    OVDCONbits.POVD1H = 1;
    OVDCONbits.POVD1L = 1;
    OVDCONbits.POVD2H = 1;
    OVDCONbits.POVD2L = 1;
    OVDCONbits.POVD3H = 1;
    OVDCONbits.POVD3L = 1;
}


void PWM_rapportCyclique(unsigned int rapport)
{
    float Periode = lectureRAM(Periode_PWM); 
    Periode /= 1000;    //On doit passer en msec
    unsigned short Prescaler = lectureRAM(Prescaler_PWM);
    
    unsigned long temp = 2*(FCY * Periode / Prescaler); //On mets la valeur maximum
    temp *= rapport;    //On réduit au rapport voulu
    temp /= 100U;       //Rajout la division par 100 au rapport demandé
    PDC1 = temp;    //Configure le rapport cyclique
    //PDC1 max = 2*PTPER
}

void PWM_Position()
{
    unsigned int _AngleMax = lectureRAM(AngleMax);
    
    float Periode = lectureRAM(Periode_PWM);
    Periode /= 1000;    //On doit passer en msec
    unsigned short Prescaler = lectureRAM(Prescaler_PWM);
    
    unsigned int Position_Desirer = lectureRAM(Position_Desirer_L) << 8;    //on recupère la position désiré sur 16bits
    Position_Desirer |= lectureRAM(Position_Desirer_H);
    
    float rapport = 1 + (Position_Desirer / _AngleMax);    //Calcul du temps sur une base de 2ms => 300° ; 1ms => 0°
    rapport /= 20;      //On divise par la periode pour le rapport Cyclique
    
    unsigned long temp = 2*(FCY * Periode / Prescaler); //On mets la valeur maximum
    temp *= rapport;    //On réduit au rapport voulu
    PDC1 = temp;    //Configure le rapport cyclique
    //PDC1 max = 2*PTPER
}
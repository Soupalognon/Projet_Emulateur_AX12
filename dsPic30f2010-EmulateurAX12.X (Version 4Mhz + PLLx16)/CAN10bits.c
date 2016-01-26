#include "CAN10bits.h"

unsigned int TAD = 154; //Temps minimum des cycles

void CAN_init()
{
    ADCON1bits.SSRC = 0b111;    //Temps de sample prédéfini dans 
    ADCON1bits.ASAM = 0;        //Sample activé par la mise à 1 du bit SAMP
    ADCON1bits.FORM = 0b00; //Configure la valeur de sortie en entier
    
    ADCON2 = 0;   
    
    ADCON3bits.ADRC = 0; //Utilise la clock externe comme clock de conversion
    ADCON3bits.SAMC = 3;  //Auto sample de 3TAD
    //ADCON3bits.ADCS = 2*TAD*FCY/1000000000 - 1;
    ADCON3bits.ADCS = 4;    //Vitesse max de la clock pour FCY=16MHz
    
    ADCHSbits.CH0SA = 0b0001;   //Connecte AN1 au Channel 0
    ADPCFG = 0b11101;   //RB1 est une entrée analogique
    
    TRISBbits.TRISB1 = 1;   //La pin B1 est en entrée
    
    //IEC0bits.ADIE = 1;  //Active l'interruption de la CAN pour la fin de la conversion
    
    ADCON1bits.ADON = 1;    //Active CAN
    
    //IFS0bits.ADIF = 0; //Reset du Flag CAN
}

float LectureTensionMoteur()
{
    LancerCAN = 1;
    
    while(!FinConversionCAN);
    
    float Tension = 3.3 * BufferCAN / 1024.0;
    Tension *= 7.3;  //(63000+10000)/10000   ou (R1+R2)/R2
    
    return Tension;
}
#include "RAM.h"

int poubelle;   //Permet d'éviter un bug de réattribution de la première variable avec la dernière à chaque fois

//Ram Config AX12
int TEN;   //Torque Enable
int LED;   //LED
int PPL;    //Present Position (L)          //Fonction permettant de connaitre La position
int PPH;     //Present Position (H)         //Fonction permettant de connaitre La position
int GPL;   //Goal Position (L)              
int GPH;   //Goal Position (H)              
int MSL;   //Moving Speed (L)
int MSH;   //Moving Speed (H)
int PV;   //Present Voltage
int PT;   //Present Temperature
int RI;  //Registered Instruction
int L;   //Lock
int PLL;   //Punch (L)
int PLH;   //Punch (H)

//RAM config Moteur PWM
unsigned short PrescalerPWM;
unsigned short PeriodePWM;
unsigned short RapportCycliquePWM;


void RAM_init()
{
    TEN = 0;   //Torque Enable
    LED = 0;   //LED
    PPL = 0;    //Present Position (L)          //Fonction permettant de connaitre La position
    PPH = 0;     //Present Position (H)         //Fonction permettant de connaitre La position            
    MSL = 0;   //Moving Speed (L)
    MSH = 0;   //Moving Speed (H)
    RI = 0;  //Registered Instruction
    L = 0;   //Lock
    PLL = 32;   //Punch (L)
    PLH = 0;   //Punch (H)
    GPL = PPL;
    GPH = PPH;
    
    PrescalerPWM = 16;
    PeriodePWM = 20;
    RapportCycliquePWM = 25;
}


void ecritureRAM(unsigned short adresse, unsigned short value)
{
    switch(adresse)
    {
        case 50:
            PrescalerPWM = value;
            break;
        case 51:
            PeriodePWM = value;
            break;
        case 52:
            RapportCycliquePWM = value;
            break;
        case 24:
            TEN = value;
            break;
        case 25:
            LED = value;
            break;
        case 30:
            GPL = value;
            break;
        case 31:
            GPH = value;
            break;
        case 32:
            MSL = value;
            break;
        case 33:
            MSH = value;
            break;
        case 44:
            RI = value;
            break;
        case 47:
            L = value;
            break;
        case 48:
            PLL = value;
            break;
        case 49:
            PLH = value;
            break;
            
        default:
            break;
    }
}


unsigned short lectureRAM(unsigned short adresse)
{
    switch(adresse)
    {
        case 50:
            return PrescalerPWM;
            break;
        case 51:
            return PeriodePWM;
            break;
        case 52:
            return RapportCycliquePWM;
            break;
        case 24:
            return TEN;
            break;
        case 25:
            return LED;
            break;
        case 30:
            return GPL;
            break;
        case 31:
            return GPH;
            break;
        case 32:
            return MSL;
            break;
        case 33:
            return MSH;
            break;
        case 36:
            return PPL;
            break;
        case 37:
            return PPH;
            break;
        case 42:
            return PV;
            break;
        case 43:
            return PT;
            break;
        case 44:
            return RI;
            break;
        case 47:
            return L;
            break;
        case 48:
            return PLL;
            break;
        case 49:
            return PLH;
            break;
            
        default:        //Ecrire l'erreur ICI
            return 0;
            break;
    }
}
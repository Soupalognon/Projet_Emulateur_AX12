#include "RAM.h"

int poubelle;   //Permet d'éviter un bug de réattribution de la première variable avec la dernière à chaque fois

//Ram Config AX12
int _TEN;   //Torque Enable
int _LED;   //LED
int _PPL;    //Present Position (L)          //Fonction permettant de connaitre La position
int _PPH;     //Present Position (H)         //Fonction permettant de connaitre La position
int _GPL;   //Goal Position (L)              
int _GPH;   //Goal Position (H)              
int _MSL;   //Moving Speed (L)
int _MSH;   //Moving Speed (H)
int _PV;   //Present Voltage
int _PT;   //Present Temperature
int _RI;  //Registered Instruction
int _L;   //Lock
int _PLL;   //Punch (L)
int _PLH;   //Punch (H)

//RAM config Moteur PWM
unsigned short _PrescalerPWM;
unsigned short _PeriodePWM;
unsigned short _RapportCycliquePWM;

float _TensionMax;   //En Volt
float _TensionMin;
float _CourantMax;

unsigned int _AngleMax;


void RAM_init()
{
    _TEN = 0;   //Torque Enable
    _LED = 0;   //LED
    _PPL = 0;    //Present Position (L)          //Fonction permettant de connaitre La position
    _PPH = 0;     //Present Position (H)         //Fonction permettant de connaitre La position            
    _MSL = 0;   //Moving Speed (L)
    _MSH = 0;   //Moving Speed (H)
    _RI = 0;  //Registered Instruction
    _L = 0;   //Lock
    _PLL = 32;   //Punch (L)
    _PLH = 0;   //Punch (H)
    _GPL = _PPL;
    _GPH = _PPH;
    
    _PrescalerPWM = 16;
    _PeriodePWM = 20;
    _RapportCycliquePWM = 25;
    
    _TensionMax = 9.6;
    _TensionMin = 9.6;
    
    _AngleMax = 300; //En degrés
}


void ecritureRAM(unsigned short adresse, unsigned short value)
{
    switch(adresse)
    {
        case 24:
            _TEN = value;
            break;
        case 25:
            _LED = value;
            break;
        case 30:
            _GPL = value;
            break;
        case 31:
            _GPH = value;
            break;
        case 32:
            _MSL = value;
            break;
        case 33:
            _MSH = value;
            break;
        case 44:
            _RI = value;
            break;
        case 47:
            _L = value;
            break;
        case 48:
            _PLL = value;
            break;
        case 49:
            _PLH = value;
            break;
        case 50:
            _PrescalerPWM = value;
            break;
        case 51:
            _PeriodePWM = value;
            break;
        case 52:
            _RapportCycliquePWM = value;
            break;
        case 53:
            _TensionMax = value;
            break;
        case 54:
            _TensionMin = value;
            break;
        case 55:
            _CourantMax = value;
            break;
        case 56:
            _AngleMax = value;
            break;
        default:
            break;
    }
}


unsigned short lectureRAM(unsigned short adresse)
{
    switch(adresse)
    {
        case 24:
            return _TEN;
            break;
        case 25:
            return _LED;
            break;
        case 30:
            return _GPL;
            break;
        case 31:
            return _GPH;
            break;
        case 32:
            return _MSL;
            break;
        case 33:
            return _MSH;
            break;
        case 36:
            return _PPL;
            break;
        case 37:
            return _PPH;
            break;
        case 42:
            return _PV;
            break;
        case 43:
            return _PT;
            break;
        case 44:
            return _RI;
            break;
        case 47:
            return _L;
            break;
        case 48:
            return _PLL;
            break;
        case 49:
            return _PLH;
            break;
        case 50:
            return _PrescalerPWM;
            break;
        case 51:
            return _PeriodePWM;
            break;
        case 52:
            return _RapportCycliquePWM;
            break;
        case 53:
            return _TensionMax;
            break;
        case 54:
            return _TensionMin;
            break;
        case 55:
            return _CourantMax;
            break;
        case 56:
            return _AngleMax;
            break;
            
        default:        //Ecrire l'erreur ICI
            return 0;
            break;
    }
}
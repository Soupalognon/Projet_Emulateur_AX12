#include "Gestion_UART.h"
#include "eeprom.h"
#include "RAM.h"
#include "Interaction_Moteur.h"
#include "PWM.h"
#include "CAN10bits.h"

//Variables de gestion réception de données
short itr = 0;
unsigned short RXData[8];
unsigned short _ID;
unsigned short tailleRX;
unsigned short Instruction;
unsigned short CheckSumTrame;
//unsigned int DerniereCheckSum;
unsigned short Erreur = 0;
unsigned short Para[] = {};


//Permet de stocker les données lors d'un REG_WRITE
unsigned short bufferInstruction;
unsigned short bufferParametres[] = {};


unsigned short get_Erreur()
{
    return Erreur;
}

unsigned short get_tailleRX()
{
    return tailleRX;
}

void reset_itr()
{
    itr = 0;
}

void UART_init()
{
    unsigned long baudRate = 2000000 / (Eeprom_ReadWord(Baud) + 1);
    U1BRG = (FCY/(16*baudRate)) - 1;
    //U1BRG = 4;     //Configure à 9600 baud pour Fosc à 20MHz   
    //U1BRG = 129;     //Configure à 9600 baud pour Fosc à 20MHz   
    //U1BRG = 64;      //Configure à 9600 baud pour Fosc à 10MHz 
    //U1BRG = 47;      //Configure à 9600 baud pour Fosc à 30MHz 
    //U1BRG = 194;     //Configure à 9600 baud pour Fosc à 120MHz 
    
    //Config TX
    U1STAbits.UTXISEL = 0;  
    
    //Config RX
    //met le flag à 1 à chaque bit reçu
    U1STAbits.URXISEL1 = 0;
    U1STAbits.URXISEL0 = 1;
        
    U1MODEbits.UARTEN = 1;  //Active l'UART
    U1STAbits.UTXEN = 1;    //Active TX (rx actif tout le temps)
    U1MODEbits.ALTIO = 1;   //Configure TX et RX sur U1ATX et U1ARX
    
    //Initialisation des données de réception
    int i;
    for(i = 0; i<8; i++)
        RXData[i] = 0;
}


void UART_TX(unsigned short data)
{    
    while(U1STAbits.UTXBF); //On attend si le buffer est plein
    U1TXREG = data;
}

void envoi_Trame(unsigned short _ID, unsigned short *parametres, unsigned short nbParametres)
{
    int checkSum = _ID + Erreur + nbParametres + 2;
    int i;
    
    //Active TX
    PORT_TX = Active_TX;
    
    delay_us(2 * Eeprom_ReadWord(Delai_Retour_Transmission));   //On attend le temps configuré dans le "Return Delay"
    
    UART_TX(0xFF);
    UART_TX(0xFF);
    UART_TX(_ID);//ID
    UART_TX(nbParametres + 2);//Taille
    UART_TX(Erreur);
    for(i=0; i<nbParametres; i++)
    {
        UART_TX(parametres[i]);
        checkSum += parametres[i];
    }
    UART_TX(255 - checkSum%256);
    
    //Active RX
    PORT_RX = Active_RX;
}


void UART_RX()
{
    //PORTBbits.RB0 = 1;
    
    //if(U1STAbits.FERR == 0) //Si le bit de stop du message est bien à 1
    if(Flag_Timer_Reception == 0) //Si le delai de 100ms n'est pas passé
    {
        Timer_Reception = OFF;  //Désactive le timer 3
        TMR3 = 0x00;
        //IEC0bits.T3IE = 0;  //Desactive l'interruption du timer 3
        
        RXData[itr] = U1RXREG;
        
        if(RXData[0] != 255)
        {
            itr = 0;
            return;
        }
        
        if(itr == 1)
        {
            if(RXData[1] != 255)
            {
                itr = 0;
                return;
            }
        }
        
        if(itr == 3)    //Permet de connaitre la taille pour savoir quand s'arreter
            tailleRX = RXData[3];

        itr++;
        
        if(itr > (tailleRX + 3))    //Si on a tout recu
            lectureTrame();

        Timer_Reception = ON;  //Active le timer 3 Pour vérifier le temps d'attente
        //IEC0bits.T3IE = 1;  //Active l'interruption du timer 3
    }
}


void lectureTrame()
{
    itr = 0;    //Si on arrive là alors sa veut dire que la réception est fini donc on reset l'iterateur de réception
    
    _ID = RXData[2];
    Instruction = RXData[4];
    CheckSumTrame = RXData[tailleRX+3];
    unsigned short DerniereCheckSum = 0;
       
    unsigned short i;
    for(i=0; i<(tailleRX-2); i++)
    {
        Para[i] = RXData[i+5];
        DerniereCheckSum += Para[i];
    }
    
    DerniereCheckSum += _ID + tailleRX + Instruction;
    DerniereCheckSum = 255 - DerniereCheckSum%255;
        
    if(CheckSumTrame == DerniereCheckSum)  
    { 
    //Alors bonne réception du message
        analyseTrame();
        
    }
    
    else    //Sinon Erreur il faut recommencer....
    {   
        if(_ID != 0xFE && Eeprom_ReadWord(Status_Return_Level) != 0)
        {
            //Car il ne faut pas qu'il renvoi si ils sont plusieurs
            calculErreur(CHECKSUM_ERR);
            unsigned short parametres[] = {};
            envoi_Trame(_ID, parametres, 0);
            calculErreur(RESET_ERR);
        }
    }
    
    //Reset du buffer de réception
    for(i = 0; i<8; i++)
        RXData[i] = 0;
}


void calculErreur(int tempErreur)
{
    if(tempErreur == INST_ERR) //Signifie qu'il y a une erreur d'instruction
        Erreur |= 1 << 6;
    if(tempErreur == CHECKSUM_ERR)
        Erreur |= 1 << 4;
    if(tempErreur == CURRENT_ERR)    //Signigfie que le moteur est trop chaud/froid
        Erreur |= 1 << 2;
    if(tempErreur == VOLT_ERR)    //Signifie erreur de sur/sous voltage
        Erreur |= 1 << 0;
    if(tempErreur == RESET_ERR)
        Erreur = 0;
}


void analyseTrame()
{
    if(_ID == Eeprom_ReadWord(ID)  ||  _ID == 0xFE)
    {
        //Alors l'ID est le miens ou celui de tout le monde
        
        if(Instruction == PING && _ID != 0xFE && Eeprom_ReadWord(Status_Return_Level) != 0)   
        {
            //Car il ne faut pas qu'il renvoi si ils sont plusieurs
            unsigned short parametres[] = {};
            envoi_Trame(_ID, parametres, 0);
        }
        
        else if(Instruction == READ && _ID != 0xFE && Eeprom_ReadWord(Status_Return_Level) != 0)  
        {
            //Eeprom_ReadWord(16) != 0 donc configurer pour répondre
            //ID != 0xFE car il ne faut pas qu'il renvoi si ils sont plusieurs
            if(Para[0] >= 0 && Para[0] <= 23)    
            {
                unsigned short data = Eeprom_ReadWord(Para[0]);
                unsigned short temp[] = {data};
                envoi_Trame(_ID, temp, 1);
            }
            else if(Para[0] >= 24  &&  Para[0] < 53)
            {
                unsigned short data = lectureRAM(Para[0]);
                unsigned short temp[] = {Para[0], data};
                envoi_Trame(_ID, temp, 2);
            }
            else
            {
                calculErreur(INST_ERR);
                unsigned short parametres[] = {};
                envoi_Trame(_ID, parametres, 0);
                calculErreur(RESET_ERR);
                //Cette adresse n'existe pas ou n'est pas dans la ram ou l'eeprom
            }
        }
        
        else if(Instruction == WRITE)
        {
            if(Para[0] >= 0 && Para[0] <= 23)
            {
                Eeprom_WriteWord(Para[0], Para[1]);
            }
            else if(Para[0] >= 24  &&  Para[0] < 53)
            {
                ecritureRAM(Para[0], Para[1]);
            }
            else
            {
                calculErreur(INST_ERR);   
                //Signifie qu'il y a une erreur d'instruction
                //Cette adresse n'existe pas ou n'est pas dans la ram
            }

            if(_ID != 0xFE && Eeprom_ReadWord(Status_Return_Level) == 2)
            {
                unsigned short parametres[] = {Para[0], Para[1]};
                envoi_Trame(_ID, parametres, 2);
                calculErreur(RESET_ERR);
            }
            
            if(Para[0] == 4)    //Change le baud Rate si demandé
            {
                UART_init();
            }
            if(Para[0] == 51)
            {
                PWM_init();
            }
        }
        
        else if(Instruction == REG_WRITE)
        {
            bufferInstruction = Instruction;
            int i;
            for(i=0; i<tailleRX-2; i++)
                bufferParametres[i] = Para[i];
            
            ecritureRAM(Registre_Instruction, 1); //Dit qu'il est pret pour une instruction ACTION
        }
        
        else if(Instruction == ACTION)
        {
            if(lectureRAM(Registre_Instruction) == 1)
            {
                ecritureRAM(Registre_Instruction, 0);  //reset du flag
                
                if(Para[0] >= 0 && Para[0] < 24)
                {
                    Eeprom_WriteWord(bufferParametres[0], bufferParametres[1]);
                }
                else if(Para[0] >= 24  &&  Para[0] < 53)
                {
                    ecritureRAM(bufferParametres[0], bufferParametres[1]);
                }
                else
                {
                    calculErreur(INST_ERR);    
                    //Signifie qu'il y a une erreur d'instruction
                    //Cette adresse n'existe pas ou n'est pas dans la ram
                }
            }
            else
            {
                calculErreur(INST_ERR);
            }
            
            if(_ID != 0xFE && Eeprom_ReadWord(Status_Return_Level) == 2)
            {
                unsigned short parametres[] = {};
                envoi_Trame(_ID, parametres, 0);
            }
            calculErreur(RESET_ERR);
        }
        
        else if(Instruction == RESET)
        {
            ResetEpprom();
            RAM_init();
            if(_ID != 0xFE && Eeprom_ReadWord(Status_Return_Level) == 2)
            {
                unsigned short parametres[] = {};
                unsigned short tempID = Eeprom_ReadWord(ID);
                envoi_Trame(tempID, parametres, 0);
            }
            calculErreur(RESET_ERR);
        }
        
        else if(Instruction == SYNC_WRITE)
        {
            if(lectureRAM(Registre_Instruction))  //reset du flag)
            {
                ecritureRAM(Registre_Instruction, 0);  //reset du flag
            }
            else
            {
                calculErreur(INST_ERR);
            }
            
            if(_ID != 0xFE && Eeprom_ReadWord(Status_Return_Level) == 2)
            {
                unsigned short parametres[] = {};
                envoi_Trame(_ID, parametres, 0);
            }
            calculErreur(RESET_ERR);
        }
        
        else
        {
            if(_ID != 0xFE && Eeprom_ReadWord(Status_Return_Level) == 2)
            {
                calculErreur(INST_ERR);
                unsigned short parametres[] = {};
                envoi_Trame(_ID, parametres, 0);
            }
            calculErreur(RESET_ERR);
        }
    }
    
    else
        //Alors l'ID n'est pas le bon. On ne lit pas l'information
    {
        //PORTBbits.RB0 = 0;
    }
}
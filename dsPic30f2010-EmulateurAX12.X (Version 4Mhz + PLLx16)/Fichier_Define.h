/* 
 * File:   Fichier_Define.h
 * Author: user
 *
 * Created on 17 janvier 2016, 14:40
 */

#ifndef FICHIER_DEFINE_H
#define	FICHIER_DEFINE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    //Config
    #define FOSC            64000000LL
    //#define FOSC            120000000LL
    #define FCY             (FOSC/4)
    #define delay_us(x) __delay32(((x*FCY)/1000000L)) // delays x us
    #define delay_ms(x) __delay32(((x*FCY)/1000L))      // delays x ms

    //Classic
    #define Activer 1
    #define Desactiver 0
    #define ON 1
    #define OFF 0
    #define Allumer 1
    #define Eteind 0
    #define Entree 1
    #define Sortie 0

    //RX et TX
    #define PORT_SERIE PORTD
    #define Active_RX 0b10
    #define Active_TX 0b01
    
    #define PWM PTCONbits.PTEN
    
    #define PORT_LED PORTBbits.RB0
    #define Avertissement_LED T2CONbits.TON
    
    //Timer dédier au time out de réception
    #define Timer_Reception T3CONbits.TON
    #define Flag_Timer_Reception IFS0bits.T3IF


    //Pour gestion UART
    #define PING 0x01
    #define READ 0x02
    #define WRITE 0x03
    #define REG_WRITE 0x04
    #define ACTION 0x05
    #define RESET 0x06
    #define SYNC_WRITE 0x83

    #define INST_ERR 0
    #define CURRENT_ERR 1
    #define VOLT_ERR 2
    #define CHECKSUM_ERR 3
    #define RESET_ERR 255

    #define Baud_9600 207
    #define Baud_19200 103
    #define Baud_250000 7
    #define Baud_500000 3

    //Pour eeprom
    #define ID 3
    #define Baud 4
    #define Delai_Retour_Transmission 5
    #define Couple_Max_L 14
    #define Couple_Max_H 15
    #define Status_Return_Level 16
    #define Alarm_LED 17
    #define Alarm_Shutdown 18
    
    //Pour RAM
    #define Couple 24
    #define LED 25
    #define Position_Desirer_L 30
    #define Position_Desirer_H 31
    #define Vitesse_Deplacement_L 32
    #define Vitesse_Deplacement_H 33
    #define Position_Actuelle_L 36
    #define Position_Actuelle_H 37
    #define Voltage 42
    #define Current 43
    #define Registre_Instruction 44
    #define Lock 47
    #define Punch_L 48
    #define Punch_H 49
                        //Supplémentaires
    #define Prescaler_PWM 50
    #define Periode_PWM 51
    #define Rapport_Cyclique_PWM 52
    #define TensionMax 53
    #define TensionMin 54
    #define CourantMax 55
    #define AngleMax 56
    
    
    //CAN10bits
    #define BufferCAN ADCBUF0
    #define FinConversionCAN ADCON1bits.DONE
    #define LancerCAN ADCON1bits.SAMP
    
#ifdef	__cplusplus
}
#endif

#endif	/* FICHIER_DEFINE_H */


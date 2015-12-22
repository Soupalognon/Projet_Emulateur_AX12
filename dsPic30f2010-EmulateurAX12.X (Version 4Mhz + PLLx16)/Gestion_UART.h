/* 
 * File:   Gestion_UART.h
 * Author: user
 *
 * Created on 18 octobre 2015, 16:34
 */
#include "Include_Library.h"

#define PING 0x01
#define READ 0x02
#define WRITE 0x03
#define REG_WRITE 0x04
#define ACTION 0x05
#define RESET 0x06
#define SYNC_WRITE 0x83

#define INST_ERR 0
#define TEMP_ERR 1
#define VOLT_ERR 2
#define CHECKSUM_ERR 3
#define RESET_ERR 255


unsigned short get_tailleRX();
unsigned short get_Erreur();
unsigned short get_Erreur();
void UART_TX(unsigned short data);
void envoi_Trame(unsigned short ID, unsigned short *parametres, unsigned short nbParametres);
void UART_RX();
void lectureTrame();
void analyseTrame();
void UART_init();
void calculErreur(int tempErreur);
void reset_itr();
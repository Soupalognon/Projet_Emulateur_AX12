/* 
 * File:   Gestion_UART.h
 * Author: user
 *
 * Created on 18 octobre 2015, 16:34
 */
#include "Include_Library.h"


unsigned short get_tailleRX();
unsigned short get_Erreur();
unsigned short get_Erreur();
void UART_TX(unsigned short data);
void envoi_Trame(unsigned short _ID, unsigned short *parametres, unsigned short nbParametres);
void UART_RX();
void lectureTrame();
void analyseTrame();
void UART_init();
void calculErreur(int tempErreur);
void reset_itr();
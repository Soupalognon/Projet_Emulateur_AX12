/* 
 * File:   Gestion_Interruption.h
 * Author: user
 *
 * Created on 11 novembre 2015, 15:47
 */

#include "Include_Library.h"

#ifndef GESTION_INTERRUPTION_H
#define	GESTION_INTERRUPTION_H

#ifdef	__cplusplus
extern "C" {
#endif


    
    
    void Interrupts_init();
    void __attribute__((interrupt,no_auto_psv)) _T2Interrupt(void);
    void __attribute__((interrupt,no_auto_psv)) _T3Interrupt(void);
    void __attribute__((interrupt,no_auto_psv)) _U1TXInterrupt(void);
    void __attribute__((interrupt,no_auto_psv)) _U1RXInterrupt(void);
    
    
    
    
    


#ifdef	__cplusplus
}
#endif

#endif	/* GESTION_INTERRUPTION_H */


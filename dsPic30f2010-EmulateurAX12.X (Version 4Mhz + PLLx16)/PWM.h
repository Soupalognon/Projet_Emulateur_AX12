/* 
 * File:   PWM.h
 * Author: azerty
 *
 * Created on 20 novembre 2015, 12:25
 */

#include "Include_Library.h"

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    unsigned int calculPrescaler(unsigned int ptmrPrescaler);
    void PWM_init();
    void PWM_rapportCyclique(unsigned int rapport);
    void PWM_periode(unsigned int PtmrPrescaler, unsigned int periode);

#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */


/*
 Configuration du PWM en:
 * Free Running Mode
 * Sorties Indépendentes
 * PWM1(H / L) uniquement active en tant que PWM.
 * 
 */
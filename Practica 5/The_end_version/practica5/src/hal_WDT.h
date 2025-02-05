/* *****************************************************************************
 * P.H.2024: hal_WDT, interface que nos independiza del hardware concreto
 */

#ifndef HAL_WDT_H
#define HAL_WDT_H

#include <stdint.h>

/** 
 * Inicializa el watchdog con un timeout especificado
 */
void hal_WDT_iniciar(uint32_t sec);   

/** 
 * Recarga el watchdog para evitar el timeout
 */
void hal_WDT_feed(void);                     

#endif 

/* *****************************************************************************
 * P.H.2024: hal_consumo, interface que nos independiza del hardware concreto
 */

#ifndef HAL_CONSUMO_H
#define HAL_CONSUMO_H

#include <stdint.h>

/**
 * Inicializa el módulo de consumo a nivel de hardware.
 */
void hal_consumo_iniciar(void);

/**
 * Pone el procesador en modo de espera.
 */
void hal_consumo_esperar(void);

/**
 * Pone el procesador en modo de bajo consumo (dormir).
 */
void hal_consumo_dormir(void);

#endif 

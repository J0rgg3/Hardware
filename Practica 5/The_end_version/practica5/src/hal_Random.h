/* *****************************************************************************
 * P.H.2024: m�dulo de declaraci�n del generador de n�meros aleatorios
 */

#ifndef HAL_RANDOM_H
#define HAL_RANDOM_H

#include <stdint.h>
#include <stdlib.h>

/**
 * Inicializa el generador de n�meros aleatorios
 */
void hal_random_iniciar(uint32_t semilla);

/**
 * Devuelve un n�mero aleatorio comprendido entre "min" y "max"
 */
uint32_t hal_random_generar(uint32_t min,uint32_t max);

#endif 

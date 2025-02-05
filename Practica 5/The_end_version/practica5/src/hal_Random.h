/* *****************************************************************************
 * P.H.2024: módulo de declaración del generador de números aleatorios
 */

#ifndef HAL_RANDOM_H
#define HAL_RANDOM_H

#include <stdint.h>
#include <stdlib.h>

/**
 * Inicializa el generador de números aleatorios
 */
void hal_random_iniciar(uint32_t semilla);

/**
 * Devuelve un número aleatorio comprendido entre "min" y "max"
 */
uint32_t hal_random_generar(uint32_t min,uint32_t max);

#endif 

/* *****************************************************************************
 * P.H.2024: hal_random, implementacion lpc acorde a hal_random.h
 */

#include <stdint.h>
#include <stdlib.h>
#include "hal_random.h"

/**
 * Inicializa el generador de números aleatorios
 */
void hal_random_iniciar(uint32_t semilla){

		srand(semilla);
}

/**
 * Devuelve un número aleatorio comprendido entre "min" y "max"
 */
uint32_t hal_random_generar(uint32_t min,uint32_t max){

	return (rand() % max) + min;
}

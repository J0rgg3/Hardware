/* *****************************************************************************
 * P.H.2024: hal_random, implementacion nrf acorde a hal_random.h
 */

#include <stdint.h>
#include <stdlib.h>
#include "hal_random.h"
#include <nrf.h>

/**
 * Inicializa el generador de números aleatorios
 */
void hal_random_iniciar(uint32_t semilla){

		NRF_RNG -> TASKS_START = 1; // Inicia el generador 
		
}

/**
 * Devuelve un número aleatorio comprendido entre "min" y "max"
 */
uint32_t hal_random_generar(uint32_t min,uint32_t max){


	return (NRF_RNG->VALUE % max) + min;
}

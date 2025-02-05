/* *****************************************************************************
 * P.H.2024: hal_consumo en nrf, implementación acorde a hal_consumo.h
 */

#include "hal_consumo.h"
#include "nrf.h"  // Biblioteca específica de nRF si es necesario (opcional)

/**
 * Inicializa el módulo de consumo a nivel de hardware.
 */ 
void hal_consumo_iniciar(void) {
    // No es necesario hacer nada en algunos casos, pero definimos la función para consistencia.
}

/**
 * Pone el procesador en modo de espera (espera a interrupciones).
 */ 
void hal_consumo_esperar(void) {
    __WFI();  // Instrucción ARM para "Wait For Interrupt"
}

/**
 * Pone el procesador en modo de bajo consumo (dormir)
 */ 
void hal_consumo_dormir(void) {

    // Activa el modo de apagado de sistema
	
    NRF_POWER->SYSTEMOFF = POWER_SYSTEMOFF_SYSTEMOFF_Enter;
		
    // Asegurarse de que el procesador entre en modo bajo consumo (System OFF)
    while (1) {
        __WFE(); // Espera eventos de despertar
    }
}

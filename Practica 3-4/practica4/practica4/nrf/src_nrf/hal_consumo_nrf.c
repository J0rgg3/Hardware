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
 * Pone el procesador en modo de bajo consumo (también con WFI en este caso).
 */ 
void hal_consumo_dormir(void) {
    // Configura el procesador para que pueda despertar mediante una interrupción externa
    // Por ejemplo, configurando un GPIO para detección de nivel
    NRF_GPIO->PIN_CNF[PIN_WAKEUP] = (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos) |
                                     (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) |
                                     (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos);

    // Activa el modo de apagado de sistema
    NRF_POWER->SYSTEMOFF = POWER_SYSTEMOFF_SYSTEMOFF_Enter;

    // Asegurarse de que el procesador entre en modo bajo consumo (System OFF)
    while (1) {
        __WFE(); // Espera eventos de despertar, por ejemplo, para debug
    }
}

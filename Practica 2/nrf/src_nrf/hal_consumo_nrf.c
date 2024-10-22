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
    __WFI();  // Instrucción ARM para "Wait For Interrupt", dormimos el procesador hasta la próxima interrupción.
}

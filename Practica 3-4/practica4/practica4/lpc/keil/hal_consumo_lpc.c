#include "hal_consumo.h"
#include <LPC210x.H> 

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
  EXTWAKE = 1; // EXTINT0 will awake the processor
	PCON |= 0x01; 
}

/**
 * Pone el procesador en modo de bajo consumo (también con WFI en este caso).
 */ 
void hal_consumo_dormir(void) {
   EXTWAKE = 1; // EXTINT0 will awake the processor
	PCON |= 0x02; 
	switch_to_PLL(); //PLL aranca a 12Mhz cuando volvemos de powewr down
}
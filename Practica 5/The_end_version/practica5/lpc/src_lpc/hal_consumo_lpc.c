/* *****************************************************************************
 * P.H.2024: hal_consumo en lpc, implementación acorde a hal_consumo.h
 */

#include "hal_consumo.h"
#include <LPC210x.H>                       /* LPC210x definitions */

//definida en Startup.s
extern void Reset_Handler(void);

/**
 * Inicializa el módulo de consumo a nivel de hardware.
 */
void hal_consumo_iniciar(void) {
}

/**
 * Pone el procesador en modo de espera.
 */
void hal_consumo_esperar(void)  {
	EXTWAKE = 7;						// EXTINT0,1,2 will awake the processor
	PCON |= 0x01; 
}

/**
 * Pone el procesador en modo de bajo consumo (dormir).
 */
void hal_consumo_dormir(void)  {
	// Aseguramos que las interrupciones estan activas para poder despertarnos
	EXTWAKE = 7;
	
	PCON |= 0x02; 

	Reset_Handler(); 
}

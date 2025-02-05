/* *****************************************************************************
 * P.H.2024: hal_WDT, implementacion lpc acorde a hal_WDT.h
 */

#include "hal_WDT.h"
#include "hal_SC.h"
#include <LPC210x.H>                       /* LPC210x definitions */

/** 
 * Inicializa el watchdog con un timeout especificado
 */
void hal_WDT_iniciar(uint32_t sec) {
	WDTC = sec * (15000000 / 4); // time-out WatchDog.
	WDMOD = 0x03; // Habilitar y configurar reinicio.
	hal_WDT_feed();
}

/** 
 * Recarga el watchdog para evitar el timeout
 */
void hal_WDT_feed(void) { 
	SC_entrar_disable_irq();
	WDFEED = 0xAA; // Alimentar el WatchDog
	WDFEED = 0x55;
	SC_salir_enable_irq();
}

/* *****************************************************************************
 * P.H.2024: hal_WDT, implementacion nrf acorde a hal_WDT.h
 */

#include "hal_WDT.h"
#include "nrf.h"  // Incluye los registros y definiciones espec?ficas del nRF52840

/** 
 * Inicializa el watchdog con un timeout especificado
 */
void hal_WDT_iniciar(uint32_t sec) {
		// Configurar CRV (Counter Reload Value) para el timeout especificado
			NRF_WDT->CRV = (sec * 32768) - 1;  // LFCLK = 32.768 kHz, timeout en segundos

			if(NRF_WDT->RUNSTATUS !=0){
				return;
			}
			// Habilitar el canal de recarga 0
			NRF_WDT->RREN = WDT_RREN_RR0_Msk;  // Activa el primer canal de recarga
		
			NRF_WDT->CONFIG =(WDT_CONFIG_SLEEP_Run << WDT_CONFIG_SLEEP_Pos)|
											(WDT_CONFIG_HALT_Pause << WDT_CONFIG_HALT_Pos);

			// Iniciar el watchdog
			NRF_WDT->TASKS_START = 1;
		
			NRF_WDT->RR[0] = WDT_RR_RR_Reload;
	}

/** 
 * Recarga el watchdog para evitar el timeout
 */
void hal_WDT_feed(void) {
    // Recargar el primer canal para evitar el timeout
    NRF_WDT->RR[0] = WDT_RR_RR_Reload;
}

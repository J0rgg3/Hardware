/* *****************************************************************************
 * P.H.2024: Driver/Manejador de consumo
 * suministra los servicios independientemente del hardware
 */

#include "hal_consumo.h"
#include "drv_consumo.h"
#include "drv_monitor.h"
#include "board.h"

typedef uint32_t monitor;

static monitor despertar;
static monitor dormir;
	
/**
 * Inicializa el módulo de bajo consumo.
 */
void drv_consumo_iniciar(uint32_t mon_wait, uint32_t mon_dormir) {
    
		despertar = mon_wait;
		dormir = mon_dormir;

    // Inicializa el sistema de monitores
    drv_monitor_iniciar();
    
    // Marca el monitor para el estado despierto al iniciar
    drv_monitor_marcar(despertar);
		hal_consumo_iniciar();
}

/**
 * Pone el sistema en modo de espera.
 */
void drv_consumo_esperar(void) {
    // Desmarcar el monitor de despierto antes de poner el sistema en espera
    drv_monitor_desmarcar(despertar);
    
    // Llamar a la función de bajo consumo del hardware
    hal_consumo_esperar();
	
		drv_monitor_marcar(despertar);
}

/**
 * Pone el sistema en modo dormir (bajo consumo).
 */
void drv_consumo_dormir(void) {
	
    // Marcar el monitor de dormido
    drv_monitor_marcar(dormir);
    
    // Llamar a la función de dormir del hardware
    hal_consumo_dormir();    
}




/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los monitores
 * suministra los servicios de iniciar, marcar, desmarcar, ... independientemente del hardware
 */

#ifndef DRV_MONITOR
#define DRV_MONITOR

#include <stdint.h>

/**
 * inicializa los monitores, los deja apagados y devuelve el número de monitores disponibles en la plataforma
 */
uint32_t drv_monitor_iniciar(void);

/**
 * enciende el monitor id
 */
void drv_monitor_marcar(uint32_t id);

/**
 * apaga el monitor id
 */
void drv_monitor_desmarcar(uint32_t id);

/**
 * conmuta el monitor de on a off y viceversa
 */
void drv_monitor_conmutar(uint32_t id);



#endif

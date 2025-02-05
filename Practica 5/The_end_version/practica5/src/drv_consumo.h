/* *****************************************************************************
 * P.H.2024: Driver/Manejador de consumo
 * suministra los servicios independientemente del hardware
 */
 
#ifndef DRV_CONSUMO
#define DRV_CONSUMO

#include <stdint.h>



/**
 * Inicializa el módulo de bajo consumo.
 */
void drv_consumo_iniciar(uint32_t mon_wait, uint32_t mon_dormir);

/**
 * Pone el sistema en modo de espera.
 */
void drv_consumo_esperar(void);

/**
 * Pone el sistema en modo dormir (bajo consumo).
 */
void drv_consumo_dormir(void);


#endif

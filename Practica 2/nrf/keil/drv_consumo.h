/* *****************************************************************************
 * P.H.2024: Driver/Manejador de consumo
 * suministra los servicios independientemente del hardware
 */
 
#ifndef DRV_CONSUMO
#define DRV_CONSUMO

#include <stdint.h>
//#include "core_cm4.h"  // Incluye CMSIS y la función __WFI()

/**
 * define los tipos de datos asociados a consumo 
 */


/**
 * Inicializa el módulo de bajo consumo.
 */
void drv_consumo_iniciar(void);

/**
 * Poner el sistema en modo de espera.
 */
void drv_consumo_esperar(void);

/**
 * Poner el sistema en modo dormir (bajo consumo).
 */
void drv_consumo_dormir(void);


#endif

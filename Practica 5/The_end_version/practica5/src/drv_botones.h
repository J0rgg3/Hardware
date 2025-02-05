/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los botones
 * suministra los servicios de iniciar y tratar independientemente del hardware
 */
 
#ifndef DRV_BOTONES_H
#define DRV_BOTONES_H

#include <stdint.h>


/** 
 * Inicializa el m�dulo de botones, configur�ndolos como interrupciones externas,
 * suscribiendo los eventos correspondientes y finalmente, devuelve el n�mero de botones inicializados.
 */
uint32_t  drv_botones_iniciar( void(*f_callback)(uint32_t, uint32_t),uint32_t botonPulsar,uint32_t botonRetardo,uint32_t dosBotonesPulsados);

/**	
 * Gestiona los eventos de bot�n identificados por "evento" 
 */
void drv_botones_tratar(uint32_t evento, uint32_t auxData);


#endif 

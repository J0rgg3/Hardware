// drv_botones.h
#ifndef DRV_BOTONES_H
#define DRV_BOTONES_H

#include <stdint.h>
#include "rt_evento_t.h"

// Define los estados de la máquina de estados del botón
typedef enum {
    e_reposo,
    e_entrando,
    e_esperando,
    e_soltado
} Estado_boton;

/* Inicializa el módulo de botones
*/
void drv_botones_iniciar(uint32_t monitor);

/*	Gestiona los eventos de botón
*/
void drv_botones_tratar(EVENTO_T evento, uint32_t auxData);


#endif // DRV_BOTONES_H

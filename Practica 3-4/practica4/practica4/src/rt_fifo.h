#ifndef RT_FIFO_H
#define RT_FIFO_H

#include "rt_evento_t.h"
#include <stdint.h>

#define COLA_SIZE 64 // Capacidad de la cola

// Prototipos de funciones
/*
*	inicializa la cola de eventos.
*/
void rt_FIFO_inicializar(uint32_t monitor_overflow);

/*
*	añade un nuevo evento a la cola de eventos.
*/
void rt_FIFO_encolar(uint32_t ID_evento, uint32_t auxData);

/*
*	elimina un evento de la cola de eventos.
*	devuelve el # de eventos sin procesar (0 si no hay eventos sin procesar)
*/
uint8_t rt_FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData, uint32_t *TS);

/*
*	dado un evento, devuelve el número de veces que ha sido encolado.
*/
uint32_t rt_FIFO_estadisticas(EVENTO_T ID_evento);

#endif // RT_FIFO_H

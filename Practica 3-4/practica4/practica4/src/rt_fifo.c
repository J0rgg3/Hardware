#include "rt_fifo.h"
#include "drv_tiempo.h"
#include <stdio.h>


static EVENTO cola[COLA_SIZE]; // Vector circular de eventos
static uint8_t head = 0;       // Índice de inserción
static uint8_t tail = 0;       // Índice de extracción
static uint8_t num_eventos = 0; // Número de eventos en la cola
static uint32_t monitor_overflow; // Monitor para el índice de overflow

/*
*
*/

/*
*	inicializa la cola de eventos.
*/
void rt_FIFO_inicializar(uint32_t monitor) {
    monitor_overflow = monitor;
    head = 0;
    tail = 0;
    num_eventos = 0;
}

/*
*	añade un nuevo evento a la cola de eventos.
*/
void rt_FIFO_encolar(uint32_t ID_evento, uint32_t auxData) {
    if (num_eventos == COLA_SIZE) {
        // Overflow: detener la ejecución en un bucle infinito
        while(1);
    }
    // Obtener la marca de tiempo 
    uint32_t TS = drv_tiempo_actual_us(); 

    // Insertar el evento en la cola
    cola[head].ID_EVENTO = (EVENTO_T)ID_evento;
    cola[head].auxData = auxData;
    cola[head].TS = TS;

    // Avanzar el índice circular
    head = (head + 1) % COLA_SIZE;
    num_eventos++;
}

/*
*	elimina un evento de la cola de eventos.
*	devuelve el # de eventos sin procesar (0 si no hay eventos sin procesar)
*/
uint8_t rt_FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData, uint32_t *TS) {
    if (num_eventos == 0) {
        return 0; // No hay eventos pendientes
    }

    // Extraer el evento de la cola
    *ID_evento = cola[tail].ID_EVENTO;
    *auxData = cola[tail].auxData;
    *TS = cola[tail].TS;

    // Marcar el evento como procesado avanzando el índice
    tail = (tail + 1) % COLA_SIZE;
    num_eventos--;
    return num_eventos +1;
}

/*
*	dado un evento, devuelve el número de veces que ha sido encolado.
*/
uint32_t rt_FIFO_estadisticas(EVENTO_T ID_evento) {
    uint32_t cont = 0;
    for (uint8_t i = 0; i < COLA_SIZE; i++) {
        if (cola[i].ID_EVENTO == ID_evento) {
            cont++;
        }
    }
    return cont;
}

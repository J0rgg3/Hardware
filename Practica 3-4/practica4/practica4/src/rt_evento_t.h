#ifndef RT_EVENTO_T_H
#define RT_EVENTO_T_H

#include <stdint.h>

// Definimos los tipos de eventos
typedef enum {
    ev_VOID = 0,          // Evento vac�o por defecto
    ev_T_PERIODICO = 1,   // Evento peri�dico
    ev_PULSAR_BOTON = 2,  // Evento de pulsaci�n de bot�n
		ev_INACTIVIDAD = 3,
    ev_BOTON_RETARDO = 4  // Nuevo evento de retardo de bot�n
} EVENTO_T;

#define EVENT_TYPES 5 // N�mero total de tipos de eventos (ajustar este valor al agregar nuevos eventos)
#define ev_NUM_EV_USUARIO 2
#define ev_USUARIO {ev_PULSAR_BOTON, ev_BOTON_RETARDO}

// Estructura para almacenar un evento en la cola
typedef struct {
    EVENTO_T ID_EVENTO;  // Tipo de evento
    uint32_t auxData;    // Datos adicionales asociados
    uint32_t TS;         // Marca de tiempo en microsegundos
} EVENTO;

#endif // RT_EVENTO_T_H

/* *****************************************************************************
 * P.H.2024: rt_evento_t, interface que declara los tipos de eventos que se podrán gestionar en la cola
 */

#ifndef RT_EVENTO_T_H
#define RT_EVENTO_T_H

#include <stdint.h>

// Definimos los tipos de eventos
typedef enum {
    ev_VOID = 0,          		// Evento vacío por defecto
    ev_T_PERIODICO = 1,   		// Evento periódico
    ev_PULSAR_BOTON = 2,  		// Evento de pulsación de botón
    ev_INACTIVIDAD = 3,				// Evento de inactividad
		ev_BOTON_RETARDO = 4, 		// Evento para el retardo del botón
		ev_LED = 5,								// Evento para encender o apagar un led
		ev_TIMEOUT = 6,						// Evento para el timeout del WDT
		ev_RESULTADO = 7,					// Evento para el resultado de cada nivel del simon (acierto o fallo)
		ev_PULSAR_DOS_BOTONES = 8 // Evento para la pulsación de dos botones de manera simultánea
} EVENTO_T;

#define EVENT_TYPES 9 // Número total de tipos de eventos
#define ev_NUM_EV_USUARIO 2
#define ev_USUARIO {ev_PULSAR_BOTON,ev_BOTON_RETARDO}


#endif 

/* *****************************************************************************
 * P.H.2024: módulo de implementación de la cola FIFO de eventos
 */

#include "rt_fifo.h"
#include "drv_tiempo.h"
#include <stdio.h>
#include "hal_WDT.h"
#include "hal_SC.h"
#include "drv_monitor.h"

#define COLA_SIZE 64 // Capacidad de la cola


// Estructura para almacenar un evento en la cola
typedef struct {
    EVENTO_T ID_EVENTO;  // Tipo de evento
    uint32_t auxData;    // Datos adicionales asociados
    uint32_t TS;         // Marca de tiempo en microsegundos
} EVENTO;

static uint32_t conteo_eventos[EVENT_TYPES];//Vector donde se guarda el numero de veces que ha sido encolado cada evento
static EVENTO cola[COLA_SIZE]; // Vector circular de eventos
static uint8_t head = 0;       // Índice de inserción
static uint8_t tail = 0;       // Índice de extracción
static uint8_t num_eventos = 0; // Número de eventos en la cola
static uint32_t monitor_overflow; // Monitor para el índice de overflow

/**
 *	inicializa la cola de eventos.
 */
void rt_FIFO_inicializar(uint32_t monitor) {
    monitor_overflow = monitor;
    head = 0;
    tail = 0;
    num_eventos = 0;
		for(int i = 0;i<EVENT_TYPES;i++){		
			 conteo_eventos[i] = 0; 
					
		}
}

void rt_FIFO_conteo(uint32_t ID_evento){
	
	for(uint32_t i = 1;i<EVENT_TYPES;i++){//No se lleva la cuenta del evento void, ya que en su respecito lugar en el vector[0], se almacena el numero total de evento encolados
		if(ID_evento == i){
			conteo_eventos[i]++;
			conteo_eventos[0]++;
			return;
		}
	}

}

/**
 *	añade un nuevo evento a la cola de eventos.
 */
void rt_FIFO_encolar(uint32_t ID_evento, uint32_t auxData) {
		SC_entrar_disable_irq();  // Entrar en región crítica (deshabilitar interrupciones)
    if (num_eventos == COLA_SIZE) {
        // Overflow: detener la ejecución en un bucle infinito
				//se avisa del overflow
				drv_monitor_marcar(monitor_overflow);
        while(1);
    }
    // Obtener la marca de tiempo 
    uint32_t TS = drv_tiempo_actual_us(); 

    // Insertar el evento en la cola
    cola[head].ID_EVENTO = (EVENTO_T)ID_evento;
    cola[head].auxData = auxData;
    cola[head].TS = TS;
		rt_FIFO_conteo(ID_evento);

    // Avanzar el índice circular
    head = (head + 1) % COLA_SIZE;
    num_eventos++;
		
		SC_salir_enable_irq();  // Salir de región crítica (habilitar interrupciones)	
}

/**
 *	elimina un evento de la cola de eventos.
 *	devuelve el # de eventos sin procesar (0 si no hay eventos sin procesar)
*/
uint8_t rt_FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData, uint32_t *TS) {
		
		SC_entrar_disable_irq();  // Entrar en región crítica (deshabilitar interrupciones)

    if (num_eventos == 0) {
				SC_salir_enable_irq();  // Salir de región crítica (habilitar interrupciones)
        return 0; // No hay eventos pendientes
    }

    // Extraer el evento de la cola
    *ID_evento = cola[tail].ID_EVENTO;
    *auxData = cola[tail].auxData;
    *TS = cola[tail].TS;

    // Marcar el evento como procesado avanzando el índice
    tail = (tail + 1) % COLA_SIZE;
    num_eventos--;
		
		hal_WDT_feed();  // Recarga el WDT para evitar el reinicio en operación normal
		SC_salir_enable_irq();  // Salir de región crítica (habilitar interrupciones)
    return num_eventos + 1;
}


/**
 *	dado un evento, devuelve el número de veces que ha sido encolado.
*/
uint32_t rt_FIFO_estadisticas(EVENTO_T ID_evento) {
	
	for(int i = 0;i<EVENT_TYPES;i++){
		if(ID_evento == (EVENTO_T)i){
			return conteo_eventos[i];
			
		}
	}
	return (uint32_t)0;
}

/**
 * devuelve el número total de elementos encolados
 */
uint32_t rt_FIFO_eventos_encolados(){
	
	return conteo_eventos[0];
}

/**
 * devuelve el número de eventos que quedan por procesar.		
 */
uint32_t rt_FIFO_num_eventos_sinProcesar() {
	return num_eventos;
}

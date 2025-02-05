/* *****************************************************************************
 * P.H.2024: módulo de implementación del gestor de eventos
 */

#include "rt_GE.h"
#include "rt_fifo.h"
#include "rt_evento_t.h"
#include "drv_consumo.h"
#include <stdbool.h>
#include "svc_alarma.h"
#include "drv_monitor.h"
#include <stdint.h>
#include "hal_WDT.h"
#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
	#include "drv_tiempo.h"
	static Tiempo_us_t t, t_min, t_max; // para saber cuanto se tarda en procesar un evento
#endif	



// Máximo de tareas suscritas a cada evento
#define rt_GE_MAX_SUSCRITOS 4
#define TIMEOUT_INACTIVIDAD 20*1000

// Estructura de suscripción
typedef struct {
    EVENTO_T ID_evento;
    CallbackFunc Tareas [rt_GE_MAX_SUSCRITOS];	// Vector de funciones que tiene que hacer cada evento 
		int numTareas;	// Índice que muestra el número de tareas para el evento
} Suscripcion;


static Suscripcion suscripciones[EVENT_TYPES];
static uint8_t num_suscripciones; // Indice del vector de suscripciones
static uint32_t monitor_overflow;



/**
 * Inicializa el gestor de eventos 
 */
void rt_GE_iniciar(uint32_t monitor) {

	monitor_overflow = monitor;
	for(int i = 0; i < EVENT_TYPES; i++){
		suscripciones[i].ID_evento = (EVENTO_T)i;
	} 
	num_suscripciones = EVENT_TYPES;
	
	svc_GE_suscribir(ev_PULSAR_BOTON,rt_GE_tratar); // suscribir la tarea tratar del ge al evento de pulsar boton
}

/**
 * inicializa las suscripciones a eventos, activa una alarma periódica
 * y entra en un bucle infinito que extrae eventos de la cola. Cuando se extraen eventos,
 * despacha estos a las tareas suscritas correspondientes. Si no hay eventos,
 * el sistema entra en un estado de bajo consumo energético.
 *
 * Además, se alimenta al watchdog con cada llamada a "rt_FIFO_extraer". Se le alimenta en la propia función de extraer.
 *
 */
void rt_GE_lanzador(void) {
			
	EVENTO_T ID_evento;
	uint32_t auxData;
	uint32_t TS;
	svc_GE_suscribir((uint32_t)ev_INACTIVIDAD, rt_GE_tratar); // suscribir la tarea tratar del ge al evento de inactividad
	svc_alarma_activar(svc_alarma_codificar(0,TIMEOUT_INACTIVIDAD),ev_INACTIVIDAD,0); // activa alarma ev_INACTIVIDAD 
	#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
		t_min = 0x7FFFFFFF;
		t_max = 0;
	#endif
	while (1) {
		// Extrae evento de la cola y despacha a las tareas suscritas
		#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
				t = drv_tiempo_actual_us();
		#endif
		if (rt_FIFO_extraer(&ID_evento, &auxData, &TS)) {
			#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
				t = drv_tiempo_actual_us() - t; // tiempo hasta que se procesa el evento
				if(t < t_min){
					t_min = t;
				}
				if(t > t_max) {
					t_max = t;
				}
			#endif
			for (int i = 0; i < num_suscripciones; i++) {
					if (suscripciones[i].ID_evento == ID_evento) {
						// enviar el evento y el campo auxiliar a todas las tareas suscritas a ese evento.
						for(int j=0; j < suscripciones[i].numTareas; j++){
							
							suscripciones[i].Tareas[j](ID_evento,auxData);
						}
							break;
				 }
			}
		}else {
				// Si no hay eventos, poner en espera para ahorrar energía
				drv_consumo_esperar();
		}
}
}

/**
 * Suscribe la tarea "f_callback" al evento "evento"
 */
void svc_GE_suscribir(uint32_t evento, CallbackFunc f_callback) {
		
		for(int i = 0; i <num_suscripciones; i++){
			if( suscripciones[i].ID_evento == (EVENTO_T)evento){
				
					if (suscripciones[i].numTareas >= rt_GE_MAX_SUSCRITOS) {
						// Manejo de overflow, bucle infinito
						drv_monitor_marcar(monitor_overflow); // Enciende el monitor
						while (1);
						
					}else{
						// suscribir nueva tarea al evento
						suscripciones[i].Tareas[suscripciones[i].numTareas] = f_callback;
						suscripciones[i].numTareas++;
						
						break;
					}
			}
		} 
}

/**
 * cancela la tarea "f_callback" del evento "evento"
 */
void svc_GE_cancelar( uint32_t evento, CallbackFunc f_callback) {
	
		int encontrada = 0;
    for (int i = 0; i < num_suscripciones; i++) {
			  // Busca el evento al que quiere cancelarle una tarea
        if (suscripciones[i].ID_evento == (EVENTO_T)evento) {
					// Busca la tarea a desuscribir
					for (int j = 0; j < suscripciones[i].numTareas; j++) {
                if(suscripciones[i].Tareas[j] == f_callback){
									encontrada = 1;								// Ha encontrado la tarea a cancelar
									suscripciones[i].numTareas--;	// Resta la tarea cancelada
								}
								if(encontrada){
									if(j != suscripciones[i].numTareas ){
												suscripciones[i].Tareas[j] = suscripciones[i].Tareas[j+1]; // Compacta el vector de tareas
									}
								}
            }
            break;
        }
    }
}

/**
 * gestiona el tratamiento de eventos según el tipo
 */
void rt_GE_tratar(uint32_t evento, uint32_t auxiliar) {
    
		switch ((EVENTO_T)evento) {
    case ev_PULSAR_BOTON:
				// Reprogramar la alarma de inactividad
				// funcion para activar botones
				svc_alarma_activar(svc_alarma_codificar(0,20*1000),ev_INACTIVIDAD,0);
		
        break;
    case ev_INACTIVIDAD:
				// Activar modo de suspensión profunda o bajo consumo
         drv_consumo_dormir();
        break;
    default:
        // Código para cualquier otro caso
        break;
	}     
}

/*
 * devuelve el número de tareas suscritas al evento "evento".
 */
uint32_t rt_GE_estadisticas(uint32_t evento){
	uint32_t num_suscritas = 0;
	for (int i = 0; i < num_suscripciones; i++) {
		if (suscripciones[i].ID_evento == (EVENTO_T) evento) {
			num_suscritas = suscripciones[i].numTareas;
		}
	}
	return num_suscritas;
}


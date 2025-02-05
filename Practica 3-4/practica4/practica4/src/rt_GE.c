#include "rt_GE.h"
#include "rt_fifo.h"
#include "rt_evento_t.h"
#include "drv_tiempo.h"
#include "drv_consumo.h"
#include <stdbool.h>
#include "svc_alarma.h"

static const uint8_t evs_usuario[ev_NUM_EV_USUARIO] = ev_USUARIO;
static Suscripcion suscripciones[rt_GE_MAX_SUSCRITOS];
static uint8_t num_suscripciones = 0;
static uint32_t monitor_overflow;

// Inicializa las estructuras y alarma de inactividad
void rt_GE_iniciar(uint32_t monitor) {
    //Inicializacion
		monitor_overflow = monitor;
		num_suscripciones = 0;
	
    rt_FIFO_inicializar(monitor);
}

// Revisa la cola de eventos y los despacha
void rt_GE_lanzador(void) {
			
	EVENTO_T ID_evento;
	uint32_t auxData;
	uint32_t TS;
	svc_alarma_activar(20*1000, ev_INACTIVIDAD, 0); // activa alarma ev_INACTIVIDAD 
	
	while (1) {
        // Extrae evento de la cola y despacha a las tareas suscritas
        if (rt_FIFO_extraer(&ID_evento, &auxData, &TS)) {
            for (int i = 0; i < num_suscripciones; i++) {
                if (suscripciones[i].ID_evento == ID_evento) {
                    suscripciones[i].callback(ID_evento,auxData);
                }
            }
        } 
        else {
            // Si no hay eventos, poner en espera para ahorrar energ?a
            drv_consumo_esperar();
        }
    }
}

// Suscribe una tarea a un evento (maneja overflow)
void svc_GE_suscribir(EVENTO_T evento, CallbackFunc f_callback) {
    if (num_suscripciones >= rt_GE_MAX_SUSCRITOS) {
        // Manejo de overflow, bucle infinito
        while (1);
    }
    suscripciones[num_suscripciones].ID_evento = evento;
    suscripciones[num_suscripciones].callback = f_callback;
    num_suscripciones++;
}

// Cancela una suscripci?n a un evento y compacta
void svc_GE_cancelar(EVENTO_T evento, CallbackFunc f_callback) {
	
    for (int i = 0; i < num_suscripciones; i++) {
        if (suscripciones[i].ID_evento == evento && suscripciones[i].callback == f_callback) {
            for (int j = i; j < num_suscripciones - 1; j++) {
                suscripciones[j] = suscripciones[j + 1];
            }
            num_suscripciones--;
            break;
        }
    }
}

// Gestiona el tratamiento de eventos seg?n el tipo
void rt_GE_tratar(EVENTO_T evento, uint32_t auxiliar) {
    
		switch (evento) {
    case ev_PULSAR_BOTON:
				// Reprogramar la alarma de inactividad
				// funcion para activar botones
				svc_alarma_activar(20*1000,ev_INACTIVIDAD,0);
		
        break;
    case ev_INACTIVIDAD:
				// Activar modo de suspensi?n profunda o bajo consumo
        // Implementar suspensi?n o reinicio seg?n el sistema
         drv_consumo_dormir();
        break;
    default:
        // C?digo para cualquier otro caso
        break;
	}
 
     
      
}

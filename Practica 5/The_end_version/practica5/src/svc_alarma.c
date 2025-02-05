/* *****************************************************************************
 * P.H.2024: módulo de implementación de las alarmas
 */

#include  <stdint.h>
#include "rt_evento_t.h"
#include "drv_tiempo.h"
#include "svc_alarma.h"
#include "drv_monitor.h"
#include "rt_GE.h"

#define svc_ALARMAS_MAX 4 // Número máximo de alarmas simultáneas
#define PERIODO 50	//Perido de interrupcion del timer en ms

typedef struct{
	
	int activa;					 // Indica si está activa
	uint32_t retardo_ms; // Indica cada cuantos ms es un periodo
	int periodica;			 // Indica si es o no periodica
	EVENTO_T ID_evento;	
	uint32_t auxData;
	uint32_t periodo;		// Indica el momento(ms) en tiempo de ejecucion cuando acaba el temporizador 

}Alarma;

static uint32_t monitor_overflow;										// monitor para avisar del overflow
static void (*funcion_callback)(uint32_t,uint32_t); // función de callback
static Alarma alarmas[svc_ALARMAS_MAX ];						// vector de alarmas
static int num_alarmas;															// índice del número actual de alarmas activas en el vector


/*************************************************************************************************************
 * inicializa el servicio de alarmas
 */
void svc_alarma_iniciar(uint32_t overflow, void(*f_callback)(uint32_t, uint32_t), uint32_t ID_evento){
		// Inicialización 
		monitor_overflow = overflow;	
		funcion_callback = f_callback;
		num_alarmas = 0;	
		for (uint8_t i = 0; i < svc_ALARMAS_MAX; i++) {
			alarmas[i].activa = 0;         	// Desactiva la alarma
			alarmas[i].retardo_ms = 0;      // Resetea el retardo
			alarmas[i].periodica = 0;       // Resetea el valor de periodica
			alarmas[i].ID_evento = ev_VOID; // Limpia el ID del evento
			alarmas[i].auxData = 0;         // Limpia los datos auxiliares
			alarmas[i].periodo = 0;
		}
	
		svc_GE_suscribir(ID_evento, svc_alarma_tratar);	// Suscribir evento
		drv_tiempo_periodico_ms(PERIODO,f_callback,ID_evento);	// Iniciar temporizador periodico

}

/**
 * rellena la alarma dada por "index" con los datos que recibe como parámetros
 */
void rellenarAlarma(uint32_t retardo_ms, uint32_t ID_evento, uint32_t auxData,int index,uint32_t bit31){

		alarmas[index].ID_evento = (EVENTO_T)ID_evento;
		alarmas[index].activa = 1;
		alarmas[index].auxData = auxData;
		alarmas[index].periodo = retardo_ms + drv_tiempo_actual_ms(); // Se guarda el tiempo en el que tendrá que hacer la interrupción 
		
		if(bit31){
			alarmas[index].periodica = 1;
			alarmas[index].retardo_ms = retardo_ms;// Si es periodica se guarda cada cuanto hace la interrupción
			
		}else{
			alarmas[index].periodica = 0;
		}
}

/** 
 * devuelve una alarma codificada, donde el bit de mayor peso
 * indica si es periódica o esporádica
 */
uint32_t svc_alarma_codificar(uint32_t periodica, uint32_t retardo){
	
	return (periodica << 31) | retardo;
}

/**
 * programa una alarma para el evento "evento" que saltará en "retardo_ms" ms
 */
void svc_alarma_activar(uint32_t retardo_ms, uint32_t ID_evento, uint32_t auxData){

	if(retardo_ms != 0){
		uint32_t bit31 = retardo_ms >> 31;
		uint32_t retardoSinBit31 = retardo_ms & ~(1u << 31);
		
	
			//comprobar si hay alguna alarma con el evento id
				for(int i = 0; i < svc_ALARMAS_MAX; i++){
					if(alarmas[i].ID_evento == (EVENTO_T)ID_evento && alarmas[i].activa == 1){
						// si la hay reescribirla
						rellenarAlarma(retardoSinBit31, ID_evento, auxData, i, bit31);
						return;
					}
				}
			
			//si hay menos de max alarmas, hacer una nueva
				if(num_alarmas < svc_ALARMAS_MAX){
						for(int i = 0; i < svc_ALARMAS_MAX; i++){
							
							if(alarmas[i].activa == 0){ // busca cuál está desactivada y la reescribe
								rellenarAlarma(retardoSinBit31, ID_evento,  auxData,i, bit31);
								num_alarmas++;
								return;
							}
						}
							
				}else{
					// se avisa del overflow
					drv_monitor_marcar(monitor_overflow);
					while(1);	
				}
				
	}else{
			// se cancela la alarma
			for(int i = 0; i < num_alarmas; i++){
					if(alarmas[i].ID_evento == (EVENTO_T)ID_evento){ // si existe la alarma
						
						alarmas[i].activa = 0;	// se desactiva
						num_alarmas--;
						break;	
					}
			}
	}
}

/**
 * gestiona el tratamiento de las alarmas según el tipo de evento
 */
void svc_alarma_tratar(uint32_t evento,  uint32_t aux){
	
	switch ((EVENTO_T)evento) {
    case ev_T_PERIODICO:
			
        for(int i = 0; i < svc_ALARMAS_MAX; i++){
					if(alarmas[i].periodo <= drv_tiempo_actual_ms() && alarmas[i].activa){
						
						funcion_callback(alarmas[i].ID_evento, alarmas[i].auxData); // Encola el evento
						
						if(alarmas[i].periodica == 0){ // GESTIÓN DE PERIODICIDAD
							// si no es periodica se cancela
							alarmas[i].activa = 0;
							num_alarmas --;
							
						}else{ // Si es perodica se actualza el contador
							alarmas[i].periodo = alarmas[i].retardo_ms + drv_tiempo_actual_ms();
						}
					}	
				}
        break;
		
    default:
        // Código para cualquier otro caso
        break;
	}
}

/**
 * devuelve el número de alarmas activas.
 */
uint32_t svc_alarmas_estadisticas(){
	return num_alarmas;
}

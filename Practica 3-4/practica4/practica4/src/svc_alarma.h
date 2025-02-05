#include <stdint.h>

#ifndef SVC_ALARMA
#define SVC_ALARMA

#include  <stdint.h>
#include "rt_evento_t.h"

#define svc_ALARMAS_MAX 4

typedef struct{
	
	int activa;					 //Indica si est? activa
	uint32_t retardo_ms; // Indica cada cuantos ms es un periodo
	int periodica;			 // Indica si es o no periodica
	EVENTO_T ID_evento;	
	uint32_t auxData;
	uint32_t periodo;		// Indica el momento(ms) en tiempo de ejecucion cuando acaba el temporizador 

}Alarma;

void svc_alarma_activar(uint32_t retardo_ms, EVENTO_T ID_evento, uint32_t auxData);

void svc_alarma_iniciar(uint32_t overflow, void*(f_callback), uint32_t ID_evento);

void svc_alarma_tratar(EVENTO_T evento,  uint32_t aux);



#endif
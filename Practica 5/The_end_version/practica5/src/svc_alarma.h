/* *****************************************************************************
 * P.H.2024: módulo de declaración de las alarmas
 */

#ifndef SVC_ALARMA
#define SVC_ALARMA

#include  <stdint.h>


/**
 * inicializa el servicio de alarmas
 */
void svc_alarma_iniciar(uint32_t overflow, void(*f_callback)(uint32_t, uint32_t), uint32_t ID_evento);

/**
 * programa una alarma para el evento "evento" que saltará en "retardo_ms" ms
 */
void svc_alarma_activar(uint32_t retardo_ms, uint32_t ID_evento, uint32_t auxData);

/**
 * gestiona el tratamiento de las alarmas según el tipo de evento
 */
void svc_alarma_tratar(uint32_t evento,  uint32_t aux);

/** 
 * devuelve una alarma codificada, donde el bit de mayor peso
 * indica si es periódica o esporádica
 */
uint32_t svc_alarma_codificar(uint32_t periodica, uint32_t retardo);

/**
 * rellena la alarma dada por "index" con los datos que recibe como parámetros
 */
void rellenarAlarma(uint32_t retardo_ms, uint32_t ID_evento, uint32_t auxData, int index, uint32_t bit31);

/**
 * devuelve el número de alarmas activas.
 */
uint32_t svc_alarmas_estadisticas(void);

#endif

/* *****************************************************************************
 * P.H.2024: módulo de declaración del gestor de eventos
 */

#ifndef RT_GE_H
#define RT_GE_H

#include <stdint.h>

typedef void (*CallbackFunc)();
/**
 * Inicializa el gestor de eventos 
 */
void rt_GE_iniciar(uint32_t monitor);

/**
 * inicializa las suscripciones a eventos, activa una alarma periódica
 * y entra en un bucle infinito que extrae eventos de la cola. Cuando se extraen eventos,
 * despacha estos a las tareas suscritas correspondientes. Si no hay eventos,
 * el sistema entra en un estado de bajo consumo energético.
 */
void rt_GE_lanzador(void);

/**
 * Suscribe la tarea "f_callback" al evento "evento"
 */
void svc_GE_suscribir(uint32_t evento, CallbackFunc f_callback);

/**
 * cancela la tarea "f_callback" del evento "evento"
 */
void svc_GE_cancelar(uint32_t evento, CallbackFunc f_callback);

/**
 * gestiona el tratamiento de eventos según el tipo
 */
void rt_GE_tratar(uint32_t evento, uint32_t auxiliar);

/*
 * devuelve el número de tareas suscritas al evento "evento".
 */
uint32_t rt_GE_estadisticas(uint32_t evento);

#endif 

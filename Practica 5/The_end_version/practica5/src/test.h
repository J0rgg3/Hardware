/* *****************************************************************************
 * P.H.2024: módulo de declaración para los tests 
 */

#ifndef TEST
#define TEST

#include <stdint.h>
#include "rt_evento_t.h"

/**
 * enciende todos los leds
 */
void leds_encenderTodos(uint32_t id, uint32_t ms);

/**
 * apaga todos los leds
 */
void leds_apagarTodos(uint32_t id, uint32_t ms);

/**
 * llama a conmutar el led "id"
 */
void leds_c(uint32_t id, uint32_t ms);

/**
 * simula una prueba sencilla para el gestor de eventos y la alarma.
 * Se basa en suscribir una tarea al evento periodico y otra al evento void(usamos este para evitar las funciones "tratar")
 * y se lanza una alarma periodica del void, cuando el evento void hace sus tareas, se le quita la de "encenderTodos" y se le pone "apagarTodos"
 * y asi indefinidamente
 */
void TestAlarmaGestorEvento(void);
	
/**
 * comprueba el funcionamiento de los botones junto al gestor y alarmas.
 * Se basa en lo mismo que la anterior solo que ahora el evento void es un evento pulsar boton
 */
void TestBotones(void);

/**
 * prueba si se ha configurado correctamente la gestión de overflow de las alarmas
 */
void TestOverflowAlarmas(void);

/**
 * prueba si se ha configurado correctamente la gestión de overflow del gestor de eventos
 */
void TestOverflowGE(void);

/**
 * prueba si el watchdog resetea el sistema y se le alimenta correctamente
 */
void TestWatchdog(void);

#endif

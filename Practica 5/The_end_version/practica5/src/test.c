/* *****************************************************************************
 * P.H.2024: módulo de implementación para los tests 
 */

#include "test.h"
#include "drv_leds.h"
#include "rt_GE.h"
#include "hal_WDT.h"
#include "board.h"
#include "svc_alarma.h"


void leds_encenderTodos(uint32_t id, uint32_t ms);

/**
 * apaga todos los leds
 */
void leds_apagarTodos(uint32_t id, uint32_t ms){
	svc_GE_suscribir((EVENTO_T) id,leds_encenderTodos);
	drv_led_apagar(0);
	svc_GE_cancelar((EVENTO_T)id,leds_apagarTodos);
}

/**
 * enciende todos los leds
 */
void leds_encenderTodos(uint32_t id, uint32_t ms){
	svc_GE_suscribir((EVENTO_T)id,leds_apagarTodos);
	drv_led_encender(0);
	svc_GE_cancelar((EVENTO_T)id,leds_encenderTodos);
			
}

/**
 * llama a conmutar el led "id"
 */
void leds_c(uint32_t id, uint32_t ms){

	drv_led_conmutar(id);
}

/**
 * simula una prueba sencilla para el gestor de eventos y la alarma.
 * Se basa en suscribir una tarea al evento periodico y otra al evento void(usamos este para evitar las funciones "tratar")
 * y se lanza una alarma periodica del void, cuando el evento void hace sus tareas, se le quita la de "encenderTodos" y se le pone "apagarTodos"
 * y asi indefinidamente
 */
void TestAlarmaGestorEvento(){
	
	//suscribir tareas
	svc_GE_suscribir(ev_T_PERIODICO,leds_c);
	svc_GE_suscribir(ev_VOID,leds_encenderTodos);
	svc_alarma_activar(svc_alarma_codificar(1,1000),ev_VOID,0);
	rt_GE_lanzador();

}

/**
 * comprueba el funcionamiento de los botones junto al gestor y alarmas.
 * Se basa en lo mismo que la anterior solo que ahora el evento void es un evento pulsar boton
 */
void TestBotones(){

	svc_GE_suscribir(ev_T_PERIODICO,leds_c);
	svc_GE_suscribir(ev_PULSAR_BOTON,leds_encenderTodos);
	//svc_alarma_activar(svc_alarma_codificar(1,1000),ev_PULSAR_BOTON,0);
	rt_GE_lanzador();
	
}

/**
 * prueba si se ha configurado correctamente la gestión de overflow de las alarmas
 */
void TestOverflowAlarmas(){
	
	svc_alarma_activar(svc_alarma_codificar(0,1000),ev_VOID,1); 
	svc_alarma_activar(svc_alarma_codificar(0,1000),ev_LED,1); 
	svc_alarma_activar(svc_alarma_codificar(0,1000),ev_BOTON_RETARDO,1); 
	svc_alarma_activar(svc_alarma_codificar(0,1000),ev_INACTIVIDAD,1); 
	svc_alarma_activar(svc_alarma_codificar(0,1000),ev_PULSAR_BOTON,1); 

}

/**
 * prueba si se ha configurado correctamente la gestión de overflow del gestor de eventos
 */
void TestOverflowGE(){
	svc_GE_suscribir(ev_VOID,leds_c);
	svc_GE_suscribir(ev_VOID,leds_c);
	svc_GE_suscribir(ev_VOID,leds_c);
	svc_GE_suscribir(ev_VOID,leds_c);
	svc_GE_suscribir(ev_VOID,leds_c);
}

/**
 * prueba si el watchdog resetea el sistema y se le alimenta correctamente
 */
void TestWatchdog(){
	hal_WDT_iniciar(5);
	svc_GE_suscribir(ev_T_PERIODICO,leds_encenderTodos);
 // while(1);
	rt_GE_lanzador();
	
}

/* *****************************************************************************
  * P.H.2024: Driver/Manejador de los Leds
	*
	* blink practica 2 de proyecto hardware 2024
 */
 
#include <stdint.h>
#include <stdbool.h>

#include "hal_gpio.h"
#include "drv_leds.h"
#include "drv_tiempo.h"
#include "drv_consumo.h"
#include "drv_monitor.h"
#include "rt_evento_t.h"
#include "rt_fifo.h"

#define RETARDO_MS 500 		//retardo blink en milisegundos

/* *****************************************************************************
 * BLINK, parpadeo de un led conmutando on/off 
 * retardo por bucle de instrucciones, solo usa el manejador del led
 * para realizar la primera sesi�n de la practica
 */
void blink_v1(uint32_t id){
  while (1) {
    uint32_t volatile tmo;
    
    tmo = 10000000;
    while (tmo--);
    drv_led_conmutar(id);     
	}		
}

/* *****************************************************************************
 * BLINK, parpadeo de un led conmutando on/off 
 * activacion por tiempo, usa tanto manejador del led como el del tiempo
 * para realizar en la segunda sesi�n de la practica, version a entregar
 */
void  blink_v2(uint32_t id){
	Tiempo_ms_t siguiente_activacion;	
	
	drv_led_encender(id);

	siguiente_activacion = drv_tiempo_actual_ms();
	
	/* Toggle LEDs. */
	while (true) {
		siguiente_activacion += RETARDO_MS; //ms
		drv_tiempo_esperar_hasta_ms(siguiente_activacion);
		drv_led_conmutar(id);
		//otras cosas
	}
}


//------------Blinkv3-------------------
/*
 * Función de callback que llama a la función led_conmutar. 
*/
void leds_c(uint32_t id, uint32_t ms){

	drv_led_conmutar(id);
}

/* *****************************************************************************
 * BLINK, parpadeo de un led conmutando on/off 
 * activación periódica, CPU a la espera (dormida) y poca actividad,
 * usa tanto manejador del led como el del tiempo
 */
void  blink_v3(uint32_t id){
	//drv_consumo_iniciar();
	
	drv_led_encender(id);
	drv_tiempo_periodico_ms(RETARDO_MS,leds_c,id);
	
	while(1) {
		 drv_consumo_esperar();

	}
}

//------------Blinkv4-------------------

/*
 * Función de callback que llama a la función rt_FIFO_encolar. 
*/
void encolar(uint32_t id, uint32_t ms){
	
	rt_FIFO_encolar(id,ms);
}


/* *****************************************************************************
 * BLINK, parpadeo de un led conmutando on/off 
 * activación por runtime, CPU a la espera (dormida) y poca actividad,
 * usa tanto manejador del led como el del tiempo
 */
void blink_v4(uint32_t id) {
	
	EVENTO_T EV_ID_evento;
	uint32_t EV_auxData,EV_TS;
	
	rt_FIFO_inicializar(id);
	drv_led_encender(id);
	drv_tiempo_periodico_ms(RETARDO_MS, rt_FIFO_encolar, ev_T_PERIODICO);
	
	while (1) {
		if (rt_FIFO_extraer(&EV_ID_evento, & EV_auxData, & EV_TS)) {
			if (ev_T_PERIODICO) {
				drv_led_conmutar(id); 
			}
		}
		else {
			drv_consumo_esperar(); 
		}
}
}

/* *****************************************************************************
 * MAIN, Programa principal.
 * para la primera sesion se debe usar la funcion de blink_v1 sin temporizadores
 * para la entrega final se debe incocar a blink_v2
 */
int main(void){
	

	uint32_t Num_Leds, Num_Monitors;

	/* Init tiempo, es un reloj que indica el tiempo desde que comenzo la ejecuci�n */
	drv_tiempo_iniciar(); // para la sesion 2 de practica 2, es necesario iniciarlo para blinkv3, para valor HAL_TICKS2US
	
	hal_gpio_iniciar();	// llamamos a iniciar gpio antesde que lo hagan los drivers
	
	/* Configure LED*/
	Num_Leds = drv_leds_iniciar();
	Num_Monitors = drv_monitor_iniciar();
	if (Num_Leds > 0){
//		drv_led_encender(1);
//		blink_v1(1);			
//		blink_v2(2);			
//		blink_v3(3);
		if (Num_Monitors > 0) {
			blink_v4(4);
		}
	}
	
	
}

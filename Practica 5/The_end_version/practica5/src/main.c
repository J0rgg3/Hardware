/* *****************************************************************************
 * P.H.2024: módulo programa principal 
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
#include "hal_ext_int.h"
#include "rt_GE.h"
#include "svc_alarma.h"
#include "drv_botones.h"
#include "hal_WDT.h"
#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
	#include "test.h"
#endif
#include "simon.h"


/*Variables estáticas*/
static uint32_t num_leds;			//	Número de Leds del hardware
static uint32_t num_botones;	//	Número de botones del hardware

/**
 * Apaga todos los leds 
 */
void leds_dormir(uint32_t id, uint32_t ms){
	
	drv_led_apagar(0);
}


/* *****************************************************************************
 * MAIN, Programa principal.
 */
 int main(void){
	
//INICIALIZACIÓN HARDWARE
	/* Init tiempo, es un reloj que indica el tiempo desde que comenzó la ejecución */
	drv_tiempo_iniciar();
	
	hal_gpio_iniciar();	// llamamos a iniciar gpio antesde que lo hagan los drivers
	 
	drv_consumo_iniciar(3,4);
	rt_FIFO_inicializar(5);
	rt_GE_iniciar(1);
	svc_GE_suscribir(ev_INACTIVIDAD,leds_dormir);
	svc_alarma_iniciar(2,rt_FIFO_encolar, ev_T_PERIODICO);
	
	/* Configure LED*/
		num_leds = drv_leds_iniciar();
	/* Configure Monitor*/
		drv_monitor_iniciar();
	/* Configure Botones*/
		num_botones = drv_botones_iniciar(rt_FIFO_encolar,ev_PULSAR_BOTON,ev_BOTON_RETARDO,ev_PULSAR_DOS_BOTONES);
	 
	 #if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
			#if 0
				TestAlarmaGestorEvento();
			#endif
			
			#if 0
				TestBotones();
			#endif
			
			#if 0
				TestOverflowAlarmas();
			#endif
			
			#if 0
				TestOverflowGE();
			#endif
			
			#if 0
				TestWatchdog();
			#endif
		#endif
			
	#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056) || defined (LPC2105_simulador) || defined (BOARD_PCA10056)
		#if 1
			simon_iniciar(num_leds,num_botones);
		#endif
	
	#endif
}

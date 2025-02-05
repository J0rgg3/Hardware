/* *****************************************************************************
 * P.H.2024: Temporizadores en nRF52840, Timer 0 y Timer 1
 * implementacion para cumplir el hal_tiempo.h
 * Timer0 cumple maxima frecuencia, minimas interrupciones contando en ticks
 * Timer1 avisa cada periodo de activacion en ticks
 */

#include <nrf.h>        // Definiciones de nRF52840
#include <stdint.h>

#define MAX_COUNTER_VALUE  0XFFFFFFFE  // 0xFFFFFFFE         // Máximo valor del contador de 32 bits
#define HAL_TICKS2US      16           // PCLK de 16 MHz (para el nRF52840)
#define US2MS							1000						//milisegundos por microsogundos
/* *****************************************************************************
 * Timer0 contador de ticks
 */
static volatile uint32_t timer0_int_count = 0;  // Contador de 32 bits de veces que ha saltado la RSI Timer0

/* *****************************************************************************
 * Timer 0 Interrupt Service Routine
 */
void TIMER0_IRQHandler(void) {
	 volatile uint32_t dummy;
	if (NRF_TIMER0->EVENTS_COMPARE[0] == 1)
  {
			NRF_TIMER0->EVENTS_COMPARE[0] = 0;  // Limpiar el flag de interrupción
			dummy = NRF_TIMER0->EVENTS_COMPARE[0];			//leerlo para asegurar que se ha borrado
			dummy;
			timer0_int_count++;
	}
	
	
}

/* *****************************************************************************
 * Programa un contador de tick sobre Timer0, con máxima precisión y mínimas interrupciones
 */
uint32_t hal_tiempo_iniciar_tick() {
    timer0_int_count = 0;
	
		
		NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos;
	
		NRF_TIMER0->PRESCALER = 0 << TIMER_PRESCALER_PRESCALER_Pos; // Ponerlo en su posicion 
		
		NRF_TIMER0->CC[0] = MAX_COUNTER_VALUE;									   // MAX_COUNTER_VALUE
	
		NRF_TIMER0->INTENSET = TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos;  	 // Habilitar interrupción cuando coincida con CC[0]
		
	
		NVIC_EnableIRQ(TIMER0_IRQn);											// Enable Timer0 Interrupt

   	NRF_TIMER0->TASKS_START = 1;										

    return HAL_TICKS2US;                          // Devuelve el factor de conversión de ticks a microsegundos
}

/* *****************************************************************************
 * Lee el tiempo que lleva contando el contador y lo devuelve en ticks
 */
uint64_t hal_tiempo_actual_tick() { 
    uint64_t time;
		NRF_TIMER0->TASKS_CAPTURE[1] = 1;			//captura el valor de l timer en cc[1]
		 
		time = (( MAX_COUNTER_VALUE) * (uint64_t)timer0_int_count) + (uint64_t)NRF_TIMER0->CC[1]; 
    return time;
}


/* *****************************************************************************
 * Activacion periodica con timer 1
 */
static void(*f_callback)();		//puntero a funcion a llamar cuando salte la RSI (en modo irq)

/* *****************************************************************************
 * Timer 1 Interrupt Service Routine
 * llama a la funcion de callbak que se ejecutara en modo irq
 */
void  TIMER1_IRQHandler(void) __irq {
	 volatile uint32_t dummy;
	
	if (NRF_TIMER1->EVENTS_COMPARE[0] == 1)
  {
			f_callback();																// llama a función de callback
			NRF_TIMER1->EVENTS_COMPARE[0] = 0;  				// Limpiar el flag de interrupción
			dummy = NRF_TIMER1->EVENTS_COMPARE[0];			//leerlo para asegurar que se ha borrado
			dummy;
		
	}
	
	
}

/* *****************************************************************************
 * Dependiente del hardware Timer1
 * Programa el reloj para que llame a la función de callback cada periodo.
 * El periodo se indica en tick. Si el periodo es cero se para el temporizador. 
 */
void hal_tiempo_reloj_periodico_tick(uint32_t periodo_en_tick, void(*funcion_callback)()){
  
	f_callback = funcion_callback;
	
	if (periodo_en_tick != 0) { // Si el periodo es cero solo se para el temporizador.
		
		NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos;
		
		NRF_TIMER1->CC[0] = periodo_en_tick  ; 		// 16  Ticks (ciclos) por microsegundo.
																																				// (periodo_en_ms * HAL_TICKS2US * US2MS) - 1;  
																																				// resto uno por como incrementa y compara
		NRF_TIMER1->PRESCALER = 0 << TIMER_PRESCALER_PRESCALER_Pos; // Poner el 0 en la posición del Prescaler
		NRF_TIMER1->INTENSET = TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos;  	 // Habilitar interrupción cuando coincida con CC[0]
		NRF_TIMER1->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos; // Reiniciar el temporizador cuando coincida con CC[0]//no monotono
		// Enable Timer1 Interrupt.
		
		NVIC_EnableIRQ(TIMER1_IRQn);	
		
		NRF_TIMER1->TASKS_START = 1;  // Empieza la cuenta
	} else {
		// Detiene el temporizador
		NRF_TIMER1->TASKS_START  = 0;  
		NVIC_DisableIRQ(TIMER1_IRQn);		                   // Disable Timer1 Interrupt		
	}
}





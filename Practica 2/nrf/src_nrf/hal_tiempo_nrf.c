#include <nrf.h>        // Definiciones de nRF52840
#include <stdint.h>

#define MAX_COUNTER_VALUE 0xFFFFFFFE         // Máximo valor del contador de 32 bits
#define counter_0_value	8000000						  //Número de ticks para prescaler = 0	(16Mhz) 0x007A1200
#define counter_1_value 4000000						 //Número de ticks para prescaler = 1	(16Mhz) 0x003D0900
#define counter_2_value 2000000						//Número de ticks para prescaler = 2(16Mhz) 0x001E8480
#define counter_3_value 1000000					 //Número de ticks para prescaler = 3(16Mhz) 0x000F4240
#define counter_4_value 500000					//Número de ticks para prescaler = 4(16Mhz) 0x0007A120
#define HAL_TICKS2US      16           // PCLK de 16 MHz (para el nRF52840)

/* *****************************************************************************
 * Timer0 contador de ticks
 */
static volatile uint32_t timer0_int_count = 0;  // Contador de 32 bits de veces que ha saltado la RSI Timer0

/* *****************************************************************************
 * Timer 0 Interrupt Service Routine
 */
void TIMER0_IRQHandler(void) {
	 if (NRF_TIMER0->EVENTS_COMPARE[0]) {// Al estar ahora activo solo el timer no haria falta comprobar si cc[0] es lo que ha generado la interrupción  
			NRF_TIMER0->EVENTS_COMPARE[0] = 0;  // Limpiar el flag de interrupción
			timer0_int_count++;
	 }
	
}

/* *****************************************************************************
 * Programa un contador de tick sobre Timer0, con máxima precisión y mínimas interrupciones
 */
uint32_t hal_tiempo_iniciar_tick() {
    timer0_int_count = 0;
		NRF_TIMER0->PRESCALER = 0;
		NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer;         			  // Modo temporizador
		NRF_TIMER0->CC[0] = counter_0_value;									   // T0MR0 = MAX_COUNTER_VALUE
	
		NRF_TIMER0->INTENSET = TIMER_INTENSET_COMPARE0_Msk;  	 // Habilitar interrupción en coincidencia con CC[0]
		NRF_TIMER0->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Msk; // Reiniciar el temporizador en coincidencia con CC[0]
	
		
	
		NVIC_EnableIRQ(TIMER0_IRQn);											// Enable Timer0 Interrupt

  	NRF_TIMER0->TASKS_START = 1;										//T0TCR = 1;  // Empieza la cuenta

    return HAL_TICKS2US;                          // Devuelve el factor de conversión de ticks a microsegundos
}

/* *****************************************************************************
 * Lee el tiempo que lleva contando el contador y lo devuelve en ticks
 */
uint64_t hal_tiempo_actual_tick() { 
    uint64_t time;
    time = ((MAX_COUNTER_VALUE + 1) * timer0_int_count) + (uint64_t)NRF_TIMER0->CC[0]; 
    return time;
}



#include <nrf.h>        // Definiciones de nRF52840
#include <stdint.h>

#define MAX_COUNTER_VALUE 0xFFFFFFFE       // M�ximo valor del contador de 32 bits
#define HAL_TICKS2US      16               // PCLK de 16 MHz (para el nRF52840)

/* *****************************************************************************
 * Timer0 contador de ticks
 */
static volatile uint32_t timer0_int_count = 0;  // Contador de 32 bits de veces que ha saltado la RSI Timer0

/* *****************************************************************************
 * Timer 0 Interrupt Service Routine
 */
void TIMER0_IRQHandler(void) {
        timer0_int_count++;
        NRF_TIMER0->EVENTS_COMPARE[0] = 0;  // Limpiar el flag de interrupci�n
}

/* *****************************************************************************
 * Programa un contador de tick sobre Timer0, con m�xima precisi�n y m�nimas interrupciones
 */
uint32_t hal_tiempo_iniciar_tick() {
    timer0_int_count = 0;

    NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer;      // Configura como temporizador
    NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_32Bit;  // Contador de 32 bits
    NRF_TIMER0->PRESCALER = 0;                    // Prescaler = 0 (frecuencia base de 16 MHz)

    NRF_TIMER0->CC[0] = MAX_COUNTER_VALUE;        // M�ximo valor de comparaci�n para generar interrupci�n
    NRF_TIMER0->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Msk; // Limpiar contador en cada comparaci�n
    NRF_TIMER0->INTENSET = TIMER_INTENSET_COMPARE0_Msk;   // Habilitar interrupci�n de comparaci�n 0

    NVIC_EnableIRQ(TIMER0_IRQn);                  // Habilitar interrupciones de TIMER0 en NVIC

    NRF_TIMER0->TASKS_START = 1;                  // Iniciar temporizador

    return HAL_TICKS2US;                          // Devuelve el factor de conversi�n de ticks a microsegundos
}

/* *****************************************************************************
 * Lee el tiempo que lleva contando el contador y lo devuelve en ticks
 */
uint64_t hal_tiempo_actual_tick() { 
    uint64_t time;
    time = ((MAX_COUNTER_VALUE + 1) * timer0_int_count) + (uint64_t)NRF_TIMER0->CC[0]; 
    return time;
}

/* *****************************************************************************
 * Activaci�n peri�dica con Timer1
 */
static void (*f_callback)();   // Puntero a funci�n a llamar cuando salte la RSI (en modo irq)

/* *****************************************************************************
 * Timer 1 Interrupt Service Routine
 * Llama a la funci�n de callback que se ejecutar� en modo irq
 */
void TIMER1_IRQHandler(void) {
    
        f_callback();   // Llamar a la funci�n callback desde la interrupci�n
        NRF_TIMER1->EVENTS_COMPARE[0] = 0;  // Limpiar el flag de interrupci�n
}

/* *****************************************************************************
 * Dependiente del hardware Timer1
 * Programa el reloj para que llame a la funci�n de callback cada periodo.
 * El periodo se indica en ticks. Si el periodo es cero, se para el temporizador.
 */
void hal_tiempo_reloj_periodico_tick(uint32_t periodo_en_tick, void (*funcion_callback)()) {
    f_callback = funcion_callback;

    if (periodo_en_tick != 0) {  // Si el periodo es distinto de 0, configurar el temporizador
        NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;       // Configura como temporizador
        NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_32Bit;  // Contador de 32 bits
        NRF_TIMER1->PRESCALER = 0;                      // Prescaler = 0 (16 MHz)

        NRF_TIMER1->CC[0] = periodo_en_tick - 1;        // Valor de comparaci�n
        NRF_TIMER1->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Msk; // Limpiar contador al alcanzar la comparaci�n
        NRF_TIMER1->INTENSET = TIMER_INTENSET_COMPARE0_Msk;   // Habilitar interrupci�n de comparaci�n 0

        NVIC_EnableIRQ(TIMER1_IRQn);                    // Habilitar interrupciones de TIMER1 en NVIC

        NRF_TIMER1->TASKS_START = 1;                    // Iniciar temporizador
    } else {
        // Detener temporizador
        NRF_TIMER1->TASKS_STOP = 1;                     // Detener el temporizador
        NVIC_DisableIRQ(TIMER1_IRQn);                   // Deshabilitar la interrupci�n de TIMER1
    }
}


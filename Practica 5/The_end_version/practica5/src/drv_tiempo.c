/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los temporizadores
 * Suministra los servicios independientemente del hardware
 *
 * Usa los servicios de hal_tiempo.h
 */


#include "drv_tiempo.h"
#include "hal_tiempo.h"

static uint32_t TICKS2US;
#define US2MS 1000 // para la conversión de microsegundos a milisegundos

/**
 * Inicializa el reloj y empieza a contar
 */
void drv_tiempo_iniciar(void) {
    // Iniciar el temporizador llamando a la función del HAL
   TICKS2US = hal_tiempo_iniciar_tick();


}

/**
 * Tiempo desde que se inició el temporizador en microsegundos
 */
Tiempo_us_t drv_tiempo_actual_us(void) {
    // Obtener el tiempo actual en ticks desde el HAL
    Tiempo_us_t ticks = hal_tiempo_actual_tick();
    // Convertir ticks a microsegundos
    return ticks / TICKS2US;
}

/**
 * Tiempo desde que se inició el temporizador en milisegundos
 */
Tiempo_ms_t drv_tiempo_actual_ms(void) {
    // Obtener el tiempo actual en ticks desde el HAL
    Tiempo_us_t us =  drv_tiempo_actual_us() ;
    // Convertir ticks a milisegundos
    return us / US2MS;
}

/**
 * Retardo: esperar un cierto tiempo en milisegundos
 */
void drv_tiempo_esperar_ms(Tiempo_ms_t ms) {
    // Obtener el tiempo actual
	
    Tiempo_ms_t tiempo_inicial = drv_tiempo_actual_ms();
    // Esperar hasta que se haya transcurrido el tiempo especificado
    while (drv_tiempo_actual_ms() - tiempo_inicial < ms) {
			// Esperar de manera activa hasta que se cumpla el tiempo
    }
		
}

static uint32_t Id;
static void(*f_callback)(uint32_t,uint32_t);	

/**
 * llama a la función apuntada por f_callback con dos parámetros de tipo uint32_t
 */
void funcionQueLlama(void){
	
	uint32_t auxData = drv_tiempo_actual_ms();
	f_callback(Id,auxData);
	
}

/**
 * configura un reloj periódico que ejecuta la función "callback" con "ID_evento" cada "ms" tiempo
 */
void drv_tiempo_periodico_ms(Tiempo_ms_t ms,void(*callback)(),uint32_t ID_evento){

	uint32_t ticks = ms*US2MS*TICKS2US;
	Id = ID_evento;
	f_callback = callback;
	
	hal_tiempo_reloj_periodico_tick(ticks,funcionQueLlama);
}

/**
 * Esperar hasta un determinado tiempo (en ms), devuelve el tiempo actual
 */
Tiempo_ms_t drv_tiempo_esperar_hasta_ms(Tiempo_ms_t ms) {

		while (drv_tiempo_actual_ms() < ms){};
    // Devolver el tiempo actual
     return drv_tiempo_actual_ms();
}


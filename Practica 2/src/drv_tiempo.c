#include "drv_tiempo.h"
#include "hal_tiempo.h"

static uint32_t TICKS2US;
#define US2MS 1000 

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
void drv_tiempo_periodico_ms(Tiempo_ms_t ms,void(*callback)()){

	uint32_t ticks = ms*US2MS*TICKS2US;
	hal_tiempo_reloj_periodico_tick(ticks,callback);


}
/**
 * Esperar hasta un determinado tiempo (en ms), devuelve el tiempo actual
 */
Tiempo_ms_t drv_tiempo_esperar_hasta_ms(Tiempo_ms_t ms) {
    // Obtener el tiempo actual 

		while (drv_tiempo_actual_ms() < ms){};
    // Devolver el tiempo actual
     return drv_tiempo_actual_ms();
}


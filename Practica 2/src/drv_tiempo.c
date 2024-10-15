#include "drv_tiempo.h"
#include "hal_tiempo.h"

#define HAL_TICKS_PER_US  15     // 15 ticks por microsegundo
#define HAL_TICKS_PER_MS (HAL_TICKS_PER_US * 1000) // 15,000 ticks por milisegundo

/**
 * Inicializa el reloj y empieza a contar
 */
void drv_tiempo_iniciar(void) {
    // Iniciar el temporizador llamando a la función del HAL
    hal_tiempo_iniciar_tick();
}

/**
 * Tiempo desde que se inició el temporizador en microsegundos
 */
Tiempo_us_t drv_tiempo_actual_us(void) {
    // Obtener el tiempo actual en ticks desde el HAL
    uint64_t ticks = hal_tiempo_actual_tick();
    // Convertir ticks a microsegundos
    return ticks / HAL_TICKS_PER_US;
}

/**
 * Tiempo desde que se inició el temporizador en milisegundos
 */
Tiempo_ms_t drv_tiempo_actual_ms(void) {
    // Obtener el tiempo actual en ticks desde el HAL
    uint64_t ticks = hal_tiempo_actual_tick();
    // Convertir ticks a milisegundos
    return ticks / HAL_TICKS_PER_MS;
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

/**
 * Esperar hasta un determinado tiempo (en ms), devuelve el tiempo actual
 */
Tiempo_ms_t drv_tiempo_esperar_hasta_ms(Tiempo_ms_t ms) {
    // Obtener el tiempo actual
    Tiempo_ms_t tiempo_actual = drv_tiempo_actual_ms();
    // Si el tiempo actual es menor al tiempo objetivo, esperar
    if (tiempo_actual < ms) {
        drv_tiempo_esperar_ms(ms - tiempo_actual);
    }
    // Devolver el tiempo actual
    return drv_tiempo_actual_ms();
}


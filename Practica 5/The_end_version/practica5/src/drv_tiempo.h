/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los temporizadores
 * suministra los servicios independientemente del hardware
 */
 
#ifndef DRV_TIEMPO
#define DRV_TIEMPO

#include <stdint.h>

/**
 * define los tipos de datos asociados a tiempo de uso en la app
 */
typedef uint64_t Tiempo_us_t;
typedef uint32_t Tiempo_ms_t;

/**
 * inicializa el reloj y empieza a contar
 */
void drv_tiempo_iniciar(void);

/**
 * tiempo desde que se inicio el temporizador en microsegundos
 */
Tiempo_us_t drv_tiempo_actual_us(void);

/**
 * tiempo desde que se inicio el temporizador en milisegundos
 */
Tiempo_ms_t drv_tiempo_actual_ms(void);

/**
 * retardo: esperar un cierto tiempo en milisegundos
 */
void drv_tiempo_esperar_ms(Tiempo_ms_t ms);

/**
 * configura un reloj periódico que ejecuta la función "callback" con "ID_evento" cada "ms" tiempo
 */
void drv_tiempo_periodico_ms(Tiempo_ms_t ms,void(*funcion_callback)(),uint32_t ID_evento);

/**
 * esperar hasta un determinado tiempo (en ms), devuelve el tiempo actual
 */
Tiempo_ms_t drv_tiempo_esperar_hasta_ms(Tiempo_ms_t ms);

#endif

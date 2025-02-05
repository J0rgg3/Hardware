/* *****************************************************************************
 * P.H.2024: hal_ext_int, interface que nos independiza del hardware concreto
 */

#ifndef HAL_EXT_INT_H
#define HAL_EXT_INT_H

#include <stdint.h>

/**
 * Inicializa el módulo de interrupciones externas y configura el callback
 */
void hal_ext_int_iniciar( void(*funcionCallback)());

/**
 * Configura la interrupción externa de "pin"
 */
void hal_ext_int_iniciar_boton(uint32_t botton,uint32_t pin);

/**
 * Habilita las interrupciones de "pin"
 */
void hal_ext_int_habilitar(uint32_t pin);

/**
 * Deshabilita las interrupciones de "pin"
 */
void hal_ext_int_deshabilitar(void);

/**
 * permite poder despertarse al procesador mediante una interrupción en "pin" 
 */
void hal_ext_int_habilitar_despertar(uint32_t pin);

/**
 * impide poder despertarse al procesador mediante pulsación en "pin"
 */
void hal_ext_int_deshabilitar_despertar(uint32_t pin);

/**
 * devuelve un entero que indica si "pin" está activado o no (1 true, 0 false)
 */
uint32_t hal_ext_int_comprobar_pulsado(uint32_t pin);


#endif 

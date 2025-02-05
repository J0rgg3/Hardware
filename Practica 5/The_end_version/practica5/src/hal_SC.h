/* *****************************************************************************
 * P.H.2024: módulo de declaración para evitar condiciones de carrera
 */

#ifndef HAL_SC_H
#define HAL_SC_H

#include <stdint.h>

/**
 * entra en la región crítica (deshabilita interrupciones) 
 */
uint32_t SC_entrar_disable_irq(void);

/** 
 * sale de la región crítica (habilita interrupciones) 
 */
void SC_salir_enable_irq(void);

#endif 

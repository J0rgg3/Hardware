/* *****************************************************************************
 * P.H.2024: implementación en lpc para evitar condiciones de carrera acorde a hal_SC.H
 */

#include "hal_SC.h"
#include <LPC210x.H>   // Definiciones específicas del LPC2105

/* Contador para controlar las entradas a la región crítica */
static uint32_t m_in_critical_region = 0;

/**
 * entra en la región crítica (deshabilita interrupciones) 
 */
uint32_t SC_entrar_disable_irq(void) {
    __disable_irq();  // Deshabilitar interrupciones globales
    return (m_in_critical_region++);  // Incrementar el contador de la región crítica
}

/** 
 * sale de la región crítica (habilita interrupciones) 
 */
void SC_salir_enable_irq(void) {
    m_in_critical_region--;  // Decrementar el contador de la región crítica
    if (m_in_critical_region == 0) {
        __enable_irq();  // Habilitar interrupciones globales si han salido todas las entradas
    }
}

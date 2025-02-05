
#ifndef HAL_EXT_INT_H
#define HAL_EXT_INT_H

#include <stdint.h>

// Tipo de funci�n callback para la interrupci�n externa
typedef void (*ExtIntCallback)(void);

// Inicializa el m�dulo de interrupciones externas y configura el callback
void hal_ext_int_iniciar(ExtIntCallback callback);

// Habilita la interrupci�n externa
void hal_ext_int_habilitar(void);

// Deshabilita la interrupci�n externa
void hal_ext_int_deshabilitar(void);

#endif // HAL_EXT_INT_H

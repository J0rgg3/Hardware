
#ifndef HAL_EXT_INT_H
#define HAL_EXT_INT_H

#include <stdint.h>

// Tipo de función callback para la interrupción externa
typedef void (*ExtIntCallback)(void);

// Inicializa el módulo de interrupciones externas y configura el callback
void hal_ext_int_iniciar(ExtIntCallback callback);

// Habilita la interrupción externa
void hal_ext_int_habilitar(void);

// Deshabilita la interrupción externa
void hal_ext_int_deshabilitar(void);

#endif // HAL_EXT_INT_H

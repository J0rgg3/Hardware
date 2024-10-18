
#ifndef HAL_GPIO
#define HAL_GPIO
//escribir,inicar,leer y sentido
#include <stdint.h>
#define LED_PIN 1

#define OUTPUT 1
#define INPUT  0

/**
 * Permite emplear el GPIO y debe ser invocada antes
 * de poder llamar al resto de funciones de la biblioteca.
 * re-configura todos los pines como de entrada (para evitar cortocircuitos)
 */
void hal_gpio_iniciar(void);
void hal_gpio_sentido(uint32_t gpio, uint32_t dir) ;
uint32_t hal_gpio_leer(uint32_t gpio);
void hal_gpio_escribir(uint32_t gpio, uint32_t valor);
 
#endif

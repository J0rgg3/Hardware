
#ifndef HAL_GPIO
#define HAL_GPIO
//escribir,inicar,leer y sentido
#include <stdint.h>
#define LED_PIN 1

#define OUTPUT 1
#define INPUT  0

void hal_gpio_iniciar();
void hal_gpio_sentido(uint32_t gpio, uint32_t dir) ;
uint32_t hal_gpio_leer(uint32_t gpio);
void hal_gpio_escribir(uint32_t gpio, uint32_t valor);
 
#endif
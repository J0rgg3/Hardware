
#ifndef HAL_GPIO
#define HAL_GPIO
//escribir,inicar,leer y sentido
#include <stdint.h>
#define LED_PIN 1

#define OUTPUT 1
#define INPUT  0

 /**
 * configura el hardware necesario para controlar los pines GPIO.
 */
void hal_gpio_iniciar(void);

 /**
 * Dirección del pin: 0 para entrada, 1 para salida.
 */
void hal_gpio_sentido(uint32_t gpio, uint32_t dir);

 /**
 * Lee el valor de un pin GPIO configurado como entrada.
 */
uint32_t hal_gpio_leer(uint32_t gpio);

 /**
 * Escribe un valor en un pin GPIO configurado como salida.
 */
void hal_gpio_escribir(uint32_t gpio, uint32_t valor);
 
#endif

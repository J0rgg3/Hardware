/* *****************************************************************************
 * P.H.2024: GPIOs en nRF52840
 * implementación para cumplir el hal_gpio.h
 */
 
#include "hal_gpio.h"
#include <nrf.h>

/**
 * configura el hardware necesario para controlar los pines GPIO.
 */
 void hal_gpio_iniciar(){
  NRF_GPIO->DIR= 0x00000000;
 
 }
	 
/**
 * Dirección del pin: 0 para entrada, 1 para salida.
 */ 
 void hal_gpio_sentido(HAL_GPIO_PIN_T gpio, hal_gpio_pin_dir_t dir) {
	  uint32_t masc = (1UL << gpio);
    if (dir == HAL_GPIO_PIN_DIR_INPUT) {//input
      
			NRF_GPIO->DIR = NRF_GPIO->DIR & ~masc;
			 NRF_GPIO->PIN_CNF[gpio] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                   (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                   (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
			
    } else if(dir == HAL_GPIO_PIN_DIR_OUTPUT){ // output
			
			NRF_GPIO->DIR = NRF_GPIO->DIR | masc;
			 NRF_GPIO->PIN_CNF[gpio] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                                   (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                   (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
		}
}

 /**
 * Lee el valor de un pin GPIO configurado como entrada.
 */
 uint32_t hal_gpio_leer(HAL_GPIO_PIN_T gpio) {
	 uint32_t masc = (1UL << gpio);
	return ((NRF_GPIO->OUT & masc)!=0);
}

/**
 * Escribe en el gpio el valor
 */
 void hal_gpio_escribir(HAL_GPIO_PIN_T gpio, uint32_t valor) {
	uint32_t masc = (1UL << gpio);
	
	if ((valor & 0x01) == 0) NRF_GPIO->OUTCLR = masc;
	else NRF_GPIO->OUTSET = masc;
}


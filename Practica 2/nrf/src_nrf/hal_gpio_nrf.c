/* *****************************************************************************
 * P.H.2024: TODO
 */
 
#include "hal_gpio_nrf.h"
#include <nrf.h>

#define LED1_GPIO  (13UL)
#define LED2_GPIO  (14UL)
 
 void hal_gpio_iniciar(){
 
	 NRF_GPIO->PIN_CNF[LED1_GPIO] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                                   (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                   (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);

  NRF_GPIO->PIN_CNF[LED2_GPIO] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                                   (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                   (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
	
	//NRF_GPIO->OUTSET = (1UL << LED2_GPIO);
 
 }
	 
	 
	  
 void hal_gpio_sentido(uint32_t gpio, uint32_t dir) {
	  uint32_t masc = (1UL << gpio);
    if (dir == INPUT) {//output
      //definiir pin como entrada  
			NRF_GPIO->DIR = NRF_GPIO->DIR & ~masc;
    } else if(dir == OUTPUT){//input
			NRF_GPIO->DIR = NRF_GPIO->DIR | masc;
		}
}
 

 uint32_t hal_gpio_leer(uint32_t gpio) {
	 uint32_t masc = (1UL << gpio);
	return ((NRF_GPIO->OUT & masc)!=0);
}

 void hal_gpio_escribir(uint32_t gpio, uint32_t valor) {
	uint32_t masc = (1UL << gpio);
	
	if ((valor & 0x01) == 0) NRF_GPIO->OUTCLR = masc;
	else NRF_GPIO->OUTSET = masc;
}


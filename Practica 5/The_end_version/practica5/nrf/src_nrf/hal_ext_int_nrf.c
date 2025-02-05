/* *****************************************************************************
 * P.H.2024: hal_ext_int, implementación nrf acorde a hal_ext_int.h
 */

#include "hal_ext_int.h"
#include "nrf.h"  


/*
	El id del botón representa como se tratan los botones a lo largo del programa, desde este módulo hasta el main.
	En nuestro caso, se hara referencia al "boton 1" de la placa con el id 0 que a su vez es el canal en el que esperará eventos y así sucesivamente.
	Esto es relevante a la hora de tratar los botones para entender a cual se esta haciendo referencia
	canal 0 --> boton 1 --> id del boton = 0
	canal 1 --> boton 2 --> id del boton = 1
	canal 2 --> boton 3 --> id del boton = 2
	canal 3 --> boton 4 --> id del boton = 3

*/
void (*f_callback)();
void GPIOTE_IRQHandler(void);
static uint32_t num_canales;//Numero de canales configurados

/**
 * Inicializa el módulo, con ello habilita la IRQ del Gpiote y guarda función de callback
 */
void hal_ext_int_iniciar(void(*funcionCallback)()){

 	f_callback = funcionCallback;

	num_canales = 0;
	NVIC_EnableIRQ(GPIOTE_IRQn); // Habilita la IRQ del GPIOTE


}
/**
 * Inicializa el botón pasado por paramtetro "botton" en el canal pasado por parámetro "canal"
 */
void hal_ext_int_iniciar_boton(uint32_t botton,uint32_t canal){
	
		NRF_GPIO->PIN_CNF[botton] =(GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);
														
		NRF_GPIO->PIN_CNF[botton] |= (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos);
		// Configurar GPIOTE para detección de flancos
		NRF_GPIOTE->CONFIG[canal] = (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos) |
														(GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos) |
														(botton << GPIOTE_CONFIG_PSEL_Pos);
		num_canales++;
		NRF_GPIOTE->EVENTS_IN[canal] = 0; // Se limpian eventos por si acaso hay pendientes
	
		NRF_GPIOTE->INTENSET |= (1 << canal); // Habilita el canal correspondiente del 0-3
		
}


/**
 * Habilita las interrupciones del gpiote 
 * y limpia los eventos pendientes del canal que este relacionado con el pin pasado por parametro
 */
void hal_ext_int_habilitar(uint32_t pin){
	int canal = -1;
	//Busca el canal vinculado a pin
	 for (int i = 0; i < num_canales; i++){ 
		 uint32_t test = ((NRF_GPIOTE->CONFIG[i] & GPIOTE_CONFIG_PSEL_Msk) >> GPIOTE_CONFIG_PSEL_Pos);//para debugear
        if (((NRF_GPIOTE->CONFIG[i] & GPIOTE_CONFIG_PSEL_Msk) >> GPIOTE_CONFIG_PSEL_Pos) == pin){
            canal = i;
				}
		}
	
	if(canal != -1){//Si se encuentra un canal vinculado al pin:
		
		if (NRF_GPIOTE->EVENTS_IN[canal]) { // Comprueba si quedan interrupciones pendientes
			
			NRF_GPIOTE->EVENTS_IN[canal] = 0; // Si hay, las limpia
		}
	
		// Habilitar todas(siempre hay que limpiar antes)
		NRF_GPIOTE->INTENSET = 15; // Habilita el canal correspondiente del 0-3

	}

}

/**
 * Deshabilita las interrupciones del gpiote
 */
void hal_ext_int_deshabilitar(void){
	
	// deshabilitar todas
	NRF_GPIOTE->INTENCLR = 15;

}

/**
 * Configura el pin para despertar al procesador mediente la pulsación en "pin"
 */
void hal_ext_int_habilitar_despertar(uint32_t pin){
	
		NRF_GPIO->PIN_CNF[pin] |= (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos);

}

/**
 * Comprueba si el "pin" sigue pulsado
 */
uint32_t hal_ext_int_comprobar_pulsado(uint32_t pin){
	
		return ((NRF_GPIO->IN & (1 << pin)) == 0); 
	// El registro IN refleja en tiempo real el estado lógico actual (alto o bajo) de todos los pines GPIO configurados como entradas.
	// Si el botón no está pulsado tendrá un 1 en la dirección que corresponde al botón dentro del registro
}

/**
 * Función  IRQ que maneja las interrupciones del GPIOTE para cada cana
 */

void  GPIOTE_IRQHandler(void) {
	
	if (NRF_GPIOTE->EVENTS_IN[0]) { // Evento en el canal 0
		hal_ext_int_deshabilitar();
		NRF_GPIOTE->EVENTS_IN[0] = 0; //	Limpia eventos en el canal 0 
		uint32_t pin = (NRF_GPIOTE->CONFIG[0] & GPIOTE_CONFIG_PSEL_Msk) >> GPIOTE_CONFIG_PSEL_Pos;//Recoge el pin vinculado con el canal 0
		f_callback(pin); // Llama a la funcion calback(drv_botones_cb)
		
	}else if (NRF_GPIOTE->EVENTS_IN[1]) { // Evento en el canal 1
		hal_ext_int_deshabilitar();
		NRF_GPIOTE->EVENTS_IN[1] = 0; // Limpia eventos en el canal 1
		uint32_t pin = (NRF_GPIOTE->CONFIG[1] & GPIOTE_CONFIG_PSEL_Msk) >> GPIOTE_CONFIG_PSEL_Pos;//Recoge el pin vinculado con el canal 1
		f_callback(pin);
		
	} else if (NRF_GPIOTE->EVENTS_IN[2]) { // Evento en el canal 2
		hal_ext_int_deshabilitar();
		NRF_GPIOTE->EVENTS_IN[2] = 0; //	Limpia eventos en el canal 2
		uint32_t pin = (NRF_GPIOTE->CONFIG[2] & GPIOTE_CONFIG_PSEL_Msk) >> GPIOTE_CONFIG_PSEL_Pos;//Recoge el pin vinculado con el canal 2
		f_callback(pin);
		
	}else if (NRF_GPIOTE->EVENTS_IN[3]) {	// Evento en el canal 3
		hal_ext_int_deshabilitar();
		NRF_GPIOTE->EVENTS_IN[3] = 0;	// Limpia eventos en el canal 3
		uint32_t pin = (NRF_GPIOTE->CONFIG[3] & GPIOTE_CONFIG_PSEL_Msk) >> GPIOTE_CONFIG_PSEL_Pos;//Recoge el pin vinculado con el canal 3
		f_callback(pin);
	}
}


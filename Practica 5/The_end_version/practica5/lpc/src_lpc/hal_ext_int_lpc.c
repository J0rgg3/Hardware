/* *****************************************************************************
 * P.H.2024: hal_ext_int, implementación lpc acorde a hal_ext_int.h
 */

#include "hal_ext_int.h"
#include <LPC210x.H> /* LPC210x definitions */
#include "rt_evento_t.h"


/*
	El id del boton representa como se tratan los botones a lo largo del programa, desde este modulo hasta el main.
	Se relaciona al "boton 1" con el pin 14 del gpio , al "boton 2" con el pin 15 y al "boton 3" con el pin 16 y
	se hace referencia al boton 1 con el id = 0, al boton 2 con el id = 1 y al boton 3 con el id = 2.
	
	pin 14 del gpio --> eint1 --> botton 1 --> id_botton = 0
	pin 15 del gpio --> eint2 --> botton 2 --> id_botton = 1
	pin 16 del gpio --> eint0 --> botton 3 --> id_botton = 2

*/

void (*f_callback)();  // Declaración del puntero a la función de callback
// Confgurar interrupciones EINT(0-2)
void eint0_init(void);
void eint1_init(void);
void eint2_init(void);

/**
 * Inicializaa el módulo, con ello habilita la IRQ del Gpiote y guarda función de callback 
 */
void hal_ext_int_iniciar(void(*funcionCallback)()) {
    
	f_callback = funcionCallback;  // Asigna la función de callback al puntero	
}

/**
 * Inicializa el eint que hace referencia al botón pasado por parámtero "botton", 
 * el parámetro "aux" en este caso se ignora porque solo se usa en el nrf 
 */
void hal_ext_int_iniciar_boton(uint32_t pin,uint32_t aux){
		
	switch(pin){
		
			case 16:
				eint0_init();
				break;
			case 14:
				eint1_init();
				break;
			case 15:
				eint2_init();
				break;
		}
}

/**
 * Limpia las interrupciones pendientes del eint que estén relacionadas con el parámetro "pin"
 * y habilita las interrupciones de todos los eint
 */

void hal_ext_int_habilitar(uint32_t pin) {
		
		switch (pin){
			case 16:
				EXTINT = (1 << 0);					// Limpiar bandera de interrupción de EINT0
				break;
	
			case 14:
				EXTINT = (1 << 1);					// Limpiar bandera de interrupción de EINT1
				break;
	
			case 15:
				EXTINT = (1 << 2);					// Limpiar bandera de interrupción de EINT2
				break;
		}	

		VICIntEnable |= 0x00004000; 
		VICIntEnable |= 0x00008000; 
		VICIntEnable |= 0x00010000; 
		
}

/*
 * Deshabilita las interrupciones de todos los eint
 */
void hal_ext_int_deshabilitar(void) {
  
	VICIntEnClr = 0x00004000; // deshabilita EINT0
	VICIntEnClr = 0x00008000; // deshabilita EINT1
	VICIntEnClr = 0x00010000; // deshabilita EINT2	
}

/*
 * Habilita al eint relacionado con el parámetro "pin" para despertar al procesador cuando duerme
 */
void hal_ext_int_habilitar_despertar(uint32_t pin){ // EXTINT0,1,2 will awake the processor
		
		switch (pin){
			case 16:
				EXTWAKE = (1<<0); 				
				break;
	
			case 14:
					EXTWAKE = (1<<1); 				
				break;
	
			case 15:
				EXTWAKE = (1<<2); 							
				break;
		}	
}

/*
 * Deshabilita al eint relacionado con el parámetro "pin" para despertar al procesador cuando duerme
 */
void hal_ext_int_deshabilitar_despertar(uint32_t pin){ // EXTINT0,1,2 will awake the processor
			
		switch (pin){
			case 16:
				EXTWAKE = (0<<0); 	
				break;
	
			case 14:
				EXTWAKE = (0<<1);  				
				break;
	
			case 15:
				EXTWAKE = (0<<2); 			
				break;
		}	
}

/*
 * Comprueba si el botón relacionado con el eint que se relaciona con el parametro "pin" sigue pulsado
 * Esto se hace; limpiando las interrupciones pendientes del eint correspondiente y, posteriormnete comprobando si 
 * sigue habiendo interrupciones activas en ese eint.
 */
uint32_t hal_ext_int_comprobar_pulsado(uint32_t pin){
		uint32_t pulsado;
			
		switch (pin){
			case 16:
				EXTINT = (1 << 0);	
				pulsado = (EXTINT >> 0) & 1;
				break;
	
			case 14:
				EXTINT = (1<<1);
				pulsado = (EXTINT >> 1) & 1;			
				break;
	
			case 15:
				EXTINT = (1<<2); 
				pulsado = (EXTINT >> 2) & 1;		
				break;
		}	
		
		return pulsado;

}


// IRQ para manejar las interrupciónes externas EINT0
void eint0_ISR(void) __irq { // eint0 --> boton2
	
		hal_ext_int_deshabilitar(); // Deshabilitar EINT0
		
	
 		EXTINT = (1 << 0);  // Limpia la bandera de interrupción para EINT0
    f_callback(16);  // Llama a la función de callback
    
		VICVectAddr = 0;             // Acknowledge Interrupt
		
}

// Manejador de interrupción para EINT1 
void eint1_ISR(void) __irq { // eint1 --> boton0
	
	hal_ext_int_deshabilitar(); // Deshabilitar EINT1
	
	EXTINT = (1 << 1);  // Limpia la bandera de interrupción para EINT1
	
	f_callback(14);  // Llama a la función de callback
	
	VICVectAddr = 0;             // Acknowledge Interrupt
	
}

// Manejador de interrupción para EINT2
void eint2_ISR(void) __irq { // eint0 --> boton1
	
		hal_ext_int_deshabilitar(); // Deshabilitar EINT2
	
		EXTINT = (1 << 2); // Limpia la bandera de interrupción para EINT2
	
    f_callback(15); // Llama a la función de callback
    
    VICVectAddr = 0;    // Finalizar interrupción
	
		
}

//INICIALIZACION DE INTERRUPCIONES EXTERNAS EINT(0-2)

// Inicializar EINT0
void eint0_init(void) {
 
  EXTINT =  EXTINT | 1;        // clear interrupt flag   
	EXTWAKE |= (1<<0); 					// Habilita el boton para encender el sistema cuando duerma
	// configuration of the IRQ slot number 2 of the VIC for EXTINT0
	VICVectAddr2 = (unsigned long)eint0_ISR;          // set interrupt vector in 0
    // 0x20 bit 5 enables vectored IRQs. 
		// 14 is the number of the interrupt assigned. Number 14 is the EINT0 (see table 40 of the LPC2105 user manual  
	PINSEL1 	= PINSEL1 & 0xfffffffC;	// Sets bits 0 and 1 to 0
	PINSEL1 	= PINSEL1 | 0x1;					// Enable the EXTINT0 interrupt
	
	VICVectCntl2 = 0x20 | 14;                   
  VICIntEnable = VICIntEnable | 0x00004000;   // Enable EXTINT0 Interrupt
	
}

// Inicializar EINT1
void eint1_init(void) {
 
    EXTINT |= (1<<1); // Limpiar bandera de interrupción de EINT1
		EXTWAKE|= (1<<1); // Habilita el boton para encender el sistema cuando duerma
    // Configurar la interrupción EINT1
    VICVectAddr3 = (unsigned long)eint1_ISR; // Asignar la dirección del manejador
	
	// Configurar el pin para EINT1
    PINSEL0 = PINSEL0 & 0xCFFFFFFF; // Pone a 0 los bits 2 y 3
    PINSEL0 = PINSEL0 | 0x20000000;   // Habilita EINT1

    VICVectCntl3 = 0x20 | 15;         // Habilitar IRQ vector y asignar número de interrupción 15 (EINT1)
    VICIntEnable |= 0x00008000;       // Habilitar interrupción EINT1 en el VIC

   
}

// Inicializar EINT2
void eint2_init(void) {

    EXTINT |= (1 << 2); // Limpiar bandera de interrupción de EINT2
		EXTWAKE|= (1 << 2); // Habilita el boton para encender el sistema cuando duerma
    // Configurar la interrupción EINT2
    VICVectAddr4 = (unsigned long)eint2_ISR; // Asignar la dirección del manejador
	
	// Configurar el pin para EINT2
    PINSEL0 = PINSEL0 & 0x3FFFFFFF; // Pone a 0 los bits 4 y 5
    PINSEL0 = PINSEL0 | 0x80000000;   // Habilita EINT2
	
    VICVectCntl4 = 0x20 | 16;                // Habilitar IRQ vector y asignar número de interrupción 16 (EINT2)
    VICIntEnable |= 0x00010000;               // Habilitar interrupción EINT2 en el VIC

   // Configurar el pin para EINT2
    PINSEL0 = PINSEL0 & 0x3FFFFFFF; // Pone a 0 los bits 4 y 5
    PINSEL0 = PINSEL0 | 0x80000000;   // Habilita EINT2
}





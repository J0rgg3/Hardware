/* *****************************************************************************
 * P.H.2024: módulo de implementación del juego Simon
 */

#include <stdint.h>
#include "rt_evento_t.h"
#include "simon.h"
#include "hal_random.h"
#include "svc_alarma.h"
#include "drv_leds.h"
#include "drv_tiempo.h"
#include "rt_GE.h"
#include "rt_fifo.h"
#include "hal_WDT.h"

#define TIMEOUT 5000 						// Tiempo maximo para contestar
#define TIEMPO_ENTRE_NIVEL 1000 // Tiempo de espera entre pasar un nivel y empezar el siguente
#define AUMENTO_DIFICULTAD 250  // Ms que se van a ir restando a los retardos para aumentar la dificultad
#define DIFICULTAD_MAX 500
#define TIEMPO_ALRMAS 50				//Retardo con el que se van a programar las alarmas de transicion de estados
#define RETARDO_SUCCES 250			//Retardo de las alarmas del aviso succes

static _GameState estado; 				// Estado inicial
static uint32_t nivel; 						// Indica el  nivel en el que esta el usuario y la longuitud de la secuncia de leds
static uint32_t retardo_entreLed; // Indica el tiempo que pasa entre que se apaga un led y se enciende el siguiente
static uint32_t retardo_apagado;  // Indica el tiempo que pasa un led encendido 

static uint32_t retardo_base_apagado; 	// Indica el retardo con el que se incia según la dificultad elegida
static uint32_t retardo_base_entreLed;  // Indica el retardo con el que se inicia según la dificultad elegida


static uint32_t secuencia[100]; // Secuencia de leds en orden
static uint32_t index;					// Número que representa el lugar del led actual dentro del vector de secuencia
static uint32_t acertados; 			// Número de botones que se han acertado en el input

static uint32_t num_botones;
static uint32_t num_leds;

#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
	static volatile uint32_t aciertos, Num_Niveles_Acertados = 0; // para contabilizar el número de niveles realizados correctamente en una misma partida
	static volatile uint32_t num_fallos, fallos = 0; 							// para contabilizar el número de fallos hasta que se deja de jugar
	static volatile uint32_t alarmas_activas = 0; 								// para contabilizar el número de alarmas que se encuentran activas
	static volatile uint32_t veces_encolado[EVENT_TYPES]; 				// para contabilizar el número de veces que se ha encolado cada tipo de evento
	static volatile uint32_t tareas_evento[EVENT_TYPES]; 					// para contabilizar el número de tareas asociadas a cada tipo de evento
	static volatile uint32_t eventos_encolados = 0;  							// para contabilizar el número de eventos encolados en total
	static volatile uint32_t eventos_no_procesados = 0; 					// para contabilizar el número de eventos que se encuentran sin procesar
	static volatile Tiempo_us_t t_finSecuencia, t_empezar_pulsar; // para calcular el tiempo entre que se finaliza de mostrar una secuancia y se empieza a pulsar cualquier botón
#endif


#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
	/**
	 * devuelve el número de niveles acertados en una partida
	 */
	uint32_t simon_niveles_acertados() {
	 return Num_Niveles_Acertados;
	}
	
	/**
	 * devuelve el número de total de fallos hasta que se deja de jugar
	 */
	uint32_t simon_num_fallos() {
		return fallos;
	}
#endif

/**
 * inicializa el juego del simon
 */
void simon_iniciar(uint32_t leds,uint32_t botones){
	
	num_leds = leds;
	num_botones = botones;
	estado = e_ELEGIR_DIFICULTAD;
	drv_led_encender(0);
	hal_WDT_iniciar(1);
	hal_random_iniciar(1);
	#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
		Num_Niveles_Acertados = 0; 
		fallos = 0; 
	#endif
	// Suscribir tareas
	svc_GE_suscribir(ev_LED,simon_tratar);
	svc_GE_suscribir(ev_PULSAR_BOTON,simon_tratar);
	svc_GE_suscribir(ev_TIMEOUT,simon_tratar);
	svc_GE_suscribir(ev_RESULTADO,simon_tratar);
	svc_GE_suscribir(ev_PULSAR_DOS_BOTONES,simon_tratar);
	// Inicia el juego
	svc_alarma_activar(svc_alarma_codificar(0,TIEMPO_ALRMAS),ev_RESULTADO,0);
	
	rt_GE_lanzador();
}

/**
 * avisa de que se ha acertado la secuencia del nivel actual
 */
void simon_aviso_succes(uint32_t auxiliar){
	// enciende el primer y ultimo led
	if(auxiliar == 0){
		drv_led_apagar(0);
		drv_led_encender(1);
		drv_led_encender(num_leds);

	}else{
		drv_led_apagar(0);
	}	
}

/**
 * avisa de que se ha fallado la secuencia del nivel actual
 */
void simon_aviso_fail(uint32_t auxiliar){
	if(auxiliar == 0){
		drv_led_apagar(0);
		drv_led_encender(0);
	}else{
	
		drv_led_apagar(0);
	}
}

/**
 * secuencia que avisa del inicio del juego. 
 * Se usa el parametro "auxiliar" de svc_alarma_activar para manejar el estado
 * de la secuenia de aviso.
 */
void simon_aviso_inicio(uint32_t auxiliar){
	switch (auxiliar){
		case 0:
			// Apaga todos los leds
			svc_alarma_activar(svc_alarma_codificar(0,RETARDO_SUCCES),ev_RESULTADO,1);
			drv_led_apagar(auxiliar);
			drv_led_encender(auxiliar+1);
			break;
		case 1:
			svc_alarma_activar(svc_alarma_codificar(0,RETARDO_SUCCES),ev_RESULTADO,2);
			drv_led_apagar(auxiliar);	
			drv_led_encender(auxiliar+1);
			break;
		
		case 2:
			svc_alarma_activar(svc_alarma_codificar(0,RETARDO_SUCCES),ev_RESULTADO,3);
			drv_led_apagar(auxiliar);
			drv_led_encender(auxiliar+1);
			break;
		
		case 3:
			svc_alarma_activar(svc_alarma_codificar(0,RETARDO_SUCCES),ev_RESULTADO,4);
			drv_led_apagar(auxiliar);
			drv_led_encender(auxiliar+1);	
			break;
	}

}

/**
 * secuencia que avisa del final del juego
 */
void simon_aviso_fin(){
		drv_led_encender(1); // Enciende el primer led
		drv_led_encender(num_leds-1); // Enciende el penultimo led
		drv_led_encender(num_leds); // Enciende el ultimo led
}

/**
 * selector de dificultad del juego
 */
void simon_selector_dificultad(uint32_t boton){
	
	retardo_base_apagado = 2500;
	retardo_base_entreLed = 1500;
	// Número mayor = dificultad mayor
	retardo_base_apagado = retardo_base_apagado - (350 *(boton));
	retardo_base_entreLed = retardo_base_entreLed - (150 *(boton));				
}

/**
 * gestiona la máquina de estados del simon, tratando cada evento que ocurra en el juego
 */
void simon_tratar(uint32_t evento,uint32_t auxiliar){
	
	if(evento == ev_PULSAR_DOS_BOTONES){	// Si se pulsan dos botones a la vez durante 3 segundos
	
		estado = e_FIN;	// estado fin no hace nada solo representa que ha acabado y no entra en ningun estado diferente
		simon_aviso_fin();
	} 

	switch(estado){
		
		case e_ELEGIR_DIFICULTAD:
			
			if(evento == ev_PULSAR_BOTON){
				simon_selector_dificultad(auxiliar);
				// TRANSICIÓN DE ESTADO
				estado = e_INIT;
				svc_alarma_activar(svc_alarma_codificar(0,TIEMPO_ALRMAS), ev_RESULTADO, 0);
			}
		break;
		
		case e_INIT:
			switch(evento){
				case ev_RESULTADO:
					// AVISO DE INICIO
					
					if(auxiliar < num_leds){
						
						simon_aviso_inicio(auxiliar);
						
					}else{		
						// Apaga todos los leds
						drv_led_apagar(0);
						
						// VARIABLES GENERALES
						index = 0;
						acertados = 0;
						nivel = 1;
						secuencia[0] = hal_random_generar(1,num_botones); // Genera un número del 1 al 4 que representará el led a encender
						// RETARDOS
						retardo_apagado = retardo_base_apagado;
						retardo_entreLed = retardo_base_entreLed;
						
						// TRANSICIóN AL SIGUIENTE ESTADO
						estado = e_SHOW_SEQUENCE;
						svc_alarma_activar(svc_alarma_codificar(0,TIEMPO_ALRMAS), ev_LED, 0);
					}
						break;	
				}

			break;
		
		case e_SHOW_SEQUENCE:
			switch(evento){
				case ev_LED:
				
					if(auxiliar == 0){ // si el axuliar es 0 se enciende el led
						
						drv_led_encender(secuencia[index]);
						svc_alarma_activar(svc_alarma_codificar(0,retardo_apagado), ev_LED, 1);
						
					}else{ // si el axuliar es 1 se apaga el led
						drv_led_apagar(secuencia[index]);
						index++;
						if(index < nivel){
							svc_alarma_activar(svc_alarma_codificar(0,retardo_entreLed), ev_LED, 0);
					
						}else{
							#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
								t_finSecuencia = drv_tiempo_actual_us();
							#endif
							estado = e_WAIT_FOR_INPUT;
							index = 0; // Se resetea index
							acertados = 0;
							svc_alarma_activar(svc_alarma_codificar(0,TIMEOUT), ev_TIMEOUT, auxiliar); // Se programa la alarma en caso de no contestar a tiempo
						}
					}	
					break;
			}
			break;
		
		case e_WAIT_FOR_INPUT:
			// Comprobar input que acaba de entrar(ev_pulsarBoton), si corresponde con el led que toca
			switch(evento){
				case ev_PULSAR_BOTON:
					 #if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
						t_empezar_pulsar = drv_tiempo_actual_us() - t_finSecuencia; // tiempo entre que acaba la secuencia y se empieza a pulsar botones
					#endif
					
					if(auxiliar + 1 == secuencia[index]){ // Comprueba si el botón pulsado corresponde con el led correcto
						acertados++; // Suma un acierto
						index++; // avanza al siguiente led de la secuencia
						if(acertados == nivel){
							
							estado = e_SUCCESS;
							svc_alarma_activar(svc_alarma_codificar(0,0),ev_TIMEOUT,auxiliar); // Se desactiva la alarma
							svc_alarma_activar(svc_alarma_codificar(0,TIEMPO_ALRMAS),ev_RESULTADO,0);
						}else{
							svc_alarma_activar(svc_alarma_codificar(0,TIMEOUT),ev_TIMEOUT,auxiliar); // Se programa la alarma en caso de no contestar a tiempo
						}
								
					}else{
						estado = e_FAIL;
						svc_alarma_activar(svc_alarma_codificar(0,0),ev_TIMEOUT,auxiliar);
						svc_alarma_activar(svc_alarma_codificar(0,TIEMPO_ALRMAS),ev_RESULTADO,0);	
					}
					break;
				
				case ev_TIMEOUT:
					// Caso en que no le de a tiempo a algún boton
					
					estado = e_FAIL;
					svc_alarma_activar(svc_alarma_codificar(0,TIEMPO_ALRMAS),ev_RESULTADO,0);
					break;
			}
			break;
			
		case e_SUCCESS:
			switch(evento){
				case ev_RESULTADO:
				
					// AVISO DE ACIERTO
					if(auxiliar == 0){
						simon_aviso_succes(0);
						svc_alarma_activar(svc_alarma_codificar(0,500),ev_RESULTADO, 1);
					}else{
						simon_aviso_succes(1);
				
						// AUMENTO DE DIFIFULTAD
						if(retardo_apagado > DIFICULTAD_MAX){
							retardo_apagado = retardo_apagado - AUMENTO_DIFICULTAD;
						}
						if(retardo_entreLed > DIFICULTAD_MAX){
							retardo_entreLed =	retardo_entreLed - AUMENTO_DIFICULTAD;
						}
						
						secuencia[nivel] = hal_random_generar(1,num_botones); // Genera un número del 1 al 4 que representará el led a encender
						nivel++;
						index = 0;
						
						#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
							for(int i = 0; i < EVENT_TYPES; i++) {
								veces_encolado[i] = rt_FIFO_estadisticas((EVENTO_T) i);
								tareas_evento[i] = rt_GE_estadisticas((EVENTO_T) i);
							}
							Num_Niveles_Acertados++;
							eventos_encolados = rt_FIFO_eventos_encolados();
							alarmas_activas = svc_alarmas_estadisticas();
							eventos_no_procesados = rt_FIFO_num_eventos_sinProcesar();
						#endif
						
							// TRANSICIÓN DE ESTADO
						estado = e_SHOW_SEQUENCE;
						svc_alarma_activar(svc_alarma_codificar(0,TIEMPO_ENTRE_NIVEL), ev_LED, 0);
					}
				break;
			}
			break;
		
		case e_FAIL:
			switch(evento){
				case ev_RESULTADO:
					
					#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
						for(int i = 0; i < EVENT_TYPES; i++) {
							veces_encolado[i] = rt_FIFO_estadisticas((EVENTO_T) i);
							tareas_evento[i] = rt_GE_estadisticas((EVENTO_T) i);
						}
						fallos ++;
						eventos_encolados = rt_FIFO_eventos_encolados();
						alarmas_activas = svc_alarmas_estadisticas();
						aciertos = simon_niveles_acertados();
						eventos_no_procesados = rt_FIFO_num_eventos_sinProcesar();
					#endif
					
					if(auxiliar == 0){
						// AVISO DE FALLO
						simon_aviso_fail(0);
						svc_alarma_activar(svc_alarma_codificar(0,500), ev_RESULTADO, 1);
					}else{
						simon_aviso_fail(1);
						// TRANSICIÓN DE ESTADO
						estado = e_INIT;
						svc_alarma_activar(svc_alarma_codificar(0,TIEMPO_ENTRE_NIVEL),ev_RESULTADO,0 );
					}	
				break;
			}
		break;
			
		case e_FIN:
			simon_aviso_fin();
			
			#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
				for(int i = 0; i < EVENT_TYPES; i++) {
					veces_encolado[i] = rt_FIFO_estadisticas((EVENTO_T) i);
					tareas_evento[i] = rt_GE_estadisticas((EVENTO_T) i);
				}
				alarmas_activas = svc_alarmas_estadisticas();
				eventos_encolados = rt_FIFO_eventos_encolados();
				aciertos = simon_niveles_acertados();
				num_fallos = simon_num_fallos();
				eventos_no_procesados = rt_FIFO_num_eventos_sinProcesar();
			#endif
			break;
	}
}


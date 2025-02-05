/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los botones
 * suministra los servicios de iniciar y tratar independientemente del hardware
 */
 
#include "drv_botones.h"
#include "rt_fifo.h"
#include "rt_GE.h"
#include "svc_alarma.h"
#include "hal_gpio.h"
#include "hal_ext_int.h"
#include "board.h"

#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
	#include "drv_tiempo.h"
	static Tiempo_us_t t_pulsado;
#endif

// Define los estados de la máquina de estados del botón
typedef enum {
    e_reposo,
    e_entrando,
    e_esperando,
    e_soltado
} Estado_boton;



#define TRP 100 //Tiempo rebote presión
#define TEP 150 //Tiempo encuesta periódica
#define TRD 100 // Tiempo rebote depresión
#define TIME_PULSAR_DOS_BOTONES 3000 // Tiempo que hay que mantener dos botones

static Estado_boton estado; 		// Estado
static uint32_t primera;	  		// Representa si es la primera vez que se comprueba si un boton está pulsado o no

static uint32_t boton_Pulsar;		//varible que guarda el id del evento pasado por el parametro "botonPulsar" en botones inicicar 
static uint32_t boton_Retardo;	//varible que guarda el id del evento pasado por el parametro "botonRetardo" en botones inicicar 
static uint32_t dosBotones_Pulsados;	//varible que guarda el id del evento pasado por el parametro "botonRetardo" en botones inicicar 


// Callback del botón que se ejecuta al detectar una pulsación
static void (*drv_cb)(uint32_t, uint32_t);
#if BUTTONS_NUMBER > 0
	static const uint8_t button_list[BUTTONS_NUMBER] = BUTTON_LIST;
#endif


/**
 * Función que llama al callback del módulo botones.
 * Busca el boton relacionado el parametro "pin",
 * cuando lo encuentra llama al callback con la varible boton_Pulsar y 
 * el indice que ocupa en la lista de botones
 * 
 */
void drv_botones_cb(uint32_t pin){
		
		for(int i = 0;i < BUTTONS_NUMBER;i++){
			if(button_list[i] == pin){
				drv_cb(boton_Pulsar,i);
				break;
			}
		}
		
}
	
/** 
 * Inicializa el módulo de botones, configurándolos como interrupciones externas,
 * suscribiendo los eventos correspondientes y finalmente, devuelve el número de botones inicializados.
 */
uint32_t drv_botones_iniciar(void(*f_callback)(uint32_t, uint32_t),uint32_t botonPulsar,uint32_t botonRetardo,uint32_t dosBotonesPulsados) {
		
	drv_cb = f_callback;
	boton_Pulsar = botonPulsar; 
	boton_Retardo = botonRetardo;
	dosBotones_Pulsados = dosBotonesPulsados;
	
	hal_ext_int_iniciar(drv_botones_cb); // Inicializa el módulo ext_int
	
	for(int i = 0; i < BUTTONS_NUMBER; i++){
		
		hal_gpio_sentido(button_list[i],HAL_GPIO_PIN_DIR_INPUT); // configurar botones como input

	}
	for(int i = 0; i < BUTTONS_NUMBER; i++){
		hal_ext_int_iniciar_boton(button_list[i],i);// configurar botones como interrupciones externas
	}
		estado = e_reposo;
		primera = 1;
    // Suscribirse al evento de pulsación de botón
    svc_GE_suscribir(botonPulsar, drv_botones_tratar);
	  svc_GE_suscribir(botonRetardo, drv_botones_tratar);
	
	return BUTTONS_NUMBER;
}

/**
 * comprueba y devuelve cuantos botones hay pulsados a la vez.
 */
uint32_t comprobar_botones_pulsados(){
	
	uint32_t botones_pulsados = 0;
	
	for(int i = 0; i < BUTTONS_NUMBER; i++){
			if(hal_ext_int_comprobar_pulsado(button_list[i]) == 1){
				botones_pulsados++;
			}	
	}
	return botones_pulsados;
} 

/**	
 * Gestiona los eventos de botón identificados por "evento" 
 */
void drv_botones_tratar(uint32_t evento, uint32_t auxData) { // Auxdata == id boton
    switch (estado) {
				// Estado de reposo: el botón no está siendo presionado
        case e_reposo:
            if (evento == boton_Pulsar) {
							
								estado = e_entrando; // Cambiar al estado e_entrando

								// Activar alarma de retardo Trp y generar el evento ev_BOTON_RETARDO
								svc_alarma_activar(svc_alarma_codificar(0,TRP), boton_Retardo, auxData);
            }
            break;
				
				// Estado 'entrando': El botón ha sido presionado pero aún no se ha soltado
        case e_entrando:
						#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
								t_pulsado = drv_tiempo_actual_us();
						#endif
            if (evento == boton_Retardo) {
								estado = e_esperando; // Cambiar al estado e_esperando
								svc_alarma_activar(svc_alarma_codificar(1,TEP), boton_Retardo, auxData);
            }
            break;
        
				// Estado 'esperando': El botón sigue presionado, esperando a que se suelte
				case e_esperando:
            if (evento == boton_Retardo) {
								// Verificar si el botón sigue presionado o no
								if (!hal_ext_int_comprobar_pulsado(button_list[auxData])) {
										// Cambiar al estado e_soltado
										primera = 1;
										estado = e_soltado;
										// Activar alarma de retardo Trd y generar el evento ev_BOTON_RETARDO
										svc_alarma_activar(svc_alarma_codificar(0,0),dosBotones_Pulsados,0);
										svc_alarma_activar(svc_alarma_codificar(0,TRD), boton_Retardo, auxData);
								}else{
									// Comprueba si hay dos botones a la vez, y si es el caso programa una alarma para que dentro de 3 minutos encole un evento pulsar dos botones
									if(primera  && comprobar_botones_pulsados() == 2){
										svc_alarma_activar(svc_alarma_codificar(0,TIME_PULSAR_DOS_BOTONES),dosBotones_Pulsados,0);
										primera = 0;
									}
								} 
            }
            break;
        
				// Estado 'soltado': El botón ha sido liberado, volviendo al estado de reposo
				case e_soltado:
								#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
									t_pulsado = drv_tiempo_actual_us() - t_pulsado; // tiempo transcurrido entre que se pulsa y se suelta el botón
								#endif
                // Reiniciar al estado e_reposo
								estado = e_reposo;

								// Habilitar interrupciones externas para el botón
								hal_ext_int_habilitar(button_list[auxData]);
            break;
				
        default:
            break;
    }
}

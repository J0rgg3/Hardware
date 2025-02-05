// drv_botones.c
#include "drv_botones.h"
#include "rt_evento_t.h"
#include "rt_fifo.h"
#include "rt_GE.h"
#include "svc_alarma.h"
#include "hal_gpio.h"
#include "hal_ext_int.h"
#include "drv_tiempo.h"

// Estado actual de la máquina de estados
static Estado_boton estado_actual = e_reposo; // Estado inicial
static uint32_t ID_evento_boton = ev_PULSAR_BOTON; // Evento de pulsación de botón

// Callback del botón que se ejecuta al detectar una pulsación
static void drv_cb(uint32_t ID_evento, uint32_t auxData){
    // Encola el evento cuando se detecta una pulsación
    rt_FIFO_encolar(ID_evento, auxData); // No sé seguro si tiene que ser así (en diapositivas llama a cb_encolar(ev_PULSAR_BOTON, ...)
}


// Función para iniciar el módulo de botones
void drv_botones_iniciar(uint32_t monitor) {
    // Configurar el estado inicial
    estado_actual = e_reposo;

    // Suscribirse al evento de pulsación de botón
    svc_GE_suscribir(ev_PULSAR_BOTON, drv_botones_tratar);

    // Configurar interrupciones externas para el botón
    hal_ext_int_iniciar(??);

    // Activar alarma para manejar inactividad
    svc_alarma_iniciar(monitor, drv_botones_tratar, ev_INACTIVIDAD);
}


// Función de manejo de eventos de botón
void drv_botones_tratar(EVENTO_T evento, uint32_t auxData) {
    switch (estado_actual) {
        case e_reposo:
            if (evento == ev_PULSAR_BOTON) {
                // Cambiar al estado e_entrando
								estado_actual = e_entrando;

								// Deshabilitar interrupciones externas mientras se procesa la pulsación
								hal_ext_int_deshabilitar(??);
								
								// callback para encolar ID_ev y auxdata
								drv_cb(evento, auxData);

								// Activar alarma de retardo Trp y generar el evento ev_BOTON_RETARDO
								svc_alarma_activar(1000, ev_BOTON_RETARDO, 0);
            }
            break;
        case e_entrando:
            if (evento == ev_BOTON_RETARDO) {
                // Cambiar al estado e_esperando
								estado_actual = e_esperando;
							
								// callback para encolar ID_ev y auxdata
								drv_cb(evento, auxData);

								// Activar alarma periódica Tep para verificar el estado del botón
								svc_alarma_activar(1000 | (1 << 31), ev_BOTON_RETARDO, 0);
            }
            break;
        case e_esperando:
            if (evento == ev_BOTON_RETARDO) {
								// Verificar si el botón sigue presionado o no
								if (hal_gpio_leer() == 0) { // Suponemos que 0 indica el botón liberado
										// Cambiar al estado e_soltado
										estado_actual = e_soltado;

										// Activar alarma de retardo Trd y generar el evento ev_BOTON_RETARDO
										svc_alarma_activar(500, ev_BOTON_RETARDO, 0);
								}
								// Si el botón sigue presionado, mantenemos la alarma periódica
            }
            break;
        case e_soltado:
            if (evento == ev_BOTON_RETARDO) {
                // Reiniciar al estado e_reposo
								estado_actual = e_reposo;

								// Habilitar interrupciones externas para el botón
								hal_ext_int_habilitar(??);
            }
            break;
        default:
            break;
    }
}



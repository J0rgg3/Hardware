/* *****************************************************************************
 * P.H.2024: Driver/Manejador de los Monitores
 * suministra los servicios de iniciar, marcar, desmarcar, ... independientemente del hardware
 * recoge de la definicion de la placa: 
 * MONITOR_NUMBER - numero de monitores existentes 
 * MONITOR_LIST - array de monitores que existen y sus gpios
 * el gpio de cada MONITOR_x
 * MONITOR_ACTIVE_STATE - si los monitores son activos con el gpio a nivel alto o bajo
 *
 * Usa los servicios y tipos de datos del hal_gpio.h
 */

#include "hal_gpio.h"
#include "drv_monitor.h"
#include "board.h"


#if MONITOR_NUMBER > 0
	static const uint8_t monitor_list[MONITOR_NUMBER] = MONITOR_LIST;
#endif

/**
 * inicializa los monitores, los deja apagados y devuelve el número de monitores disponibles en la plataforma
 */
uint32_t drv_monitor_iniciar(){
	#if MONITOR_NUMBER > 0
		for (uint32_t i = 0; i < MONITOR_NUMBER; ++i) 			{
			hal_gpio_sentido(monitor_list[i], HAL_GPIO_PIN_DIR_OUTPUT);
			drv_monitor_desmarcar(i+1);
		}
  #endif //MONITOR_NUMBER > 0	
	
	return MONITOR_NUMBER;  //definido en board_xxx.h en cada placa... 
}

/**
 * enciende el monitor id
 */
void drv_monitor_marcar(uint32_t id){
	#if MONITOR_NUMBER > 0
		if ((id <= MONITOR_NUMBER) && (id >0)) hal_gpio_escribir(monitor_list[id-1], MONITOR_ACTIVE_STATE);
  #endif //MONITOR_NUMBER > 0		
}

/**
 * apaga el monitor id
 */
void drv_monitor_desmarcar(uint32_t id){
	#if MONITOR_NUMBER > 0
		if ((id <= MONITOR_NUMBER) && (id >0)) hal_gpio_escribir(monitor_list[id-1], ~MONITOR_ACTIVE_STATE);
  #endif //MONITOR_NUMBER > 0	
}

/**
 * conmuta el monitor de on a off y viceversa
 * primero consulta el estado y lo invierte
 */
void drv_monitor_conmutar(uint32_t id){
	#if MONITOR_NUMBER > 0
		if ((id <= MONITOR_NUMBER) && (id >0)){
			hal_gpio_escribir(monitor_list[id-1], ~hal_gpio_leer(monitor_list[id-1]));
		}
  #endif //MONITOR_NUMBER > 0		
}

//otras???

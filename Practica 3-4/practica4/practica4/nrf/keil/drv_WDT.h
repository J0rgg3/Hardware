#ifndef DRV_WDT_H
#define DRV_WDT_H

#include <stdint.h>

// Definiciones para el tiempo de timeout en segundos
#define DEFAULT_TIMEOUT_SECONDS 2           // Tiempo de espera por defecto de 2 segundos
#define WDT_RELOAD_VALUE 0x6E524635         // Valor de recarga para el registro RR

// Prototipos de funciones

/* Inicializa el watchdog con un timeout especificado
*/
void WDT_Init(uint32_t timeout_seconds);   

/* Recarga el watchdog para evitar el timeout
*/
void WDT_Reload(void);                      

#endif 

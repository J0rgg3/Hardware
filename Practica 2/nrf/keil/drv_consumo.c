
#include "hal_consumo.h"
#include "drv_consumo.h"


/**
 * Inicializa el módulo de bajo consumo.
 */
void drv_consumo_iniciar(void) {
    

}

/**
 * Poner el sistema en modo de espera.
 */
void drv_consumo_esperar(void) {
    hal_consumo_esperar();
}

/**
 * Poner el sistema en modo dormir (bajo consumo).
 */
void drv_consumo_dormir(void) {
    hal_consumo_dormir();    
}

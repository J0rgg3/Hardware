#include "drv_WDT.h"
#include <nrf.h>  // Incluye los registros y definiciones específicas del nRF52840

void WDT_Init(uint32_t timeout_seconds) {
    // Configurar CRV (Counter Reload Value) para el timeout especificado
    NRF_WDT->CRV = (timeout_seconds * 32768) - 1;  // LFCLK = 32.768 kHz, timeout en segundos

    // Habilitar el canal de recarga 0
    NRF_WDT->RREN |= (1 << 0);  // Activa el primer canal de recarga

    // Iniciar el watchdog
    NRF_WDT->TASKS_START = 1;
}

void WDT_Reload(void) {
    // Recargar el primer canal para evitar el timeout
    NRF_WDT->RR[0] = WDT_RELOAD_VALUE;
}

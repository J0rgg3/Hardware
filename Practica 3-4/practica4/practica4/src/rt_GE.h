#ifndef RT_GE_H
#define RT_GE_H

#include <stdint.h>
#include "rt_evento_t.h"

// Máximo de tareas suscritas a cada evento
#define rt_GE_MAX_SUSCRITOS 4

typedef void (*CallbackFunc)(uint32_t auxiliar);

// Estructura de suscripción
typedef struct {
    EVENTO_T ID_evento;
    CallbackFunc callback;
} Suscripcion;

void rt_GE_iniciar(uint32_t monitor);
void rt_GE_lanzador(void);
void svc_GE_suscribir(EVENTO_T evento, CallbackFunc f_callback);
void svc_GE_cancelar(EVENTO_T evento, CallbackFunc f_callback);
void rt_GE_tratar(EVENTO_T evento, uint32_t auxiliar);

#endif // RT_GE_H

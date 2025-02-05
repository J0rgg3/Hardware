/* *****************************************************************************
 * P.H.2024: módulo de declaración del juego Simon
 */

#ifndef SIMON_H
#define SIMON_H

#include <stdint.h>

// Estados de la máquina de estados que gestiona el funcionamiento del Simon
typedef enum {
	e_INIT,
	e_SHOW_SEQUENCE,
	e_WAIT_FOR_INPUT,
	e_SUCCESS,
	e_FAIL,
	e_FIN,
	e_ELEGIR_DIFICULTAD
} _GameState;

/**
 * inicializa el juego del simon
 */
void simon_iniciar( uint32_t leds,uint32_t num_botones);

/**
 * gestiona la máquina de estados del simon, tratando cada evento que ocurra en el juego
 */
void simon_tratar(uint32_t evento,uint32_t auxiliar);

/**
 * avisa de que se ha acertado la secuencia del nivel actual
 */
void simon_aviso_succes(uint32_t auxiliar);

/**
 * avisa de que se ha fallado la secuencia del nivel actual
 */
void simon_aviso_fail(uint32_t auxiliar);

/**
 * secuencia que avisa del inicio del juego
 */
void simon_aviso_inicio(uint32_t auxiliar);

/**
 * secuencia que avisa del final del juego
 */
void simon_aviso_fin(void);

/**
 * selector de dificultad del juego
 */
void simon_selector_dificultad(uint32_t boton);

#if defined (DEBUG_LPC2105_simulador) || defined (DEBUG_BOARD_PCA10056)
	uint32_t simon_niveles_acertados(void);
	
	uint32_t simon_num_fallos(void);
#endif
	
#endif

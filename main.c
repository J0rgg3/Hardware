// fuentes Proyecto Hardware 2024
// version 0.1

#include "matriz_3x3.h"

    // Inicialización de matrices usando el tipo de datos Matriz3x3
static int Test_A[N][N] = {
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1}
	};

static int Test_B[N][N] = {
		{9, 8, 7},
		{6, 5, 4},
		{3, 2, 1}
	};

		
/* *****************************************************************************
 * declaración funciones internas 
 */
		
// funcion que ejecuta las distintas versiones de la implementacion en C, ARM y Thumb y verifica que dan el mismo resultado.
// recibe las matrices con las que operar
// devuelve si todas las versiones coinciden en el numero de terminos_no_cero o no y el resultado de la opoeración.
uint8_t matrizNxN_verificar(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]);

//TODO ???	
//??? void matrizNxN_transponer ???
//??? void matrizNxN_sumar ???		
		

/* *****************************************************************************
 * IMPLEMENTACIONES 
 */
		
void matrizNxN_multiplicar_C(int A[N][N], int B[N][N], int Resultado[N][N]){
//TODO
}

//funcion que calcula Resultado = A*B + transpuesta(C*D) y devuelva el numero de terminos distintos de cero en el Resultado
//ayudandose de funcion matrizNxN_multiplicar_C que calcula A*B de NxN
uint8_t matrizNxN_operar_C(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]){
	uint8_t terminos_no_cero;
	
	matrizNxN_multiplicar_C(A, B, Resultado);
	//TODO
	//....
	return terminos_no_cero;
}
		
uint8_t matrizNxN_verificar(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]){
	uint8_t terminos_no_cero_C;
	uint8_t terminos_no_cero_ARM_C;
	uint8_t terminos_no_cero_ARM;
	uint8_t terminos_no_cero_THB;
	uint8_t resultado;
	
	terminos_no_cero_C = matrizNxN_operar_C(A, B, C, D, Resultado);
		
	//TODO llamar al resto de implementaciones en ensamblador
	
	//TODO resultado = (terminos_no_cero_C ! = al resto....
	return resultado;
}

// MAIN 
int main (void) {
	int Resultado_E[N][N];
	int error;

	int Test_C[N][N]  = {
		{1, 0, 2},
		{0, 1, 2},
		{2, 0, 1}
	};

	int Test_D[N][N]  = {
		{2, 1, 0},
		{1, 2, 0},
		{0, 0, 2}
	};
		
	error = matrizNxN_verificar(Test_A, Test_B, Test_C, Test_D, Resultado_E);
	
	while(1); //no hay S.O., no se retorna
}

/**
 *@}
 **/

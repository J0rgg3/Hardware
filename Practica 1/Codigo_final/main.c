// fuentes Proyecto Hardware 2024
// version 0.1

#include "matriz_3x3.h"
#include <stdio.h>


    // Inicializaci�n de matrices usando el tipo de datos Matriz3x3
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
 * declaraci�n funciones internas 
 */
		
// funcion que ejecuta las distintas versiones de la implementacion en C, ARM y Thumb y verifica que dan el mismo resultado.
// recibe las matrices con las que operar
// devuelve si todas las versiones coinciden en el numero de terminos_no_cero o no y el resultado de la opoeraci�n.
uint8_t matrizNxN_verificar(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]);

/*
* 	Procedimiento que recibe una matriz de tipo entero como parámetro y carga en matriz Traspuesta el resultado 
*  de trasponer la matriz pasada como parámetro.
*/		
void matrizNxN_transponer(int A[N][N],int Traspuesta[N][N]){
	for(int fila = 0; fila < N; fila++){
		for(int columna = 0; columna < N; columna++){

			Traspuesta[columna][fila] = A[fila][columna];
			
		}
	}

}
		

/* *****************************************************************************
 * IMPLEMENTACIONES 
 */

/* 
*	Procedimiento que recibe dos matrices de tipo entero como parámetro y carga en la matriz Resultado
*  la matriz originada de realizar la multiplicación de las dos matrices recibidas como parámetro.
*/	
		
void matrizNxN_multiplicar_C(int A[N][N], int B[N][N], int Resultado[N][N]){

    int suma,fila = 0,columna = 0;
	while(fila < 3){
		
		suma = 0;
		for(int j = 0; j < N; j++){
			
			suma += A[fila][j] * B[j][columna];
			
		}

		Resultado[fila][columna] = suma;

		if(columna == 2){
			columna = 0;
			fila++;
		}else{ 
			columna++;
		}
		}
}


/*
* Función que recibe 4 matrices como parámetro, calcula Resultado = A*B + transpuesta(C*D) y devuelve el número de términos distintos de cero en el Resultado
* ayudándose de función matrizNxN_multiplicar_C que calcula A*B y C*D de NxN y de la función matrizNxN_transponer que
* calcula transpuesta(C*D).
*/

uint8_t matrizNxN_operar_C(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]){
	uint8_t terminos_no_cero = 0;
	int AB[N][N],CD[N][N],Traspuesta[N][N];
	matrizNxN_multiplicar_C(A, B, AB);//AB = A*B

	matrizNxN_multiplicar_C(C, D, CD);//CD = C*D
	
	//TRASPONER CD
	matrizNxN_transponer(CD,Traspuesta);
	
	//SUMAR CD TRASPUESTA + AB
	for (int i = 0; i <N ; i++){
		for (int j = 0; j < N; j++)
		{
			if((Resultado[i][j] = (AB[i][j] + Traspuesta[i][j])) != 0){
				//RESULTADOS NO CERO	
				terminos_no_cero++;
			}
		}
			
	}
	
	return terminos_no_cero;
}

/* función que ejecuta las distintas versiones de la implementación en C, ARM y Thumb y verifica que dan el mismo resultado.
*  recibe las matrices con las que operar y devuelve si todas las versiones coinciden en el numero de terminos_no_cero o no 
*  y el resultado de la operación.
*/	
		
uint8_t matrizNxN_verificar(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]){
	uint8_t terminos_no_cero_C;
	uint8_t terminos_no_cero_ARM_C;
	uint8_t terminos_no_cero_ARM;
	uint8_t terminos_no_cero_THB;
	uint8_t resultado;
	
	terminos_no_cero_C = matrizNxN_operar_C(A, B, C, D, Resultado);
	terminos_no_cero_ARM_C = matriz3x3_operar_ARM_C(A,B,C,D,Resultado);
	terminos_no_cero_ARM = matriz3x3_operar_ARM(A,B,C,D,Resultado);
	terminos_no_cero_THB = matriz3x3_operar_THB(A,B,C,D,Resultado);	
	//TODO llamar al resto de implementaciones en ensamblador
	resultado = (terminos_no_cero_C == terminos_no_cero_ARM_C) && (terminos_no_cero_C == terminos_no_cero_ARM) && (terminos_no_cero_C == terminos_no_cero_ARM)&& (terminos_no_cero_C == terminos_no_cero_THB);
	//resultado = terminos_no_cero_C != 9;
	//resultado = terminos_no_cero_C == 9;
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
	if(error == 0){
		return -1;
	}else{
	
		while(1);
	}
}

/**
 *@}
 **/

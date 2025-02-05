/* guarda para evitar inclusiones m?ltiples ("include guard") */
#ifndef matrizNxN
#define matrizNxN

#include <inttypes.h>

//el codigo en C debe ser independiente del tama�o de las matrices usando la definicion de "N"
//las versiones en ensamblador hacerlas para matrices de 3x3
#define N 3

/* *****************************************************************************
 * declaracion de funciones visibles en el exterior
 */
void matrizNxN_multiplicar_C(int A[N][N], int B[N][N], int Resultado[N][N]);
void matrizNxN_sumar(int A[N][N], int B[N][N], int Resultado[N][N]);
void matrizNxN_transponer(int A[N][N],int Traspuesta[N][N]);
uint8_t matrizNxN_operar_C(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]);
uint8_t matriz3x3_operar_ARM_C(int A[N][N], int B[N][N], int C[N][N], int D[N][N],int Resultado[N][N]);
uint8_t matriz3x3_operar_ARM(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]);
uint8_t matriz3x3_operar_THB(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int Resultado[N][N]);
//TODO
#if 0
// funciones a implementar en ARM y thumb para matrices de 3x3

//operar implementada integramente en ARM, se puede hacer con subfunciones o incrustado todo el codigo (pero que se entienda)


//operar implementada en Thumb


#endif // 0

#endif /* matrizNxN */

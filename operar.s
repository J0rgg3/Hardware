//operar 
//    STMDB     R13!,{R4-R9,R14}//apila regitros (mas abajo)14 y 9-4
//    SUB       R13,R13,#0x0000006C//r13 = r13 - 108
//    MOV       R8,R0
//    MOV       R9,R1
//    MOV       R4,R2
//    MOV       R5,R3
//    LDR       R6,[R13,#0x0088]	
/*
    31: void matrizNxN_transponer(int A[N][N],int Traspuesta[N][N]){ 
0x00000438  E52D4004  STR       R4,[R13,#-0x0004]!
0x0000043C  E1A03000  MOV       R3,R0
0x00000440  E1A02001  MOV       R2,R1
    32:         for(int fila = 0; fila < N; fila++){ 
0x00000444  E3A00000  MOV       R0,#0x00000000
0x00000448  EA00000B  B         0x0000047C
    33:                 for(int columna = 0; columna < N; columna++){ 
    34:  
0x0000044C  E3A01000  MOV       R1,#0x00000000
0x00000450  EA000006  B         0x00000470
    35:                         Traspuesta[columna][fila] = A[fila][columna]; 
    36:                          
    37:                 } 
    38:         } 
    39:  
0x00000454  E080C080  ADD       R12,R0,R0,LSL #1
0x00000458  E083C10C  ADD       R12,R3,R12,LSL #2
0x0000045C  E79CC101  LDR       R12,[R12,R1,LSL #2]
0x00000460  E0814081  ADD       R4,R1,R1,LSL #1
0x00000464  E0824104  ADD       R4,R2,R4,LSL #2
0x00000468  E784C100  STR       R12,[R4,R0,LSL #2]
0x0000046C  E2811001  ADD       R1,R1,#0x00000001
0x00000470  E3510003  CMP       R1,#0x00000003
0x00000474  BAFFFFF6  BLT       0x00000454
    32:         for(int fila = 0; fila < N; fila++){ 
    33:                 for(int columna = 0; columna < N; columna++){ 
    34:  
    35:                         Traspuesta[columna][fila] = A[fila][columna]; 
    36:                          
    37:                 } 
    38:         } 
    39:  
0x00000478  E2800001  ADD       R0,R0,#0x00000001
0x0000047C  E3500003  CMP       R0,#0x00000003
0x00000480  BAFFFFF1  BLT       0x0000044C
    40: } 



*/
AREA data, DATA
	Test_A DCB 1,0,0,0,1,0,0,0,1
	Test_B DCB 9,8,7,6,5,4,3,2,1
	

AREA codigo,CODE
	IMPORT matrizNxN_multiplicar_C 

	LDR r0, =Test_A 
	LDR r1,	=Test_B
	
	STMDB     R13!,{R4-R9,R14}
	
bfin 		b bfin
												// |Variables locales
sumar											// |registros de r4-rx
			MOV r12, r15						// |FP(r11)
			STMDB r13!, {r4- ,r11,r12,r14,r15}	// |SP(r13)
			SUB r11, r12, #4					// |LR(r14)
			SUB r13, #SpaceForLocalVaribles		// |PC(r15) 
			//r0-r3 = matrices?
			//
			
			
			LDMDB r11, {r4- ,r11,r13,r15}			//epilogo
			

trasponer


END
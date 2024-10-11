
	AREA datos, DATA
	
N 			EQU 3
Tamanio		EQU N*N
;Test_A 		DCD 1,0,0,0,1,0,0,0,1
;Test_B 		DCD 9,8,7,6,5,4,3,2,1
;Resultado 	SPACE N*N*4

	AREA codigo,CODE
			
			EXPORT matriz3x3_operar_ARM_C
			EXPORT matriz3x3_operar_ARM
			IMPORT matrizNxN_multiplicar_C
			
			PRESERVE8 {TRUE}
; Función que recibe 4 matrices como parámetro, calcula Resultado = A*B + transpuesta(C*D) y devuelve el número de términos distintos de cero en el Resultado
; ayudándose de la función matrizNxN_multiplicar_C que calcula A*B y C*D de NxN y de la función matrizNxN_transponer que calcula transpuesta(C*D)

			


matriz3x3_operar_ARM_C

			MOV IP, SP								;PROLOGO
			STMDB SP!, {r4-r10,FP,IP,LR,PC}
			SUB FP, IP, #4
			SUB SP, #108							;Hacemos espacio en la pila para las variables locales, 2 matrices 3x3 108=9*4*3
			; r0 = Test_A
			; r1 = Test_B
			; r2 = Test_C
			; r3 = Test_D	
			
			MOV r4,r0 							; r4 = @Test_a
			MOV r5,r1 							; r5 = @Test_b
			MOV r6,r2 							; r6 = @Test_c
			MOV r7,r3 							; r7 = @Test_d
			LDR r9,[IP]							; r9 = IP = @Resultados_E
	
			MOV r2,SP						 	; r2 =@AB r2 apunta a la memoria que hemos hecho para las variables locales, en concreto a donde apunta sp
			
			
 			bl matrizNxN_multiplicar_C
			
			;RECORDATORIO Resultado en SP(AB)
			
	;PONEMOS LOS PARAMETROS PARA LLAMAR A MULTPLICAR_C OTRA VEZ PARA HACER C*D
	
			mov r0,r6							; r0 = @Test_c
			mov r1,r7							; r1 = @Test_d
			ADD r2,SP,#36						; r2 =@CD r2 apunta a la memoria que hemos hecho para las variables locales, en concreto a donde apunta sp+36 que lo que AB
			
			bl matrizNxN_multiplicar_C
			
			;RECORDATORIO Resultado en SP+36(CD)
			;A PARTIR DE AQUI PODRIAMOS HACER USO DE R4-R7 YA QUE YA NO NECESITAMOS LAS MATRICES ORIGINALES

			;TRASPUESTA CD
			add r0,SP,#36						; r0 = @CD
			add r1,SP,#72						; r1 = @Traspuesta
			
			bl Trasponer
			
			;RECORDATORIO Resultado en SP+72(CD TRASPUESTA)
			
			;SUMA AB + CD traspuesta
			mov r0,#0
			LDR r4,=Tamanio						; r4 = N
			mov r5,SP							; r5 = AB
			add r6,SP,#72						; r6 = CD traspuesta
			
suma		subs r4,r4,#1						; r4(N)--
			bmi fin_suma
			ldr r7,[r5,r4,LSL #2]				; r7 = AB[r4]
			ldr r8,[r6,r4,LSL #2]				; r8 = CD[r4]
			add r8,r8,r7						; r8=r8+r7
			cmp r8,#0
			addne r0,r0,#1						; r0++ = terminos no cero++
			str r8,[r9,r4,LSL #2]				; Resultado_E[r4] = r8
			b suma
fin_suma

			;RESULTADOS NO CERO
			
			
			LDMDB FP, {r4-r10,FP,SP,PC}	




; Función que recibe 4 matrices como parámetro, calcula Resultado = A*B + transpuesta(C*D) y devuelve el número de términos distintos de cero en el Resultado
; ayudándose de la función matrizNxN_multiplicar que calcula A*B y C*D de NxN y de la función Transponer que calcula transpuesta(C*D)




matriz3x3_operar_ARM

			MOV IP, SP								;PROLOGO
			STMDB SP!, {r4-r10,FP,IP,LR,PC}
			SUB FP, IP, #4
			SUB SP, #108							;Hacemos espacio en la pila para las variables locales, 2 matrices 3x3 108=9*4*3


			
			MOV r4,r0 							; r4 = @Test_a
			MOV r5,r1 							; r5 = @Test_b
			MOV r6,r2 							; r6 = @Test_c
			MOV r7,r3 							; r7 = @Test_d
			LDR r9,[IP]							; r9 = IP = @Resultados_E
	
			MOV r2,SP						 	; r2 =@AB r2 apunta a la memoria que hemos hecho para las variables locales, en concreto a donde apunta sp
			
			
 			bl matrizNxN_multiplicar
			
			;RECORDATORIO Resultado en SP(AB)
			
	;PONEMOS LOS PARAMETROS PARA LLAMAR A MULTPLICAR_C OTRA VEZ PARA HACER C*D
	
			mov r0,r6							; r0 = @Test_c
			mov r1,r7							; r1 = @Test_d
			ADD r2,SP,#36						; r2 =@CD r2 apunta a la memoria que hemos hecho para las variables locales, en concreto a donde apunta sp+36 que lo que AB
			
			bl matrizNxN_multiplicar
			
			;RECORDATORIO Resultado en SP+36(CD)
			;A PARTIR DE AQUI PODRIAMOS HACER USO DE R4-R7 YA QUE YA NO NECESITAMOS LAS MATRICES ORIGINALES

			;TRASPUESTA CD
			add r0,SP,#36						; r0 = @CD
			add r1,SP,#72						; r1 = @Traspuesta
			
			bl Trasponer
			
			;RECORDATORIO Resultado en SP+72(CD TRASPUESTA)
			
			;SUMA AB + CD traspuesta
			mov r0,#0
			LDR r4,=Tamanio						; r4 = N
			mov r5,SP							; r5 = AB
			add r6,SP,#72						; r6 = CD traspuesta
			
suma_ARM	subs r4,r4,#1						; r4(N)--
			bmi fin_suma_ARM
			ldr r7,[r5,r4,LSL #2]				; r7 = AB[r4]
			ldr r8,[r6,r4,LSL #2]				; r8 = CD[r4]
			add r8,r8,r7						; r8=r8+r7
			cmp r8,#0
			addne r0,r0,#1						; r0++ = terminos no cero++
			str r8,[r9,r4,LSL #2]				; Resultado_E[r4] = r8
			b suma_ARM
fin_suma_ARM

			;RESULTADOS NO CERO
			
			
			LDMDB FP, {r4-r10,FP,SP,PC}	



;-------------------------------------------------------------------SUBRUTINAS----------------------------------------------------------------------
												
; Procedimiento que recibe una matriz de tipo entero como parámetro y carga en matriz Traspuesta el resultado 
; de trasponer la matriz pasada como parámetro.
; En r8 se utiliza la N, en r5 el índice i, en r7 el índice j,

Trasponer
				
			MOV IP, SP						;PROLOGO
			STMDB SP!, {r4-r10,FP,IP,LR,PC}
			SUB FP, IP, #4
					
			
		
			
			LDR r8, =N					; r8 = N
			MOV r6,r8					; r6 = i
				
bcl_i
			subs r6,r6,#1           	; i < N?
			bmi epilogo
			mov r7,r8          			 ; r7 = j

bcl_j
			
			subs r7,r7,#1           		; j < N?
			bmi bcl_i
			mul r9,r6,r8        		; r9 = i*N = r9
			add r9,r9,r7        		; r9 = i*N+j
			ldr r10,[r0,r9, LSL#2]     	; r10 = CD[i][j]
			
			mul r9,r7,r8        		; r9 = jN
			add r9,r9,r6        		; r9 = jN+i
			
			str r10,[r1,r9, LSL #2]     ; transpuesta[j][i] = matriz [i][j]
			
			b bcl_j
	

epilogo 	LDMDB FP, {r4-r10,FP,SP,PC}	



; Procedimiento que recibe dos matrices de tipo entero como parámetro y carga en la matriz Resultado
; la matriz originada de realizar la multiplicación de las dos matrices recibidas como parámetro.
; En r4 se utiliza el índice i, en r5 el índice j, en r6 EL índice k 


matrizNxN_multiplicar
		
			MOV IP, SP								;PROLOGO
			STMDB SP!, {r4-r10,FP,IP,LR,PC}
			SUB FP, IP, #4
			SUB SP,#4
			
			; r0 = @A
			; r1 = @B
			; r2 = @Resultado
			; SP = i
			; SP+4= j
			; SP+8= k
			
			LDR r4,=N							; r4 = i
			
bucle_i
			subs r4,r4,#1         		 	 	; i--
			bmi epilogo2
			LDR r5,=N          			 		; r5 = j
			
bucle_j
			subs r5,r5,#1         		  		; j--
			bmi bucle_i	
												; j < 0?
			LDR r7,=N
			mla r8,r4,r7,r5						; r8  = i*N + j =[i][j]
			mov r9,#0
			STR r9,[r2,r8,LSL#2]				; Resultado[i][j] = 0 inicialización del elemento
			STR r8,[SP]							; r8 como variable local

			LDR r6,=N							; r6 = k
			
bucle_k		subs r6,r6,#1						; k < N?
			bmi bucle_j
			
		; A[i][k] y B[k][j]

			LDR r7,=N					; r7  = N
			mla r8,r4,r7,r6        		; r8  = i*N + K =[i][k]
			mla r9,r6,r7,r5				; r9  = k*N + j =[k][j]

			LDR r10,[r0,r8,LSL#2]		; r10 = A[i][k]
			MOV r8,r10					; r8  = A[i][k]
			LDR r10,[r1,r9,LSL#2]		; r10 = B[k][j]
			
		; A[i][k] * B[k][j]

			mul r9,r8,r10				; r9  = A[i][k] * B[k][j]
			
		;Resultado [i][j] = A[i][k] * B[k][j]

			LDR r8,[SP]					; r8  = i*N + j =[i][j]
			LDR r7,[r2,r8,LSL#2]		; r7 = Resultado[i][j]
			add r10,r9,r7				; r10 = Resultado[i][j] + A[i][k] * B[k][j]
			STR r10,[r2,r8,LSL#2]		; Resultado[i]][j] = Resultado[i][j] + A[i][k] * B[k][j]
			

			b bucle_k	
			
			
			

epilogo2 	LDMDB FP, {r4-r10,FP,SP,PC}



	END


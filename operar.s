
	AREA datos, DATA
	
N 			EQU 3
Tamanio		EQU N*N
;Test_A 		DCD 1,0,0,0,1,0,0,0,1
;Test_B 		DCD 9,8,7,6,5,4,3,2,1
;Resultado 	SPACE N*N*4

	

	AREA codigo,CODE
			
			EXPORT matriz3x3_operar_ARM_C
			IMPORT matrizNxN_multiplicar_C
			PRESERVE8 {TRUE}
			
	

matriz3x3_operar_ARM_C

			MOV IP, SP								;PROLOGO
			STMDB SP!, {r4-r10,FP,IP,LR,PC}
			SUB FP, IP, #4
			SUB SP, #108							;Hacemos espacio en la pila para las variables locales, 2 matrices 3x3 108=9*4*3
			; r0 = Test_A
			; r1 = Test_B
			; r2 = Test_C
			; r3 = Test_D	
			; RESULTADO_E?¿?¿?¿ deberia estar en la pila al ser el 5 parametro pero no lo encuentro
	;SALVAGUARDA DE LAS MEMORIAS DE LAS MATRICES Y MULTIPLICACIÓN DE A*B
			
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





;-------------------------------------------------------------------SUBRUTINAS----------------------------------------------------------------------
												



Trasponer
				
			MOV IP, SP						;PROLOGO
			STMDB SP!, {r4-r10,FP,IP,LR,PC}
			SUB FP, IP, #4
					
		
			
			mov r4,r0 					; r4 = @CD
			mov r5,r1 					; r5 = @Traspuesta
		
			MOV r6,#0					; r6 = i
			LDR r8, =N					; r8 = N
			
			
bucle_i
			cmp r6,r8           		 ; i < N?
			bge epilogo
			mov r7,#0          			 ; r7 = j

bucle_j
			;prologo
			cmp r7,r8           		; j < N?
			bge sig_i
			mul r9,r6,r8        		; r9 = i*N = r9
			add r9,r9,r7        		; r9 = i*N+j
			ldr r10,[r4,r9, LSL#2]     	; r10 = CD[i][j]
			
			mul r9,r7,r8        		; r9 = jN
			add r9,r9,r6        		; r9 = jN+i
			
			str r10,[r5,r9, LSL #2]     ; transpuesta[j][i] = matriz [i][j]
			add r7,r7,#1        		; j++
			b bucle_j

sig_i
			add r6,r6,#1        		; i++
			b bucle_i
			

epilogo 	LDMDB FP, {r4-r10,FP,SP,PC}	






	END


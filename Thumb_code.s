	AREA datos, DATA
	
N 			EQU 3
Tamanio		EQU N*N
;Test_A 		DCD 1,0,0,0,1,0,0,0,1
;Test_B 		DCD 9,8,7,6,5,4,3,2,1
;Resultado 	SPACE N*N*4

	AREA codigo,CODE
		
		EXPORT matriz3x3_operar_THB
		PRESERVE8 {TRUE}
		ARM

matriz3x3_operar_THB

			MOV IP, SP								;PROLOGO
			STMDB SP!, {r4-r10,FP,IP,LR,PC}
			SUB FP, IP, #4
			SUB SP, #108	
			
			MOV r4,r0 							; r4 = @Test_a
			MOV r5,r1 							; r5 = @Test_b
			MOV r6,r2 							; r6 = @Test_c
			MOV r7,r3 							; r7 = @Test_d
			LDR r9,[IP]							; r9 = IP = @Resultados_E
			
;			MOV r0,r1
;			; Pasar como parametro resultados
;			ADR r4, Trasponer + 1 	; Processor starts in ARM state
;			BX r4 					; Inline switch to Thumb state
;			THUMB 					; Subsequent instructions are Thumb
			
			
			MOV r0,r6
			MOV r1,r7
			MOV r2,r9
			
			bl Trasponer
			ARM

			
			LDMDB FP, {r4-r10,FP,SP,PC}	

;-------------------------------------------------------------------SUBRUTINAS----------------------------------------------------------------------
			
Trasponer	
			MOV IP, SP								;PROLOGO
			STMDB SP!, {r4-r6,FP,IP,LR,PC}
			SUB FP, IP, #4
			ADR r4,Trasponer_thumb+1
			bx r4
			
Trasponer_thumb	
			THUMB
			
			; r0 = @A	
			
			LDR r2, =N					; r2 = N
			MOVS r3,#0					; r3 = i
				
bcl_i
			CMP r3,r2           		 ; i < N?
			bge epilogo
			MOVS r4,#0          			 ; r7 = j

bcl_j
			cmp r4,r2           		; j < N?
			bge sg_i
			MOVS r5,r2					; r5 = N	
			MULS r5,r3,r5       		; r5 = i*N 
			ADDS r5,r5,r4        		; r5 = i*N+j
			LSLS r5,r5,#2
			LDR r6,[r0,r5]     			; r6 = CD[i][j]
			
			MOVS r5,r2					; r5 = N	
			MULS r5,r4,r5       		; r5 = j*N 
			ADDS r5,r5,r3        		; r5 = j*N+i
			LSLS r5,r5,#2
			STR r6,[r1,r5]				; Traspuesta [j][i] = r6 = CD[i][j]

			ADDS r4,r4,#1        		; j++
			B bcl_j

sg_i
			ADDS r3,r3,#1        		; i++
			B bcl_i
			
			
		
epilogo 	
			ARM
			LDMDB FP, {r4-r7,FP,SP,PC}	
;--------------------------------------------------------------------------------------------										


Multiplicar	
			
			MOV IP, SP								;PROLOGO
			STMDB SP!, {r4-r10,FP,IP,LR,PC}
			SUB FP, IP, #4
			SUB SP, #4	
			ADR r4,Multiplicar_thumb+1
			bx r4	

Multiplicar_thumb
			THUMB
			; r0 = @A
			; r1 = @B
			; r2 = @Resultado
			
			
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
			MULS r7,r4,r7						; r7 = i*N + j =[i][j]
			ADDS r7,r5						;
			LSLS r7,r7,#2
			MOVS r3,#0							; r3 = 0
			STR r3,[r2,r7]						; Resultado[i][j] = 0 inicialización del elemento
			STR r7,[SP]							; Guardamos r7(indice [i][j]) como variable local

			LDR r6,=N							; r6 = k
			
bucle_k		subs r6,r6,#1						; k < N?
			bmi bucle_j
			
		; A[i][k] y B[k][j]

			LDR r7,=N					; r7  = N
			;mla r8,r4,r7,r6        		; r8  = i*N + K =[i][k]
			MULS r7,r4,r7				; r7 = i*N + j =[i][j]
			ADDS r7,r6					;
			LSLS r7,r7,#2
			LDR r3,[r0,r7]				; r3  = A[i][k]
			MOV r8,r3					; r8  = A[i][k]
			
			LDR r7,=N				
			;mla r9,r6,r7,r5			; r9  = k*N + j =[k][j]
			MULS r7,r6,r7				; r7 = i*N + j =[i][j]
			ADDS r7,r5					;
			LSLS r7,r7,#2
			LDR r3,[r1,r7]				; r3 = B[k][j]
			MOV r7,r8					; r7 = A[i][k]
			
		; A[i][k] * B[k][j]

			MULS r3,r7,r3				; r3  = A[i][k] * B[k][j]
			
			
		;Resultado [i][j] = A[i][k] * B[k][j]

			LDR r7,[SP]					; r7  = i*N + j =[i][j]
			MOV r8,r7					; r8 =[i][j] 
			LDR r7,[r2,r7]				; r7 = Resultado[i][j]
			ADDS r3,r7				 	; r3 = Resultado[i][j] + A[i][k] * B[k][j]
			MOV r7,r8
			STR r3,[r2,r7]				; Resultado[i]][j] = Resultado[i][j] + A[i][k] * B[k][j]
			

			b bucle_k	
			
	;ARM			
epilogo2 

			;LDMDB FP, {r4-r7,FP,SP,PC}			; Load R2,R6 and R15 (PC) from the stack


	END
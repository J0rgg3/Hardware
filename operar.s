
	AREA datos, DATA
	
N 			EQU 3
Tamanio		EQU N*N
Test_A 		DCD 1,0,0,0,1,0,0,0,1
Test_B 		DCD 9,8,7,6,5,4,3,2,1
Resultado 	SPACE N*N 

	

	AREA codigo,CODE
	EXPORT inicio
inicio
	;IMPORT matrizNxN_multiplicar_C 

			LDR r0,=Test_A 
			LDR r1,=Test_B
			LDR r2,=Resultado
			LDR r3,=Tamanio
			
			bl sumar

bfin 		b bfin
												; |Variables locales
sumar											; |registros de r4-rx
												; |FP(r11)
			MOV IP, SP
			STMDB SP!, {r4-r10,FP,IP,LR,PC}
			SUB FP, IP, #4
												; |LR(r14)
												; |PC(r15) 
			;r0-r1= Test_a-Test_b
            mov r4,r0 ; r4 = @Test_a
			mov r5,r1 ; r5 = @Test_b
			mov r6,r2 ; r6 = @resultados
			mov r7,r3 ; r7 = n*n
	
	
bucle		subs r7,r7,#1
			bmi fin_bucle
			ldr r8,[r4,r7,LSL #2]
			ldr r9,[r5,r7,LSL #2]
			add r9,r9,r8
			str r9,[r6,r7,LSL #2]
			b bucle
fin_bucle
			
			LDMDB FP, {r4-r10,FP,SP,PC}		;epilogo
			


trasponer
			mov r4,#0                ; r4 = i
			ldr r5,=N
			
bucle_i
			cmp r4,r5           ; i < N?
			bge end
			mov r6,#0           ; r6 = j

bucle_j
			;prologo
			cmp r6,r5           ; j < N?
			bge sig_i
			;ldr r0,=matriz
			mul r7,r4,r5        ; r7 = iN
			add r7,r7,r6        ; r7 = iN+j
			LSL r7,r7,#2        ; r7 = (iN+j)4
			ldr r1,[r0,r7]      ; r1 = matriz[i][j]
			;ldr r2,=transpuesta
			mul r8,r6,r4        ; r8 = jN
			add r8,r8,r5        ; r8 = jN+i
			LSL r8,r8,#2        ; r8 = (jN+i)4
			str r1,[r2,r8]      ; transpuesta[j][i] = matriz [i][j]
			add r6,r6,#1        ; j++
			b bucle_j

sig_i
			add r4,r4,#1        ; i++
			b bucle_i

end  		;epilogo


	END


#include  <stdint.h>
#include "rt_evento_t.h"
#include "drv_tiempo.h"
#include "svc_alarma.h"
#include "drv_monitor.h"
#include "rt_GE.h"


static uint32_t monitor_overflow;
static void(*funcion_callback)(uint32_t,uint32_t);
static Alarma alarmas[svc_ALARMAS_MAX ];
static int num_alarmas;		

/************************************************************************************************************************
 * Planteamiento:
 *	Se configuran las interrupciones periodicas para que interrumpan cada x tiempo cada vez que interrumpen llama a encolar 
 *	A su vez, usamos ese mismo timer para saber cuando tiene que saltar cada una de las alarmas guardanado momento(ms)
 *	en el que acaba cada alarma alarmas[].periodo
 *	
*/

void svc_alarma_iniciar(uint32_t overflow, void*(f_callback), uint32_t ID_evento){
		//Inicializaci?n 
		monitor_overflow = overflow;	
		funcion_callback = f_callback;
		num_alarmas = 0;		
	
		svc_GE_suscribir(ID_evento, svc_alarma_tratar);	//Suscribir evento
		drv_tiempo_periodico_ms(2000,f_callback,ID_evento);	//Inciar temprizador periodico

}
/*********************************************************************************************************
 * Funcion que rellena la alarma dada por el index con los datos que recibe como parametros
 *
 */
void rellenarAlarma(uint32_t retardo_ms, EVENTO_T ID_evento, uint32_t auxData,int index,uint32_t bit31){

		alarmas[index].activa = 1;
		alarmas[index].auxData = auxData;
		alarmas[index].periodo = retardo_ms + drv_tiempo_actual_ms(); // Se guarda el tiempo en el que tendr? que hacer la interrupci?n 
		
		if(bit31){
			alarmas[index].periodica = 1;
			alarmas[index].retardo_ms = retardo_ms; // Si es periodica se guarda cada cuanto hace la interrupci?n
			
		}
		else{
			alarmas[index].periodica = 0;
		}
}

void svc_alarma_activar(uint32_t retardo_ms, EVENTO_T ID_evento, uint32_t auxData){
	
	if(retardo_ms != 0){
		uint32_t bit31 = retardo_ms >> 31;
		uint32_t retardoSinBit31 = retardo_ms & ~(1u << 31);
		
			
			//comprobar si hay alguna alarma con el evento id
				for(int i = 0; i < num_alarmas; i++){
					if(alarmas[i].ID_evento == ID_evento){
						//si la hay reescribirla
						rellenarAlarma(retardoSinBit31, ID_evento,  auxData, i, bit31);
						break; //Acaba el for ahorrar iteraciones
					}
				}
			
			//si hay menos de max alarmas, hacer una nueva
				if(num_alarmas < svc_ALARMAS_MAX){
						for(int i = 0; i < svc_ALARMAS_MAX; i++){
							
							if(alarmas[i].activa == 0){ //busca cual esta desactivada y la reescribe
								rellenarAlarma(retardoSinBit31, ID_evento,  auxData,i, bit31);
								num_alarmas++;
								break;	//Acaba el for ahorrar iteraciones
							}
						}
							
				}
				else{
					//se avisa del overflow
					drv_monitor_marcar(monitor_overflow);
					while(1){
					//bucle infinito
						//WDT
					
					}
				}
				
		}
		else{
		//se cancela la alarma
			for(int i = 0; i < num_alarmas; i++){
					if(alarmas[i].ID_evento == ID_evento){ // si existe la alarma
						
						alarmas[i].activa = 0;	// se desactiva
						break;	//Acaba el for ahorrar iteraciones
					}
			}
			
	}
}


void svc_alarma_tratar(EVENTO_T evento,  uint32_t aux){
	
	switch (evento) {
    case ev_T_PERIODICO:
			
        for(int i = 0; i < num_alarmas; i++){
					if(alarmas[i].periodo <= drv_tiempo_actual_ms() && alarmas[i].activa){
						
						funcion_callback(evento,aux); //Encola el evento
			
						if(alarmas[i].periodica == 0){ //GESTI?N DE PERIOCIDAD
							//si no es periodica se cancela
							alarmas[i].activa = 0;
							num_alarmas --;
							
						}
						else{ //Si es perodica se actualza el contador
							alarmas[i].periodo = alarmas[i].retardo_ms + drv_tiempo_actual_ms();
						}
					}
					
				}
				
        break;
		
    default:
        // C?digo para cualquier otro caso
        break;
	}
	
	
}

#include "scheduler.h"
#include "listx.h"
#include "types_rikaya.h"
#include "utils.h"
#include <umps/libumps.h>
#include "pcb.h"

pcb_t* current;
extern void log_process_order();
void scheduler(struct list_head* head){
	if (current!=NULL){
	current->priority = current->original_priority;/*se il processore ha un processo ne riaggiorniamo la priorità*/
	increment_pcbs_priority(head);/*effettuiamo l' aging su tutti gli altri processi*/
	insertProcQ(head,current);/*reinseriamo il processo corrente nella coda dei processi*/
	current=removeProcQ(head);/*scegliamo un altro processo da eseguire usando le funzioni di pcb.c*/
	log_process_order((int)(current->original_priority));
	setTIMER(3000);/*aggiorniamo il time-slice*/
	LDST(&(current->p_s));/*carichiamo il processo nel processore*/
		}
	else{
		/*se il processore non ha un processo significa che il processo prima ha terminato la sua esecuzione
		esattamente alla fine dell' ultimo time-slice quindi lo trattiamo come una nuova inizializzazione*/
		scheduler_init(head);
		}
}

void scheduler_init(struct list_head* head){
	/*questa funzione seleziona il processo con priorità maggiore e lo inserisce nel processore facendo l' aging degli altri nel frattempo*/
	current=removeProcQ(head);
	log_process_order((int)(current->original_priority));
	increment_pcbs_priority(head);
	setTIMER(3000);
	setSTATUS(getSTATUS()|1);/*impostiamo lo stato del processore*/
	LDST(&(current->p_s));
}

void increment_pcbs_priority(struct list_head* head){
	/*scorriamo la lista e incrementiamo la priorità di ogni processo presente (che quindi non è nel processore)*/
		struct list_head* scroll = head->next;
		list_for_each(scroll,head){
			container_of(scroll,pcb_t,p_next)->priority = container_of(scroll,pcb_t,p_next)->priority+1;
		}
}

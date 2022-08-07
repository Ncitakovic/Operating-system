/*
 * List.cpp
 *
 *  Created on: Apr 18, 2020
 *      Author: OS1
 */
#include "List.h"
#include "pcb.h"
List::List() {
	tail = 0;
	head = 0;
}


List::~List(){

	while(get());
	head = 0;
	tail = 0;
}



void List::add(PCB* pcb) {
	if (pcb == 0)
		return;
														//CHECK: da li treba stavljati u QUEUE( Listu) ako se PCB vec nalazi u QUEUE(Listi)
	if (tail == 0) {
		head = tail = new Elem(pcb);

	} else {
		tail->next = new Elem(pcb);
		tail = tail->next;

	}

}

PCB* List::get() {
	if (head == 0){
		return 0;
	}
	Elem* old = head;
	PCB* pcb;
	if(head!=0) pcb = head->pcb;
	if(head!=0) head = head->next;
	if(head == 0){
		tail = 0;
	}
	//old->pcb = 0;
	if(old != 0) old->next = 0;
	delete old;
	return pcb;

}

void List::releaseThreadsAndChangeStates(){
	PCB* pcb = this->get();

	while(pcb	!=	0){
		pcb->state = READY;
		Scheduler::put(pcb);
		pcb = this->get();

	}

}
Thread* List::getThread(int id){
	Elem* tek;
	for(tek = head;tek != 0;tek = tek->next){
		if(tek->pcb!=0)
			if(tek->pcb->myThread->getId() == id){
				return tek->pcb->myThread;
			}

	}

	return 0;

}

void List::withdraw(PCB* pcb){

	Elem* current = head;
	Elem* previous = 0;
	while(1){

		if(current == 0 || pcb == current->pcb) break;

		previous = current;
		current = current->next;
	}
	if(current == 0 ) return;

	Elem* toDelete = current;
	if(previous != 0){
		previous->next = current->next;
	}else{
		head = current->next;
	}
	if(current->next == 0){
		tail = previous;
	}
	if(toDelete!=0) delete toDelete;


}


/*
int List::empty(){
	return head==0;
}
*/


#include "Timer.h"
															//TIMER

pInterrupt Timer::oldISR = 0;
Time Timer::cnt = 0;

void tick();

void Timer::inic(){
	LOCK
	oldISR = GETVECT(8);
	SETVECT(8,&Timer::timerIntr);
	UNLOCK

}


void Timer::restore(){

	LOCK
	SETVECT(8,oldISR);
	UNLOCK

}

void interrupt Timer::timerIntr(...){

	if(Timer::oldISR){
		(*oldISR)();
	}

	Timer::cnt++;

	tick();
	PCB::sleepList->timerTick();

	if(PCB::idle == PCB::running) dispatch();
	if(PCB::running->timeSlice == 0) return;
	if(Timer::cnt >= PCB::running->timeSlice){
		dispatch();
	}

}

void resetCnt(){
	Timer::cnt = 0;
}


															//SLEEPLIST

SleepList::SleepList(){
	head = 0;
}

SleepList::~SleepList(){
	while(head){
		timerTick();
	}
}

void SleepList::releaseThreadsAndChangeStates(){}

void SleepList::add(PCB* pcb,Time remainingTime){

	if(remainingTime == 0) return;
	if(pcb == 0) return;

	pcb->inSleepList = 1;
	Elem* current = head;
	Elem* previous = 0;

	while(current != 0) {

		if(remainingTime <  current->remainingTime  ) break;

		remainingTime -= current->remainingTime;

		previous = current;
		current = current -> next;


	}
	Elem* novi = new Elem(pcb, remainingTime);
	if (previous == 0)
		head = novi;
	else
		previous->next = novi;
	novi->next = current;
	if(current != 0){ //if next element exists decrement remainingTime
		current -> remainingTime -= novi->remainingTime;
	}


}

PCB* SleepList::pop(){
	Elem* old = head;
	PCB* pcb=0;
	if(head != 0) pcb = head -> pcb;
	if(head != 0) head = head -> next;
	delete old;
	return pcb;
}

void SleepList::withdraw(PCB* pcb){
	Elem* current = head;
	Elem* previous = 0;
	while(current != 0){

		if(pcb == current->pcb) break;

		previous = current;
		current = current->next;


	}

	if(current == 0) return;
	pcb->inSleepList = 0;
	Elem* old = current;
	current = current->next;
	if(previous != 0)
		previous->next = current;
	else head = current;
	if(current != 0)
		current->remainingTime += old->remainingTime;
	delete old;

}


void SleepList::tickTimeForAllThreads() {
	if (head != 0 && head->remainingTime > 0)
		--head->remainingTime;
}

void SleepList::timerTick(){

	// tick timeUnit for all threads

	tickTimeForAllThreads();

	//wake up all threads that reached their thread time (remainingTime ==0)

	while (head != 0) {
		if (head->remainingTime > 0)
			break;
		else wakeUp(pop());
	}

}

void SleepList::wakeUp(PCB* pcb){
	pcb->state = READY;
	pcb->semWaitReturnValue = 0;
	pcb -> sem ->blocked ->withdraw(pcb);
	pcb -> sem ->val++;
	pcb -> sem = 0;
	pcb->inSleepList = 0;
	Scheduler::put(pcb);

}


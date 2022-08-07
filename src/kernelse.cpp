/*
 * kernelse.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: OS1
 */

#include "kernelse.h"


KernelSem::KernelSem(int init){
	if(init<0) val = 0;
	else val = init;
	blocked = new List();
}

KernelSem::~KernelSem(){
	while(val<0){
		signal(0);
	}
	delete blocked;
}


int KernelSem::wait(Time maxTimeToWait){

	PCB::running->semWaitReturnValue = 1;
	if(--val<0){
		PCB::running->sem = this;								//CHECK: order
		if(maxTimeToWait!=0){
			PCB::sleepList->add(PCB::running,maxTimeToWait);		//<-
		}
		block();
	}
	return PCB::running->semWaitReturnValue;
}


int KernelSem::signal(int n){
	if (n < 0)
		return n;
	if (n == 0) {
		if (val++ < 0) {
			deblock();
		}
		return 0;											//CHECK: return value
	}
	// n > 0:
	if (val >= 0) {
		val += n;
		return 0;
	} else if (n > -val) {
		int x = -val;						//CHECK :is ret val correct
		for (int i = 0; i < -val; i++) {
			deblock();
		}
		val += n;
		return x;
	} else {						//if(n <= -value)
		for (int i = 0; i < n; i++) {
			deblock();
		}
		val += n;
		return n;
	}



}

int KernelSem::value() const{
	return val;
}


void KernelSem::block(){
	PCB::running->state = BLOCKED;
	blocked->add(PCB::running);
	dispatch();
}

void KernelSem::deblock(){
	PCB* pcb = blocked->get();
	if(pcb!=0){
		if(pcb->inSleepList == 1){
			PCB::sleepList->withdraw(pcb);
		}
		pcb->semWaitReturnValue = 1;
		pcb->state = READY;
		pcb -> sem = 0;
		Scheduler::put(pcb);

	}
}

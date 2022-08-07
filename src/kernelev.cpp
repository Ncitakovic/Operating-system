/*
 * kernelev.cpp
 *
 *  Created on: Jul 9, 2020
 *      Author: OS1
 */
#include "pcb.h"
#include "kernelev.h"
#include "event.h"
const int WAS_SIGNALLED = 1;
const int THREAD_NOT_BLOCKED = 0;
const int THREAD_BLOCKED = -1;

KernelEv::KernelEv(IVTNo ivtNo){
	this->ivtNo = ivtNo;
	this->pcb = PCB::running;
	this->val = 0;


	if(arrayOfEntries[ivtNo])
		if(arrayOfEntries[ivtNo]->kernelEv== 0){
			arrayOfEntries[ivtNo]->kernelEv = this;
		}

}

KernelEv::~KernelEv(){

	if(arrayOfEntries[ivtNo])
			if(arrayOfEntries[ivtNo]->kernelEv== this){
				arrayOfEntries[ivtNo]->kernelEv = 0;
			}


}

void KernelEv::signal(){
	
	if(val==THREAD_BLOCKED){
		val = THREAD_NOT_BLOCKED;
		pcb->state = READY;
		Scheduler::put(pcb);
	}else if(val==THREAD_NOT_BLOCKED){
		val = WAS_SIGNALLED;
	}

}

void KernelEv::wait(){
	if(PCB::running != this->pcb) return;	//sklonimo ovo za 1

	if (val == THREAD_NOT_BLOCKED) {
		val = THREAD_BLOCKED;
		PCB::running->state = BLOCKED;
		dispatch();
	} else if (val == WAS_SIGNALLED) {
		val = THREAD_NOT_BLOCKED;
	}

}


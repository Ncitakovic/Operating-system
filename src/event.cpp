/*
 * event.cpp
 *
 *  Created on: Jul 9, 2020
 *      Author: OS1
 */
#include "event.h"
#include "lock.h"
#include "kernelev.h"


Event::Event(IVTNo ivtNo){
	LOCK
	myImpl = new KernelEv(ivtNo);
	UNLOCK
}

Event::~Event(){
	LOCK
	delete myImpl;
	UNLOCK

}

void Event::wait(){
	LOCK
	myImpl->wait();
	UNLOCK
}

void Event::signal(){
	LOCK
	myImpl->signal();
	UNLOCK
}


//							IVTEntry

IVTEntry* arrayOfEntries[256] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo,pInterrupt newINTR,int callPrevious){
	LOCK
	this->kernelEv = 0;
	this->ivtNo = ivtNo;
	this->callPrevious = callPrevious;
	arrayOfEntries[ivtNo] = this;
	oldINTR = GETVECT(ivtNo);
	SETVECT(ivtNo,newINTR);
	UNLOCK
}

IVTEntry::~IVTEntry(){
	LOCK
	SETVECT(ivtNo,oldINTR);
	arrayOfEntries[ivtNo] = 0;
	if(oldINTR != 0) (*oldINTR)();
	UNLOCK
}

void IVTEntry::signal(){
	// call previous interrupt routine if needed
	if(oldINTR)
		if(this->callPrevious == 1){
			(*oldINTR)();
		}
	if(this->kernelEv != 0) kernelEv->signal();

	dispatch();
}





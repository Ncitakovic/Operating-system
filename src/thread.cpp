/*
 * thread.cpp
 *
 *  Created on: Apr 8, 2020
 *      Author: OS1
 */
#include "thread.h"
#include "PCB.h"

#include "List.h"
#include "lock.h"
class List;


Thread::Thread(StackSize stackSize  , Time timeSlice ){
	LOCK
	myPCB = new PCB(stackSize, timeSlice, this);
	UNLOCK
}

Thread::~Thread() {
	LOCK
	waitToComplete();
	delete myPCB;
	UNLOCK
}

void Thread::start() {
	LOCK
	if(myPCB->state == NEW){

		myPCB->state = READY;


		//if(this->myPCB != PCB::idle){
			PCB::listOfAllThreads->add(this->myPCB);
		//}
		myPCB->initialContext();
		if(this->myPCB != PCB::idle){
			Scheduler::put(myPCB);
		}


	}
	UNLOCK
}

void dispatch() {
	LOCK
	dispatchIntr();
	UNLOCK
}

void Thread::waitToComplete(){
	LOCK
	if(this->myPCB == PCB::running){
		UNLOCK
		return;
	}
	if(this->myPCB->state==FINISHED  ||   this->myPCB->state==NEW) {
		UNLOCK
		return;
	}
	PCB::running->state = BLOCKED;
	myPCB->blockedThreads->add(PCB::running);
	dispatch();

	UNLOCK
}


ID Thread::getId(){
	LOCK
	int id;
	id = myPCB->id;
	UNLOCK
	return id;
}

ID Thread::getRunningId(){
	LOCK
	int id;
	id = PCB::running->id;
	UNLOCK
	return id;
}

Thread * getThreadById(ID id){
	LOCK
	Thread* t = PCB::listOfAllThreads->getThread(id);
	UNLOCK
	return t;

}






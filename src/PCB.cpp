/*
 * PCB.cpp
 *
 *  Created on: Apr 8, 2020
 *      Author: OS1
 */
#include "PCB.h"
#include"List.h"
#include "Timer.h"
#include"lock.h"
//#include<iostream.h>

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* myThread) {

	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	this->myThread = myThread;
	ss = 0;
	sp = 0;
	bp = 0;
	stack = 0;
	state = NEW;
	blockedThreads = new List();
	id = PCB::nextId++;
	semWaitReturnValue = 0;
	sem = 0;
	inSleepList = 0;
	listOfAllThreads->add(this);
}

PCB::~PCB() {
	listOfAllThreads->withdraw(this);
	if(blockedThreads!=0) delete blockedThreads;
	if(stack!=0) delete[] stack;
}

PCB* PCB::running=0;
ID PCB::nextId = 0;
List* PCB::listOfAllThreads = 0;
SleepList* PCB::sleepList = 0;
PCB* PCB::idle = 0;
volatile unsigned tss, tsp, tbp;

void interrupt dispatchIntr() {
	//push PSW,CS,IP
	// push registers : AX,BC,CX,DX,ES,DS,SI,DI,BP (9 in total)

	asm {
		mov tsp, sp
		mov tss, ss
		mov tbp, bp
	}

	PCB::running->sp = tsp;
	PCB::running->ss = tss;
	PCB::running->bp = tbp;

	if(PCB::running->state == READY && PCB::running != PCB::idle){
		Scheduler::put (PCB::running);				//put old Thread to the Scheduler
	}
	PCB::running = Scheduler::get();//get next Thread
	if(PCB::running == 0) PCB::running = PCB::idle;

	tsp = PCB::running->sp;
	tss = PCB::running->ss;
	tbp = PCB::running->bp;

	asm {
		mov sp, tsp
		mov bp, tbp
		mov ss, tss


	}

	//Timer::cnt=0;
	resetCnt();

//pop registers (9 in total)
//IRET
}

void PCB::wrapper() {

	ASSERT(PCB::running != 0);
	if(PCB::running->myThread!=0){
		PCB::running->myThread->run();
	}
/*
	while(!PCB::running->blockedThreads->empty()){
		PCB* p = PCB::running->blockedThreads->get();
		p->state = READY;
		Scheduler::put(p);
	}
*/
	//LOCK
	PCB::running->state=FINISHED;
	PCB::running->blockedThreads->releaseThreadsAndChangeStates();
	dispatch();


}

void PCB::initialContext() {

	stack = new unsigned char[stackSize];
	StackSize size = stackSize/ sizeof(unsigned);
	unsigned* stackP = (unsigned*)stack;



	stackP[size - 1] = 0x200;

	#ifndef BCC_BLOCK_IGNORE
	stackP[size-2] = FP_SEG(&PCB::wrapper);
	stackP[size-3] = FP_OFF(&PCB::wrapper);

	this->sp=FP_OFF(stackP+size-12);
	this->ss=FP_SEG(stackP+size-12);
	this->bp=FP_OFF(stackP+size-12);
	#endif

}


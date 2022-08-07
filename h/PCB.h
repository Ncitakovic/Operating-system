/*
 * PCB.h
 *
 *  Created on: Apr 8, 2020
 *      Author: OS1
 */

#ifndef H_PCB_H_
#define H_PCB_H_

#include "thread.h"



const int NEW = 1;
const int READY = 2;
const int BLOCKED = 3;
const int FINISHED = 4;

class List;

class PCB{

public:
	PCB(StackSize stackSize, Time timeSlice,Thread* myThread);
	virtual ~PCB();

	static void PCB::wrapper();



	void initialContext();

	friend int userMain(int argc, char* argv[]);
	friend int main (int argc, char* argv[]);
	friend void interrupt dispatchIntr();
	friend class List;
	friend class Thread;
	friend class KernelSem;
	friend class SleepList;
	friend void interrupt timerIntr(...);




	static PCB* running;
	int state;
	KernelSem* sem;
	List* blockedThreads;
	static ID nextId;
	static SleepList* sleepList;
	static List* listOfAllThreads;
	static PCB* idle;
private:
	ID id;
	StackSize stackSize;
	Time timeSlice;
	unsigned char* stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;

	Thread* myThread;

	friend class Timer;
	int inSleepList;
	int semWaitReturnValue;

};




void interrupt dispatchIntr();



#endif /* H_PCB_H_ */

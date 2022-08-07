

#include "PCB.h"
#include"List.h"
#include<iostream.h>
#include "Timer.h"
#include "idle.h"
int userMain (int argc, char* argv[]);

int main (int argc, char* argv[]){

	LOCK
	PCB::listOfAllThreads = new List();
	PCB::sleepList = new SleepList();
	Thread* t = new Thread(0);
	Thread* idle = new Idle();
	PCB::idle = idle->myPCB;
	idle->start();
	PCB::running = t->myPCB;
	PCB::running->state = READY;

	Timer::inic();
	UNLOCK

	int res = userMain(argc , argv);

	LOCK
	Timer::restore();


	delete idle;
	delete t;
	PCB::idle = 0;
	PCB::running = 0;
	delete PCB::listOfAllThreads;
	delete PCB::sleepList;
	UNLOCK
	return res;

}


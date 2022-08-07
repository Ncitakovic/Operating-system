/*
 * Timer.h
 *
 *  Created on: Apr 21, 2020
 *      Author: OS1
 */

#ifndef H_TIMER_H_
#define H_TIMER_H_

#include "kernelse.h"
#include "pcb.h"
typedef void interrupt (*pInterrupt)(...);
#include"lock.h"

typedef unsigned int Time;
class SleepList{
public:
		SleepList();

		virtual ~SleepList();

		virtual void add(PCB*,Time remainingTime);

		virtual void withdraw(PCB* pcb);

		virtual PCB* pop();
		//Thread* getThread(int id);

		virtual void tickTimeForAllThreads();
		virtual void wakeUp(PCB*);
		class Elem {
		 public:
			PCB* pcb;
			Elem* next;
			Time remainingTime;
			Elem(PCB* pcb,Time time) {
				this->pcb = pcb;
				this->remainingTime = time;
				next = 0;
			}
		};
		virtual void releaseThreadsAndChangeStates();
		friend class Thread;
		friend class KernelSem;
		void timerTick();
private:
	Elem* head;


};




class Timer{

public:
	static void inic();

	static void restore();

	static void interrupt timerIntr(...);

	static void resetCnt();

private:

	static pInterrupt oldISR;

	static Time cnt;


	friend void interrupt dispatchIntr();
	friend void resetCnt();

};

void resetCnt();




#endif /* H_TIMER_H_ */

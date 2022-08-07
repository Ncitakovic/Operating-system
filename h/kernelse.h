/*
 * kernelse.h
 *
 *  Created on: Apr 22, 2020
 *      Author: OS1
 */

#ifndef H_KERNELSE_H_
#define H_KERNELSE_H_
#include "thread.h"
#include "List.h"
#include "PCB.h"
#include "Timer.h"


class KernelSem{

public:

	KernelSem(int init);

	virtual ~KernelSem();

	virtual int wait(Time maxTimeToWait);

	virtual int signal(int n = 0);

	int value() const;

	virtual void block();
	virtual void deblock();

	friend class SleepList;

private:
	int val;
	List* blocked;


};



#endif /* H_KERNELSE_H_ */

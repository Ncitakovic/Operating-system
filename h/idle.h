/*
 * idle.h
 *
 *  Created on: Jul 9, 2020
 *      Author: OS1
 */

#ifndef H_IDLE_H_
#define H_IDLE_H_

#include "thread.h"






class Idle:Thread{
public:
	Idle(StackSize sz = 512 , Time t=5);
	virtual ~Idle();

	virtual void run();
private:
	int redo;
};




#endif /* H_IDLE_H_ */

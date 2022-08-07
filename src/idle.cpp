/*
 * idle.cpp
 *
 *  Created on: Jul 9, 2020
 *      Author: OS1
 */

#include "idle.h"


Idle::Idle(StackSize sz,Time t): Thread(sz,t){redo = 1;}

Idle::~Idle(){
	redo = 0;
	waitToComplete();
}

void Idle::run(){
	while(redo==1) dispatch();
}




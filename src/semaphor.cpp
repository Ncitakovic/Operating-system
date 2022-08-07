/*
 * semaphor.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: OS1
 */
#include"semaphor.h"
#include"kernelse.h"
#include "lock.h"

Semaphore::Semaphore(int init){
	LOCK
	myImpl = new KernelSem(init);
	UNLOCK

}

Semaphore::~Semaphore(){
	LOCK
	if(myImpl!=0) delete myImpl;
	UNLOCK
}

int Semaphore::wait(Time maxTimeToWait){
	LOCK
	int res;
	res = myImpl -> wait(maxTimeToWait);
	UNLOCK
	return res;
}

int Semaphore::signal(int n){
	LOCK
	int res;
	res = myImpl->signal(n);
	UNLOCK
	return res;
}

int Semaphore::val() const{
	LOCK
	int res;
	res = myImpl->value();
	UNLOCK
	return res;
}


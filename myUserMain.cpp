/*
 * myUserMain.cpp
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#include <iostream.h>
#include "PCB.h"
#include "event.h"
#include "semaphor.h"
#include "stdlib.h"

PREPAREENTRY(9,1);

void tick(){}
Semaphore* mutex = 0;
Semaphore* randomMutex = 0;
Semaphore* sleepSem = 0;


class KeyBoard : public Thread{
private:
	int n;

public:
	KeyBoard(int n){this->n=n;}

	void run(){

	Event event(9);
	for (int i = 0; i < n; ++i) {

		mutex->wait(0);
		cout << endl <<" waiting for key"<< (i+1)<<endl;
		mutex->signal();

		event.wait();

		mutex->wait(0);
		cout << endl <<" key continue"<< (i+1)<<endl;
		mutex->signal();
	}
		mutex->wait(0);
		cout << endl <<" key finished successfully!!!"<< endl;
		mutex->signal();


	}
	virtual ~KeyBoard(){waitToComplete();}

};


class Character: public Thread {
private:
	char c;
	int n;
public:
	Character(char cc, int nn) {
		c = cc;
		n = nn;
	}
	virtual ~Character(){
		waitToComplete();
	}

	void run() {
		//for(int j=0;j<9999;j++);
		//for(int i=0;i<20;i++) {	cout<< 'a'+i<<"  in run(A)  ";
		for (long x = 0; x < n ; x++) {
			mutex->wait(0);
			cout << c;
			mutex->signal(1);
		//	for (int j = 0; j < 10000; j++)
			//	for (int k = 0; k < 10000; k++);
			//randomMutex->wait(0);
			//Time timeToSleep = 1+rand()%4;
			sleepSem->wait(5);
			//randomMutex->signal();
			//dispatch();

		}
		mutex->wait(0);
		cout<<endl<<c<<" FINISHED "<<endl;
		mutex->signal(1);

	}

};




int userMain(int argc, char* argv[]) {

	mutex = new Semaphore(1);
	randomMutex = new Semaphore(1);
	sleepSem = new Semaphore(0);
	int max = 5;
	Character** characters = new Character* [max];
	KeyBoard* key = new KeyBoard(100);
	int i;

	for (i = 0; i < max; i++) {
		characters[i] = new Character('a' + i, 20 + 4 * i);
	}
	for (i = 0; i < max; i++) {
		characters[i]->start();
	}
	key->start();
	//mutex->wait(0);
	//cout<<PCB::listOfAllThreads->getThread(1)->myPCB->id;
	//mutex->signal();
	//while(1){ dispatch();}

	for(i=0;i<max;i++){
		delete characters[i];
	}


	delete[] characters;
	delete sleepSem;
	delete mutex;
	delete key;

	//cout << endl << "USER FINISHED!!!!" << endl;
	return 0;

}

/*
 cout<<"BBBBBBBBBBBBBBBBB"<<endl<<"YYYYYYYYYYYYYYYYY"<<endl;
 Thread* a = new A();
 Thread* b = new B();

 a->start();
 b->start();

 for(int l=0;l<32000;l++);
 for(int o=0;o<32000;o++);
 int r=4;
 while(r--)
 dispatch();






 */


/*
 class B : public Thread {
 public:

 B(){
 cout<<"u BBBBB"<<endl;
 }

 void run(){
 for(int j=0;j<9999;j++);
 for(int i=0;i<20;i++) {	cout<< 'a'+i<<"  in run(B)  ";

 if(i==4) dispatch();
 }
 cout<<endl;
 //	PCB::running->finished=1;
 //	dispatch();

 }



 };
 */

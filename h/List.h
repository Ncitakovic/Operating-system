/*
 * List.h
 *
 *  Created on: Apr 18, 2020
 *      Author: OS1
 */

#ifndef H_LIST_H_
#define H_LIST_H_

class PCB;
class Thread;
class List {

public:

	List();

	virtual ~List();

	virtual void add(PCB*);

	virtual PCB* get();


	Thread* getThread(int id);
	void withdraw(PCB*);

	class Elem {
	 public:
		PCB* pcb;
		Elem* next;
		Elem(PCB* pcb) {
			this->pcb = pcb;
			next = 0;
		}
	};

	virtual void releaseThreadsAndChangeStates();
	friend class Thread;
	//int empty();
private:
	Elem* head;
	Elem* tail;


};





#endif /* H_LIST_H_ */

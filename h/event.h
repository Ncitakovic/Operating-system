
#ifndef _event_h_
#define _event_h_

#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, &inter##numEntry,callOld); \
void interrupt inter##numEntry(...) {\
 newEntry##numEntry.signal();\
}




typedef void interrupt (*pInterrupt)(...);

typedef unsigned char IVTNo;
class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait  ();

protected:
	friend class KernelEv;
	void signal();

private:
	KernelEv* myImpl;
};



//						IVTEntry
class IVTEntry;
extern IVTEntry* arrayOfEntries[256];

class IVTEntry{
public:
	virtual void signal();

	IVTEntry(IVTNo ivtNo, pInterrupt newINTR, int callPrevious);
	virtual ~IVTEntry();
	friend class KernelEv;
private:
	//static IVTEntry* arrayOfEntries[256];
	int callPrevious;
	KernelEv* kernelEv;
	pInterrupt oldINTR;
	IVTNo ivtNo;
};



#endif

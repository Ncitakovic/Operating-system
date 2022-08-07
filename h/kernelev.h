

#ifndef H_KERNELEV_H_
#define H_KERNELEV_H_
#include "pcb.h"
typedef unsigned char IVTNo;
class KernelEv{
public:

	KernelEv(IVTNo ivtNo);
	virtual ~KernelEv();
	virtual void signal();
	virtual void wait();
private:
	int val;
	IVTNo ivtNo;
	PCB* pcb;

};



#endif /* H_KERNELEV_H_ */

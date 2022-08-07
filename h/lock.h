/*
 * lock.h
 *
 *  Created on: Apr 22, 2020
 *      Author: OS1
 */

#ifndef H_LOCK_H_
#define H_LOCK_H_

#ifndef BCC_BLOCK_IGNORE
#include <dos.h>
#include <assert.h>
#define LOCK asm {pushf;cli;}
#define UNLOCK asm {popf;}
#define ASSERT(value) assert(value)
#define GETVECT(intNo) getvect(intNo)
#define SETVECT(intNo,newInterrupt) setvect(intNo,newInterrupt)
#else
#define LOCK
#define UNLOCK
#define ASSERT(value)
#define GETVECT(intNo) ((pInterrupt)0)
#define SETVECT(intNo,newInterrupt)
#endif


#endif /* H_LOCK_H_ */

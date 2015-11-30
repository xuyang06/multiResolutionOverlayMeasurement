/*
 * mbcommon.h
 *
 *  Created on: Apr 9, 2014
 *      Author: xuyang06
 */

#ifndef MBCOMMON_H_
#define MBCOMMON_H_

#include <stdio.h>

typedef int    (*MBcmpfn)(const void *, const void *);
typedef void   (*MBforfn)(void *);
typedef void   (*MBdeletefn)(void *);
typedef void * (*MBgetfn)(void *);
typedef void   (*MBprintfn)(const void *, FILE *);


#endif /* MBCOMMON_H_ */

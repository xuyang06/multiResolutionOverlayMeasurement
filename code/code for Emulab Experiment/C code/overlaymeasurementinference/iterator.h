/*
 * iterator.h
 *
 *  Created on: Apr 9, 2014
 *      Author: xuyang06
 */

#ifndef ITERATOR_H_
#define ITERATOR_H_

#include "mbcommon.h"

struct MBiterator {
    void *      body;
    MBgetfn     getfn;
    MBdeletefn  deletefn;
};
typedef struct MBiterator MBiterator;

MBiterator * MBiterator_create(void *body, MBgetfn getfn, MBdeletefn deletefn);
void         MBiterator_delete(MBiterator *it);
void *       MBiterator_get(MBiterator *it);




#endif /* ITERATOR_H_ */

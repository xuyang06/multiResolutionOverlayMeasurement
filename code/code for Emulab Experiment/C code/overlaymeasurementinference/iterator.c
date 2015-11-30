/*
 * iterator.c
 *
 *  Created on: Apr 9, 2014
 *      Author: xuyang06
 */


#include <stdlib.h>

#include "iterator.h"

MBiterator *MBiterator_create(void *body, MBgetfn getfn, MBdeletefn deletefn)
{
    MBiterator *it = malloc(sizeof(MBiterator));
    if (it) {
        it->body = body;
        it->getfn = getfn;
        it->deletefn = deletefn;
    }

    return it;
}

void MBiterator_delete(MBiterator *it)
{
    if (it) {
        if (it->deletefn) {
            it->deletefn(it->body);
        }
        free(it);
    }
}

void *MBiterator_get(MBiterator *it)
{
    return it->getfn(it->body);
}


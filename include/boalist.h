#ifndef __H_BOALIST__
#define __H_BOALIST__

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "object.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    BoaObject base;
    bint_t capacity, length;
    BoaObject **value;
} BoaList;

BoaList *malloc_BoaList(bint_t capacity);
BoaObject *add_BoaList(BoaObject *this, BoaObject *other);
char *tostring_BoaList(BoaObject *this);
void free_BoaList(BoaList *o);

#ifdef __cplusplus
}
#endif

#endif /* __H_BOALIST__ */

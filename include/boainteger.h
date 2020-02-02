#ifndef __H_BOAINTEGER__
#define __H_BOAINTEGER__

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
    bint_t value;
} BoaInteger;

BoaInteger *malloc_BoaInteger(bint_t value);
bint_t getvalue_BoaInteger(BoaInteger *o);
BoaObject *add_BoaInteger(BoaObject *this, BoaObject *other);
char *tostring_BoaInteger(BoaObject *this);
void free_BoaInteger(BoaInteger *o);

#ifdef __cplusplus
}
#endif

#endif /* __H_BOAINTEGER__ */

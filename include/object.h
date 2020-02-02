#ifndef __H_OBJECT__
#define __H_OBJECT__

#include <stdlib.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BOA_CAST_TO_OBJ(o) ((BoaObject *)(o))
#define BOA_CAST_OBJ_TO(o, type) ((type *)(o))
#define INC_REF_COUNT(o) (++(BOA_CAST_TO_OBJ(o))->ref_count)
#define DEC_REF_COUNT(o) (--(BOA_CAST_TO_OBJ(o))->ref_count)
#define GET_REF_COUNT(o) ((BOA_CAST_TO_OBJ(o))->ref_count)

typedef long long bint_t;

typedef struct {
    size_t ref_count;
} BoaObject;

typedef struct {
    BoaObject base;
    bint_t value;
} BoaInteger;

typedef struct {
    BoaObject base;
    bint_t capacity, length;
    BoaObject **value;
} BoaList;

BoaInteger *malloc_BoaInteger(bint_t value);
bint_t getvalue_BoaInteger(BoaInteger *o);
void free_BoaInteger(BoaInteger *o);

BoaList *malloc_BoaList(bint_t capacity);
void free_BoaList(BoaList *o);

#define add_BoaList(o, v) _add_BoaList(o, BOA_CAST_TO_OBJ(v))
void _add_BoaList(BoaList *o, BoaObject *val);

#ifdef __cplusplus
}
#endif

#endif /* __H_OBJECT__ */

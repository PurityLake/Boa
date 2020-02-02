#ifndef __H_OBJECT__
#define __H_OBJECT__

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "gc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BOA_OBJ(o) ((BoaObject *)(o))
#define BOA_OBJ_TO(o, type) ((type *)(o))
#define INC_REF_COUNT(o) (++(BOA_OBJ(o))->ref_count)
#define DEC_REF_COUNT(o) (--(BOA_OBJ(o))->ref_count)
#define GET_REF_COUNT(o) ((BOA_OBJ(o))->ref_count)

#define EXPOSE_FUNC(o) ((BOA_OBJ(o))->impl)
#define ADD_FUNC(this, other) (EXPOSE_FUNC(this)->add(BOA_OBJ(this), BOA_OBJ(other)))
#define STR_FUNC(this) (EXPOSE_FUNC(this)->to_string(BOA_OBJ(this)))

typedef long long bint_t;

typedef struct _boaobject BoaObject;

typedef BoaObject* (*binary_func)(BoaObject *, BoaObject *);
typedef char* (*unary_func)(BoaObject *);

typedef struct {
    binary_func add;
    unary_func to_string;
} BoaObjectImpl;

struct _boaobject {
    size_t ref_count;
    BoaObjectImpl *impl;
};

#ifdef __cplusplus
}
#endif

#endif /* __H_OBJECT__ */

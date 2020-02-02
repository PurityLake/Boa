#include "object.h"

#ifdef __cplusplus
extern "C" {
#endif

BoaInteger *malloc_BoaInteger(bint_t value) {
    BoaObject obj = { 0 };
    BoaInteger *out = (BoaInteger *)malloc(sizeof(BoaInteger));
    out->base = obj;
    out->value = value;
    INC_REF_COUNT(out);
    return out;
}

bint_t getvalue_BoaInteger(BoaInteger *o) {
    return o->value;
}

void free_BoaInteger(BoaInteger *o) {
    DEC_REF_COUNT(o);
    free(o);
    o = NULL;
}

BoaList *malloc_BoaList(bint_t capacity) {
    BoaObject obj = { 0 };
    BoaList *out = (BoaList *)malloc(sizeof(BoaList));
    out->base = obj;
    out->capacity = capacity;
    out->length = 0;
    BoaObject **value = (BoaObject **)calloc(sizeof(BoaObject*), capacity);
    for (int i = 0; i < capacity; ++i) {
        value[i] = NULL;
    }
    out->value = value;
    INC_REF_COUNT(out);
    return out;
}

void free_BoaList(BoaList *o) {
    DEC_REF_COUNT(o);
    BoaObject **temp = o->value;
    while (*temp != NULL) {
        DEC_REF_COUNT(*temp);
        ++temp;
    }
    free(o->value);
    free(o);
    o = NULL;
}

void _add_BoaList(BoaList *o, BoaObject *val) {
    if (o->length == o->capacity) {
        o->value = (BoaObject **)realloc(o->value, sizeof(BoaObject *) * (o->capacity * 2));
        o->capacity *= 2;
        for (int i = o->length; i < o->capacity; ++i) {
            o->value[i] = NULL;
        }
    }
    BoaObject **temp = o->value;
    while (*temp != NULL) {
        ++temp;
    }
    *temp = val;
    ++o->length;
    INC_REF_COUNT(val);
}

#ifdef __cplusplus
}
#endif

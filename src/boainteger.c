#include "object/boainteger.h"

#ifdef __cplusplus
extern "C" {
#endif

static const char *class_name = "Integer";

static BoaObjectImpl BoaIntegerObjectImpl = {
    "Integer",
    add_BoaInteger,
    tostring_BoaInteger,
    compare_BoaInteger
};

BoaInteger *create_BoaInteger(bint_t value) {
    BoaInteger *fromGC = (BoaInteger *)ison_GC((void *)&value);
    if (fromGC == NULL) {
        BoaObject obj = { 0, &BoaIntegerObjectImpl };
        BoaInteger *out = (BoaInteger *)malloc_GC(sizeof(BoaInteger));
        out->base = obj;
        out->value = value;
        INC_REF_COUNT(out);
        return out;
    }
    INC_REF_COUNT(fromGC);
    return fromGC;
}

bint_t getvalue_BoaInteger(BoaInteger *o) {
    return o->value;
}

BoaObject *add_BoaInteger(BoaObject *this, BoaObject *other) {
    BoaInteger *_this = BOA_OBJ_TO(this, BoaInteger);
    BoaInteger *_other = BOA_OBJ_TO(other, BoaInteger);

    bint_t thisvalue = getvalue_BoaInteger(_this);
    bint_t othervalue = getvalue_BoaInteger(_other);

    return BOA_OBJ(create_BoaInteger(thisvalue + othervalue));
}

char *tostring_BoaInteger(BoaObject *this) {
    BoaInteger *_this = BOA_OBJ_TO(this, BoaInteger);
    char *value = (char *)malloc(sizeof(32));
    sprintf(value, "%lld", getvalue_BoaInteger(_this));
    value = (char *)realloc(value, strlen(value));
    return value;
}

int compare_BoaInteger(BoaObject *this, void *other) {
    BoaInteger *_this = (BoaInteger *)this;
    int *otherprim = (int *)other;
    return getvalue_BoaInteger(_this) == *otherprim;
}

void destroy_BoaInteger(BoaInteger *o) {
    DEC_REF_COUNT(o);
}

#ifdef __cplusplus
}
#endif

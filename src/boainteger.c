#include "boainteger.h"

#ifdef __cplusplus
extern "C" {
#endif

static BoaObjectImpl BoaIntegerObjectImpl = {
    add_BoaInteger,
    tostring_BoaInteger
};

BoaInteger *create_BoaInteger(bint_t value) {
    BoaObject obj = { 0, &BoaIntegerObjectImpl };
    BoaInteger *out = (BoaInteger *)malloc_GC(sizeof(BoaInteger));
    out->base = obj;
    out->value = value;
    INC_REF_COUNT(out);
    return out;
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

void destroy_BoaInteger(BoaInteger *o) {
    DEC_REF_COUNT(o);
}

#ifdef __cplusplus
}
#endif

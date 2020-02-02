#include "object/boalist.h"

#ifdef __cplusplus
extern "C" {
#endif

static const char *class_name = "List";

static BoaObjectImpl BoaListObjectImpl = {
    "List",
    add_BoaList,
    tostring_BoaList,
    compare_BoaList
};

BoaList *create_BoaList(bint_t capacity) {
    BoaObject obj = { 0, &BoaListObjectImpl };
    BoaList *out = (BoaList *)malloc_GC(sizeof(BoaList));
    out->base = obj;
    out->capacity = capacity;
    out->length = 0;
    BoaObject **value = (BoaObject **)calloc(capacity, sizeof(BoaObject*));
    for (int i = 0; i < capacity; ++i) {
        value[i] = NULL;
    }
    out->value = value;
    INC_REF_COUNT(out);
    return out;
}

void destroy_BoaList(BoaList *o) {
    DEC_REF_COUNT(o);
    BoaObject **temp = o->value;
    while (*temp != NULL) {
        DEC_REF_COUNT(*temp);
        ++temp;
    }
    free(o->value);
}

BoaObject *add_BoaList(BoaObject *this, BoaObject *other) {
    BoaList *_this = BOA_OBJ_TO(this, BoaList);
    if (_this->length == _this->capacity) {
        _this->value = (BoaObject **)realloc(_this->value, sizeof(BoaObject *) * (_this->capacity * 2));
        _this->capacity *= 2;
        for (int i = _this->length; i < _this->capacity; ++i) {
            _this->value[i] = NULL;
        }
    }
    BoaObject **temp = _this->value;
    while (*temp != NULL) {
        ++temp;
    }
    *temp = other;
    ++_this->length;
    INC_REF_COUNT(other);

    return this;
}

int compare_BoaList(BoaObject *this, void *other) {
    return 0;
}

char *tostring_BoaList(BoaObject *this) {
    BoaList *_this = BOA_OBJ_TO(this, BoaList);
    BoaObject **temp = _this->value;
    char *value = (char *)malloc(100);
    char *iter = value;
    memset(value, '\0', 100);
    if (*temp != NULL) {
        int len = sprintf(iter, "[ %s",  STR_FUNC(*temp));
        iter += len;
        ++temp;
        while (*temp != NULL) {
            len = sprintf(iter, ", %s", STR_FUNC(*temp));
            iter += len;
            ++temp;
        }
        sprintf(iter, " ]");
    } else {
        sprintf(value, "[]");
    }
    value = (char *)realloc(value, strlen(value));
    return value;
}

#ifdef __cplusplus
}
#endif

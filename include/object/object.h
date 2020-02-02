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

// default size of BoaInteger
typedef long long bint_t;

// forward dec
typedef struct _boaobject BoaObject;

// compare to primitives
typedef int (*compare_method)(BoaObject *, void *);
// compare to other BoaObjects
typedef int (*rich_compare_method)(BoadObject *, BoadObject *)
// method that takes two operands
typedef BoaObject* (*binary_method)(BoaObject *, BoaObject *);
// method that take one operand
typedef char* (*unary_method)(BoaObject *);

/* BoaObjectImpl
 * =================================================================================
 * This struct contains compile time information about the primitive data types that
 * are defined by the Boa standard library.
 * class_name -> used for rich comparisons that determine if casted objects contain
 *               the same type of data
 * add -> a method that takes a reference to the current object and another object
 *        reference and attempts to perform a relative addition operation.
 * to_string -> a method that converts the current object to a string format
 *              to be printed to screen
 * =================================================================================
 * TODO: Add more generic opreations to this object
 */
typedef struct {
    char *class_name;
    binary_method add;
    unary_method to_string;
    compare_method compare;
} BoaObjectImpl;

/* BoaObject
 * =================================================================================
 * This is the base objec that all subsequent types inherit from.
 * ref_count -> amount of references to this object that currently exists in the
 *              scope, once this reaches 0 a sweep_GC is called the memoory will be 
 *              reclaimed.
 * impl -> The base functionality of an object. Each data type defined by the
 *          standard library will have an implementation of type BoaObjectImpl.
 */
struct _boaobject {
    size_t ref_count;
    BoaObjectImpl *impl;
};

#ifdef __cplusplus
}
#endif

#endif /* __H_OBJECT__ */

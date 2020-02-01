#ifndef __H_OBJECT__
#define __H_OBJECT__

typedef struct {
    size_t ref_count;
} Object;

typedef struct {
    Object base;
    char name[4];
} ListObject;

#endif /* __H_OBJECT__ */

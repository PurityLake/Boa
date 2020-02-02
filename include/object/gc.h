#ifndef __H_GC__
#define __H_GC__

#include <stdlib.h>

#include "object.h"

#ifdef __cpluscplus
extern "C" {
#endif

typedef struct _entry {
    void *value;
    struct _entry *next;
} ReferenceTableEntry;

typedef struct {
    ReferenceTableEntry *start;
} ReferenceTable;

ReferenceTableEntry *create_emtpy_rtentry();

void init_GC();
void *malloc_GC(size_t size);
void *ison_GC(void *o);
void sweep_GC();
int len_GC();
void cleanup_GC();

#ifdef __cpluscplus
}
#endif

#endif /* __H_GC__ */

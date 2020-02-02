#ifndef __H_GC__
#define __H_GC__

#include <stdlib.h>

#include "object.h"

#ifdef __cpluscplus
extern "C" {
#endif

/* ReferenceTaleEntry
 * ==========================
 * Simple linked list pointing to BoaObject cast to a void pointer
 * value -> the physical memory that the object taks up, easily freed when reference
 *          count hits zero
 * next -> next entry in the table
 */
typedef struct _entry {
    void *value;
    struct _entry *next;
} ReferenceTableEntry;

typedef struct {
    ReferenceTableEntry *start;
} ReferenceTable;

ReferenceTableEntry *create_emtpy_rtentry();

void init_GC(void);
void *malloc_GC(size_t size);
void *ison_GC(void *o);
void sweep_GC(void);
int len_GC(void);
void cleanup_GC(void);

#ifdef __cpluscplus
}
#endif

#endif /* __H_GC__ */

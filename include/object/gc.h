#ifndef __H_GC__
#define __H_GC__

#include <stdlib.h>

#include "object.h"

#ifdef __cpluscplus
extern "C" {
#endif

/* ================================================================================
 * Gargbage Collector
 * ================================================================================
 * The garbage collector uses reference counting which is stored by the individual
 * objexts that are created by the user. There is a global reference table that is
 * used to hold the memory of objects created globally in the file run by the
 * interpreter.
 * ===============
 * Example usage
 *   init_GC();                            // stat the garbage collect
 *   BoaInteger *a = create_BoaInteger(1); // implicit call malloc_GC but increments the reference count
 *   destroy_BoaInteger(a);                // decreases reference count
 *   sweep_GC();                           // checks the reference table for ref counts of 0
 *   cleanup_GC();                         // frees all memory in the global reference table
 */

/* ReferenceTaleEntry
 * =================================================================================
 * Simple linked list pointing to BoaObject cast to a void pointer
 * value -> the physical memory that the object taks up, easily freed when reference
 *          count hits zero
 * next -> next entry in the table
 */
typedef struct _entry {
    void *value;
    struct _entry *next;
} ReferenceTableEntry;

/* ReferenceTable
 * =================================================================================
 * A wrapper for the ReferenceTableEntry
 * start -> the beginning of the table
 */
typedef struct {
    ReferenceTableEntry *start;
} ReferenceTable;

ReferenceTableEntry *create_emtpy_rtentry();

// must be called to allow for memory to be allocated and freed using
// and of the following functions
void init_GC(void);
// allocates memory and adds it to the global reference table
void *malloc_GC(size_t size);
// checks if a primitive is on the global reference table
void *ison_GC(void *o);
// destroys and memory with a reference count of 0
void sweep_GC(void);
// used to determine size of the global reference table
int len_GC(void);
// cleans up any left over memory in the global reference table and destroys the table
void cleanup_GC(void);

#ifdef __cpluscplus
}
#endif

#endif /* __H_GC__ */

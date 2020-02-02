#include "object/gc.h"

#include <stdio.h>

#ifdef __cpluscplus
extern "C" {
#endif

static ReferenceTable global_reference_table;

static int gc_cleanup_needed = 0;
static int gc_started = 0;

/* ReferenceTableEntry* reate_empty_rtentry
 * ===============================================================================
 * Simply creates new entry in reference table
 */
ReferenceTableEntry *create_emtpy_rtentry() {
    ReferenceTableEntry *out = (ReferenceTableEntry *)malloc(
        sizeof(ReferenceTableEntry));
    out->value = NULL;
    out->next = NULL;
    return out;
}

/* void init_GC
 * ================================================================================
 * Initialises the garbage collector, can only be run once
 */
void init_GC(void) {
    if (gc_started) return;
    global_reference_table.start = create_emtpy_rtentry();
    gc_started = gc_cleanup_needed = 1;
}

/* void* malloc_GC(size_t)
 * ================================================================================
 * Creates a new chunk of memory size `size` bytes. Adds
 * size: size_t -> size of the memory to allocate in bytes
 */
void *malloc_GC(size_t size) {
    if (!gc_started) return NULL;
    ReferenceTableEntry *curr = global_reference_table.start;
    if (curr->value == NULL) {
        curr->value = malloc(size);
        return curr->value;
    }
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = create_emtpy_rtentry();
    curr = curr->next;
    curr->value = malloc(size);
    return curr->value;
}

void *ison_GC(void *o) {
    if (!gc_started) return NULL;
    ReferenceTableEntry *curr = global_reference_table.start;
    while (curr != NULL) {
        if (curr->value != NULL) {
            if (EXPOSE_FUNC(curr->value)->compare(BOA_OBJ(curr->value), o)) {
                return curr->value;
            }
        }
        curr = curr->next;
    }
    return NULL;
}

void sweep_GC() {
    if (!gc_started) return;
    ReferenceTableEntry *curr = global_reference_table.start;
    ReferenceTableEntry *prev = NULL;
    while (curr != NULL) {
        if (curr->value != NULL) {
            if (GET_REF_COUNT(curr->value) == 0) {
                free(curr->value);
                curr->value = NULL;
                if (prev == NULL) {
                    curr = curr->next;
                } else {
                    prev->next = curr->next;
                    free(curr);
                    curr = prev->next;
                }
            } else {
                prev = curr;
                curr = curr->next;
            }
        } else {
            break;
        }
    }
}

int len_GC() {
    if (!gc_started) return -1;
    int i = 0;
    ReferenceTableEntry *curr = global_reference_table.start;
    if (curr->value != NULL) ++i;
    while (curr->next != NULL) {
        curr = curr->next;
        ++i;
    }
    return i;
}

void cleanup_GC() {
    if (gc_cleanup_needed) {
        ReferenceTableEntry *curr = global_reference_table.start;
        ReferenceTableEntry *prev = NULL;
        while (curr != NULL) {
            if (curr->value != NULL) {
                free(curr->value);
                curr->value = NULL;
            }
            prev = curr;
            curr = curr->next;
            free(prev);
        }
        gc_cleanup_needed = 0;
    }
}

#ifdef __cpluscplus
}
#endif

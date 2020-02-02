#include "gc.h"

#ifdef __cpluscplus
extern "C" {
#endif

static ReferenceTable global_reference_table;

static int gc_cleanup_needed = 0;
static int gc_started = 0;

ReferenceTableEntry *create_emtpy_rtentry() {
    ReferenceTableEntry *out = (ReferenceTableEntry *)malloc(sizeof(ReferenceTableEntry));
    out->value = NULL;
    out->next = NULL;
    return out;
}

void init_GC() {
    if (gc_started) return;

    global_reference_table.start = create_emtpy_rtentry();
    gc_started = 1;
}

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
    }
}

#ifdef __cpluscplus
}
#endif

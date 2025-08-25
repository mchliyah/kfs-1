#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

// Memory management constants
#define HEAP_START 0x100000  // Start heap at 1MB
#define HEAP_SIZE  0x100000  // 1MB heap size
#define BLOCK_SIZE 32        // Minimum block size

// Memory block structure
typedef struct memory_block {
    size_t size;
    int is_free;
    struct memory_block* next;
    struct memory_block* prev;
} memory_block_t;

// Memory management functions
void memory_init(void);
void* kmalloc(size_t size);
void kfree(void* ptr);
void* krealloc(void* ptr, size_t size);
void memory_dump(void);
size_t memory_get_free(void);
size_t memory_get_used(void);

#endif // MEMORY_H

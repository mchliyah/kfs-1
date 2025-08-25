#include "memory.h"
#include "string.h"

static memory_block_t* heap_start = NULL;
static int memory_initialized = 0;

void memory_init(void) {
    if (memory_initialized) return;
    
    heap_start = (memory_block_t*)HEAP_START;
    heap_start->size = HEAP_SIZE - sizeof(memory_block_t);
    heap_start->is_free = 1;
    heap_start->next = NULL;
    heap_start->prev = NULL;
    
    memory_initialized = 1;
}

static memory_block_t* find_free_block(size_t size) {
    memory_block_t* current = heap_start;
    
    while (current) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

static void split_block(memory_block_t* block, size_t size) {
    if (block->size <= size + sizeof(memory_block_t) + BLOCK_SIZE) {
        return; // Not worth splitting
    }
    
    memory_block_t* new_block = (memory_block_t*)((char*)block + sizeof(memory_block_t) + size);
    new_block->size = block->size - size - sizeof(memory_block_t);
    new_block->is_free = 1;
    new_block->next = block->next;
    new_block->prev = block;
    
    if (block->next) {
        block->next->prev = new_block;
    }
    
    block->next = new_block;
    block->size = size;
}

static void merge_free_blocks(memory_block_t* block) {
    // Merge with next block if it's free
    while (block->next && block->next->is_free) {
        memory_block_t* next_block = block->next;
        block->size += next_block->size + sizeof(memory_block_t);
        block->next = next_block->next;
        if (next_block->next) {
            next_block->next->prev = block;
        }
    }
    
    // Merge with previous block if it's free
    if (block->prev && block->prev->is_free) {
        memory_block_t* prev_block = block->prev;
        prev_block->size += block->size + sizeof(memory_block_t);
        prev_block->next = block->next;
        if (block->next) {
            block->next->prev = prev_block;
        }
    }
}

void* kmalloc(size_t size) {
    if (!memory_initialized) {
        memory_init();
    }
    
    if (size == 0) return NULL;
    
    // Align size to multiple of 8
    size = (size + 7) & ~7;
    
    // Ensure minimum block size
    if (size < BLOCK_SIZE) {
        size = BLOCK_SIZE;
    }
    
    memory_block_t* block = find_free_block(size);
    if (!block) {
        return NULL; // Out of memory
    }
    
    block->is_free = 0;
    split_block(block, size);
    
    return (char*)block + sizeof(memory_block_t);
}

void kfree(void* ptr) {
    if (!ptr || !memory_initialized) return;
    
    memory_block_t* block = (memory_block_t*)((char*)ptr - sizeof(memory_block_t));
    
    // Sanity check
    if ((char*)block < (char*)heap_start || 
        (char*)block >= (char*)heap_start + HEAP_SIZE) {
        return; // Invalid pointer
    }
    
    block->is_free = 1;
    merge_free_blocks(block);
}

void* krealloc(void* ptr, size_t size) {
    if (!ptr) {
        return kmalloc(size);
    }
    
    if (size == 0) {
        kfree(ptr);
        return NULL;
    }
    
    memory_block_t* block = (memory_block_t*)((char*)ptr - sizeof(memory_block_t));
    
    if (block->size >= size) {
        return ptr; // Current block is large enough
    }
    
    void* new_ptr = kmalloc(size);
    if (new_ptr) {
        size_t copy_size = (block->size < size) ? block->size : size;
        memcpy(new_ptr, ptr, copy_size);
        kfree(ptr);
    }
    
    return new_ptr;
}

size_t memory_get_free(void) {
    if (!memory_initialized) return 0;
    
    size_t free_memory = 0;
    memory_block_t* current = heap_start;
    
    while (current) {
        if (current->is_free) {
            free_memory += current->size;
        }
        current = current->next;
    }
    
    return free_memory;
}

size_t memory_get_used(void) {
    if (!memory_initialized) return 0;
    
    size_t used_memory = 0;
    memory_block_t* current = heap_start;
    
    while (current) {
        if (!current->is_free) {
            used_memory += current->size;
        }
        current = current->next;
    }
    
    return used_memory;
}

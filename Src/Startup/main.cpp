#include <Hardware.hpp>

extern "C" {
    // Ranges of the .bss section
extern size_t __bss_start[];
extern size_t __bss_end[];

    // Starting point of the data being values for static variables
extern size_t __data_load[];
    // Location of the static variables
extern size_t __data_start[];
extern size_t __data_end[];

    // Substitute common functions because linker links with --nostartfiles
void *memset(void *str, int c, size_t n){
    for(size_t idx = 0; idx < n; ++n){
        *(static_cast<char*>(str) + idx) = c;
    }
    return str;
}

void* memcpy(void * destination, const void * source, size_t num){
    for(size_t idx = 0; idx < num; ++idx){
        *(static_cast<char*>(destination) + idx) = *(static_cast<const char*>(source) + idx);
    }
    return destination;
}


extern "C" {
    void* pvPortMalloc(size_t);
    void* vPortFree(void*);
}

}
// Substitute calls for dynamic memory with FreeRTOS syscalls
void* operator new(size_t size){
    return pvPortMalloc(size);
}

void* operator new(size_t size, [[maybe_unused]] std::align_val_t alignment){
    return pvPortMalloc(size);
}

void operator delete(void* pointer){
    vPortFree(pointer);
}

void operator delete(void* pointer, [[maybe_unused]] size_t size){
    vPortFree(pointer);
}

void operator delete(void* pointer, [[maybe_unused]] std::align_val_t alignment){
    vPortFree(pointer);
}

// Assume that user will use this function as 'main'
void entryPoint();

void resetHandler() {
    // Zero the default-initialized data
    for(size_t* word = __bss_start; word < __bss_end; ++word){
        *word = 0;
    }

    // Initialize static data
    for(ptrdiff_t idx = 0; idx < __data_end - __data_start; ++idx){
        *(__data_start + idx) = *(__data_load + idx);
    }

    entryPoint();
}

// Handler for 1ms interrupt
void systick() {
        HAL_IncTick();
}
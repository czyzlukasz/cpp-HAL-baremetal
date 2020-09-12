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
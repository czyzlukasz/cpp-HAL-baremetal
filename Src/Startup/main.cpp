#include <Hardware.hpp>
#include <utility>
size_t i = 0x0012345678;
int m = -1234;
size_t y;
int k = 123;
std::pair<int, long> sm = {-21, 0x1234567890};
extern "C" {
extern size_t __bss_start[];
extern size_t __bss_end[];

extern size_t __data_load[];
extern size_t __data_start[];
extern size_t __data_end[];
}
// This is an entry point
void entryPoint() {
    // Zero the bss section
    for(size_t* word = __bss_start; word < __bss_end; ++word){
        *word = 0;
    }

    // Initialize static data
    for(ptrdiff_t idx = 0; idx < __data_end - __data_start; ++idx){
        *(__data_start + idx) = *(__data_load + idx);
    }

    // Initialize just LED
    Hardware::configureClocks();
    Hardware::enableGpio(GPIOC, GPIO_PIN_13, Gpio::Direction::Output);

    while (true) {
        Hardware::toggle(GPIOC, GPIO_PIN_13);
        HAL_Delay(y);
    }
}

// Handler for 1ms interrupt
void systick() {
        HAL_IncTick();
}
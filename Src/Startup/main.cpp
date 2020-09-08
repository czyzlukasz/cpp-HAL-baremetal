#include <Hardware.hpp>


// This is an entry point
void entryPoint() {
    // Initialize just LED
    Hardware::configureClocks();
    Hardware::enableGpio(GPIOC, GPIO_PIN_13, Gpio::Direction::Output);

    while (true) {
        Hardware::toggle(GPIOC, GPIO_PIN_13);
        HAL_Delay(100);
    }
}

// Handler for 1ms interrupt
void systick() {
        HAL_IncTick();
}
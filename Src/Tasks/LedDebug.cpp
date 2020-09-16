#include <LedDebug.hpp>
#include <Hardware.hpp>


LedDebug::LedDebug() : Task({}, 500, 2){

}

void LedDebug::initialize() {
    Hardware::configureClocks();
    Hardware::enableGpio(GPIOC, GPIO_PIN_13, Gpio::Mode::Output);
}

void LedDebug::run() {
    Hardware::toggle(GPIOC, GPIO_PIN_13);
    Hardware::toggle(GPIOC, GPIO_PIN_13);
    Hardware::toggle(GPIOC, GPIO_PIN_13);
}

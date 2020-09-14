#include <LedDebug.hpp>
#include <Hardware.hpp>


LedDebug::LedDebug() : Task({}, 500, 0), i(){

}

void LedDebug::initialize() {
    Hardware::configureClocks();
    Hardware::enableGpio(GPIOC, GPIO_PIN_13, Gpio::Direction::Output);
}

void LedDebug::run() {
    Hardware::toggle(GPIOC, GPIO_PIN_13);
    ++i;
}

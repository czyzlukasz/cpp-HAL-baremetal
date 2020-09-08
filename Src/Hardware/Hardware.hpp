#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include <stm32f1xx_hal.h>

// Goal of this abstraction is to isolate main code from hardware-specific code. Ideally, this code could use a stub and
// be compiled for Linux without any problem
namespace Gpio {
    enum class Direction {
        Input,
        Output
    };

    enum class Pull {
        Pullup = 1,
        PullDown = 2,
        NoPull = 0
    };
}

struct Hardware {
    static void enableGpio(GPIO_TypeDef* gpio, uint32_t pin, Gpio::Direction direction, Gpio::Pull pull = Gpio::Pull::NoPull);
    static void toggle(GPIO_TypeDef* gpio, uint32_t pin);
    static void configureClocks();
};


#endif //HARDWARE_HPP

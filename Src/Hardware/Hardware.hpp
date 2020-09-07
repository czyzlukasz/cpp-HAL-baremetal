#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include <stm32f1xx_hal.h>

struct Hardware {
    static void enableGpio();
    static void toggle();
};


#endif //HARDWARE_HPP

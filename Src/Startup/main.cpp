#include <Hardware.hpp>


// This is an entry point
void entryPoint() {
    // Initialize just LED
    Hardware::enableGpio();

    while (true) {
        Hardware::toggle();
    }
}

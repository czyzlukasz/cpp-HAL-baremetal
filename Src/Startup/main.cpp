#include <Hardware.hpp>


// This is an entry point
void entryPoint() {
    // Initialize just LED
    Hardware::enableGpio();

    int x = 0.0;
    int y = 0.0;
    while (true) {
        Hardware::toggle();
    }
}

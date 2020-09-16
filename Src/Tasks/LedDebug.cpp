#include <LedDebug.hpp>
#include <Hardware.hpp>


LedDebug::LedDebug() : Task({}, 500, 0){

}

void LedDebug::initialize() {
    Hardware::configureClocks();
    Hardware::enableGpio(GPIOC, GPIO_PIN_13, Gpio::Mode::Output);
    Hardware::initializeUart(Uart::Uart::UART_1, 9600);
}

void LedDebug::run() {
    Hardware::uartReceive(Uart::Uart::UART_1, dataReceived, 50);
    Hardware::uartSend(Uart::Uart::UART_1, data, 50);
    Hardware::toggle(GPIOC, GPIO_PIN_13);
}

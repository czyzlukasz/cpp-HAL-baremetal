#include <GpsInterface.hpp>
#include <Hardware.hpp>

// Items to collect:
// Latitude, longitude, altitude, speed

GpsInterface::GpsInterface() : Task({}, 2000, 3){

}

void GpsInterface::initialize() {
    Hardware::configureClocks();
    Hardware::initializeUart(Uart::Uart::UART_2, 9600);
    successfullReceptions = 0;
//    Hardware::uartReceive(Uart::Uart::UART_2, receiveBuffer.data(), 2);
}

void GpsInterface::run() {
    Hardware::abortUartRx(Uart::Uart::UART_2);
    if(Hardware::isUartRxComplete(Uart::Uart::UART_2)) {
        Hardware::uartReceive(Uart::Uart::UART_2, receiveBuffer.data(), 6);
        if(receiveBuffer.at(0) == '$'){
            ++successfullReceptions;
        }
        receiveBuffer.fill(0);
    }
}

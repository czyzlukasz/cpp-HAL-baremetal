#include <GpsInterface.hpp>
#include <Hardware.hpp>

// Items to collect:
// Latitude, longitude, altitude, speed

GpsInterface::GpsInterface() : Task({}, 500, 3){

}

void GpsInterface::initialize() {
    Hardware::configureClocks();
    Hardware::initializeUart(Uart::Uart::UART_2, 9600);
    receiveBuffer = std::make_unique<std::array<uint8_t, 256>>();
    prepareBufferForRead();
}

void GpsInterface::run() {
    if(receiveBuffer->at(1) != 0){
        // TODO: Parse buffer
        receiveBuffer->at(5) = 'X';
        ++dummy;
        receiveBuffer->fill(0); // Invalidate buffer
    }
    prepareBufferForRead();
}

void GpsInterface::prepareBufferForRead() {
    // Reset RX in order to start receiving new data from the start of the buffer
    Hardware::abortUartRx(Uart::Uart::UART_2);
    Hardware::uartReceive(Uart::Uart::UART_2, receiveBuffer->data(), receiveBuffer->size());
}

bool GpsInterface::isBufferFilled() const {
    return receiveBuffer->at(1) != 0;
}

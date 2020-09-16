#include <AhrsInterface.hpp>
#include <Src/Hardware/Hardware.hpp>
#include <algorithm>

AhrsInterface::AhrsInterface()
  : Task("AhrsInterface", 5, 1),
    parameterArray({Ahrs::Parameter::GYRO_X,
                    Ahrs::Parameter::GYRO_Y,
                    Ahrs::Parameter::GYRO_Z,
                    Ahrs::Parameter::ACCEL_X,
                    Ahrs::Parameter::ACCEL_Y,
                    Ahrs::Parameter::ACCEL_Z,
                    Ahrs::Parameter::MAG_X,
                    Ahrs::Parameter::MAG_Y,
                    Ahrs::Parameter::MAG_Z}){

}

void AhrsInterface::initialize() {
    Hardware::initializeUart(Uart::Uart::UART_1, 115200);
}

void AhrsInterface::run() {
    // Check received data
    if(Hardware::isUartRxComplete(Uart::Uart::UART_1)){
        if(isReceivedMessageValid()){
            const auto parameter = static_cast<Ahrs::Parameter>(getReceivedAddress());
            // Check if received address is interesting one
            if(std::find(parameterArray.cbegin(), parameterArray.cend(), parameter) != parameterArray.cend()) {
                const Ahrs::ValueType value = getValue();
                // TODO: save timestamp
                parameters[parameter].value = value;
            }
            invalidateReceivedData();
        }
    }

    if(Hardware::isUartTxComplete(Uart::Uart::UART_1)){
        currentParameter = (currentParameter + 1) % parameterArray.size();
        prepareReadMessage(parameterArray[currentParameter]);

        Hardware::uartReceive(Uart::Uart::UART_1, receiveBuffer.data(), 11);
        Hardware::uartSend(Uart::Uart::UART_1, sendBuffer.data(), 7);
    };
}

void AhrsInterface::prepareReadMessage(Ahrs::Parameter parameter) {
    // Standard header
    sendBuffer[0] = 's';
    sendBuffer[1] = 'n';
    sendBuffer[2] = 'p';
    // Packet type
    sendBuffer[3] = 0b00000000;
    // Address
    sendBuffer[4] = static_cast<uint8_t>(parameter);
    const auto [checksumFirst, checksumSecond] = calculateChecksum(sendBuffer.cbegin(), sendBuffer.cbegin() + 5);
    sendBuffer[5] = checksumFirst;
    sendBuffer[6] = checksumSecond;
}

uint8_t AhrsInterface::getReceivedAddress() const {
    return receiveBuffer[4];
}

Ahrs::ValueType AhrsInterface::getValue() const {
    union TypePunner{
        uint32_t uint32;
        float fpoint;
    };
    TypePunner value;
    value.uint32 = 0;
    value.uint32 += static_cast<uint32_t>(receiveBuffer[8]) << 0;
    value.uint32 += static_cast<uint32_t>(receiveBuffer[7]) << 8;
    value.uint32 += static_cast<uint32_t>(receiveBuffer[6]) << 16;
    value.uint32 += static_cast<uint32_t>(receiveBuffer[5]) << 24;
    return value.fpoint;
}

bool AhrsInterface::isReceivedMessageValid() const {
    bool isValid = true;
    isValid &= receiveBuffer[0] == 's';
    isValid &= receiveBuffer[1] == 'n';
    isValid &= receiveBuffer[2] == 'p';
    isValid &= receiveBuffer[3] == 0b10000000;

    const auto [checksumFirst, checksumSecond] = calculateChecksum(receiveBuffer.cbegin(), receiveBuffer.cbegin() + 9);
    isValid &= receiveBuffer[9] == checksumFirst;
    isValid &= receiveBuffer[10] == checksumSecond;

    return isValid;
}

void AhrsInterface::invalidateReceivedData() {
    receiveBuffer[0] = 'Q';
    receiveBuffer[1] = 'Q';
    receiveBuffer[2] = 'Q';
}

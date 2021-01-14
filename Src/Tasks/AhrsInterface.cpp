#include <AhrsInterface.hpp>
#include <Hardware.hpp>
#include <numeric>

AhrsInterface::AhrsInterface() :
    Task("", 100, 1),
    currentParameterIndex(0),
    parametersToRead({
         AhrsMsgId::SENSOR_TEMPERATURE,
         AhrsMsgId::GYRO_X,
         AhrsMsgId::GYRO_Y,
         AhrsMsgId::GYRO_Z,
         AhrsMsgId::ACCEL_X,
         AhrsMsgId::ACCEL_Y,
         AhrsMsgId::ACCEL_Z,
         AhrsMsgId::MAG_X,
         AhrsMsgId::MAG_Y,
         AhrsMsgId::MAG_Z,
    }){

}

void AhrsInterface::initialize() {
    Hardware::configureClocks();
    Hardware::initializeUart(Uart::Uart::UART_1, 115200);
}

void AhrsInterface::run() {
    // TODO: gangbang AHRS with message requests, expect that every time AHRS will catch up and not time out
    if(verifyReceivedMessage()){
        const auto [id, value] = getValueFromMsg(receiveBuffer);
        printf("AHRS Msg verified, parameter 0x%x, value x1000:%li\n", static_cast<uint>(id), static_cast<int32_t>(value * 1000));
        parameterValues.emplace(id, value);
    }
    else{
        printf("AHRS Msg invalid\n");
        Hardware::abortUartTx(Uart::Uart::UART_1);
        Hardware::abortUartRx(Uart::Uart::UART_1);
    }

    // Prepare for new Msg
    receiveBuffer.fill(0);
    Hardware::uartReceive(Uart::Uart::UART_1, receiveBuffer.data(), 11);
    // Request new Msg
    const AhrsMsgId newMsgId = parametersToRead.at(currentParameterIndex);
    ++currentParameterIndex;
    if(currentParameterIndex >= parametersToRead.size()){
        currentParameterIndex = 0;
    }
    sendRequest(newMsgId);
}

void AhrsInterface::sendRequest(AhrsMsgId msgId) {
    // Clear buffer in preparation for new data
    sendBuffer.fill(0);
    // Msg header
    sendBuffer.at(0) = 's';
    sendBuffer.at(1) = 'n';
    sendBuffer.at(2) = 'p';
    // Packet type
    sendBuffer.at(3) = 0;
    // Address
    sendBuffer.at(4) = static_cast<uint8_t>(msgId);
    // Checksum
    const auto [higherByte, lowerByte] = calculateControlSum(sendBuffer.cbegin(), sendBuffer.cbegin() + 5);
    sendBuffer.at(5) = higherByte;
    sendBuffer.at(6) = lowerByte;

    // Send prepared Msg
    Hardware::uartSend(Uart::Uart::UART_1, sendBuffer.data(), 7);
}

std::array<uint8_t, 2> AhrsInterface::calculateControlSum(std::array<uint8_t, 32>::const_iterator msgStart,
                                                          std::array<uint8_t, 32>::const_iterator msgEnd) const {
    const uint16_t sum = std::accumulate(msgStart, msgEnd, 0);
    const uint8_t lowerByte = sum & 0x00FF;
    const uint8_t higherByte = (sum & 0xFF00) >> 8;
    return {higherByte, lowerByte};
}

bool AhrsInterface::verifyReceivedMessage() const{
    bool result = true;
    // This is verbose by design, do not optimize it please
    result &= receiveBuffer.at(0) == 's';
    result &= receiveBuffer.at(1) == 'n';
    result &= receiveBuffer.at(2) == 'p';
    result &= receiveBuffer.at(3) == 0b10000000;    // Documentation p. 31

    // Checksum
    const auto [higherByte, lowerByte] = calculateControlSum(receiveBuffer.cbegin(), receiveBuffer.cbegin() + 9);
    result &= receiveBuffer.at(9) == higherByte;
    result &= receiveBuffer.at(10) == lowerByte;
    return result;
}

std::pair<AhrsMsgId, float> AhrsInterface::getValueFromMsg(const AhrsInterface::MsgBuffer &buffer) const {
    const auto id = static_cast<AhrsMsgId>(buffer.at(4));

    union {
        uint32_t integerValue;
        float floatValue;
    } result{};
    result.integerValue += static_cast<uint32_t>(buffer.at(5)) << 24;
    result.integerValue += static_cast<uint32_t>(buffer.at(6)) << 16;
    result.integerValue += static_cast<uint32_t>(buffer.at(7)) << 8;
    result.integerValue += static_cast<uint32_t>(buffer.at(8)) << 0;

    const float value = result.floatValue;

    return {id, value};
}

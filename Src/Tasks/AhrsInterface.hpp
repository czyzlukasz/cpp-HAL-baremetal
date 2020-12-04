#ifndef AHRSINTERFACE_HPP
#define AHRSINTERFACE_HPP


#include <TaskManager.hpp>

enum class AhrsMsgId : uint8_t {
    DEVICE_ID = 0xAA,
    SENSOR_TEMPERATURE = 0x5F,
    ACCEL_X = 0x65,
};

struct AhrsInterface : public Task {
    AhrsInterface();
    void initialize() override;
    void run() override;

private:
    void sendRequest(AhrsMsgId msgId);

    using MsgBuffer = std::array<uint8_t, 32>;
    std::array<uint8_t, 2> calculateControlSum(MsgBuffer::const_iterator msgStart, MsgBuffer::const_iterator msgEnd) const;
    [[nodiscard]] bool verifyReceivedMessage() const;
    float getValueFromMsg(const MsgBuffer& buffer) const;

    MsgBuffer sendBuffer;
    MsgBuffer receiveBuffer;
};


#endif //AHRSINTERFACE_HPP

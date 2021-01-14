#ifndef AHRSINTERFACE_HPP
#define AHRSINTERFACE_HPP


#include <TaskManager.hpp>
#include <unordered_map>

enum class AhrsMsgId : uint8_t {
    SENSOR_TEMPERATURE = 0x5F,
    GYRO_X = 0x61,
    GYRO_Y = 0x62,
    GYRO_Z = 0x63,
    ACCEL_X = 0x65,
    ACCEL_Y = 0x66,
    ACCEL_Z = 0x67,
    MAG_X = 0x69,
    MAG_Y = 0x6A,
    MAG_Z = 0x6B,
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
    std::pair<AhrsMsgId, float> getValueFromMsg(const MsgBuffer& buffer) const;

    MsgBuffer sendBuffer;
    MsgBuffer receiveBuffer;

    std::unordered_map<AhrsMsgId, float> parameterValues;

    uint32_t currentParameterIndex;
    const std::vector<AhrsMsgId> parametersToRead;
};


#endif //AHRSINTERFACE_HPP

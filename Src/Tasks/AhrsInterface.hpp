#ifndef AHRSINTERFACE_HPP
#define AHRSINTERFACE_HPP

#include <TaskManager.hpp>
#include <map>
#include <numeric>

namespace Ahrs{
    // Type that is used to store data fetched from AHRS
    using ValueType = float;

    // Enum values are also starting addresses
    enum class Parameter{
        GYRO_X = 0x61,
        GYRO_Y = 0x62,
        GYRO_Z = 0x63,
        ACCEL_X = 0x65,
        ACCEL_Y = 0x66,
        ACCEL_Z = 0x67,
        MAG_X = 0x69,
        MAG_Y = 0x6A,
        MAG_Z = 0x6B,
        VERSION = 0xAA
    };

    struct Value{
        ValueType value;
        uint32_t lastUpdate;
    };
}

struct AhrsInterface : public Task{
    AhrsInterface();

    void initialize() override;
    void run() override;

private:
    void prepareReadMessage(Ahrs::Parameter parameter);
    [[nodiscard]] uint8_t getReceivedAddress() const;
    [[nodiscard]] Ahrs::ValueType getValue() const;
    [[nodiscard]] bool isReceivedMessageValid() const;
    void invalidateReceivedData();

    // Checksum is 2-byte number that is the sum of all bytes in packet
    template<typename Type>
    std::pair<uint8_t, uint8_t> calculateChecksum(Type begin, Type end) const{
//        static_assert(std::is_same_v<std::iterator_traits<Iterator>::value_type, uint8_t>, "Passed iterator is wrong");
        uint16_t sum = std::accumulate(begin, end, 0);
        return {(sum & 0xFF00) >> 8, sum & 0xFF};
    }

private:
    // Receive buffer is max size of receive packet as per:
    // https://www.pololu.com/file/0J1227/um7_datasheet_v1-6_10.1.2016_1.pdf p. 32
    std::array<uint8_t, 7 + 4 * 16> receiveBuffer;
    // Transmit buffer is assumed to be "no batch" packet
    std::array<uint8_t, 11> sendBuffer;

    // Main storage for values
    std::map<Ahrs::Parameter, Ahrs::Value> parameters;

    // List of all parameters for easier iteration
    std::array<Ahrs::Parameter, 11> parameterArray;
    size_t currentParameter;
};


#endif //AHRSINTERFACE_HPP

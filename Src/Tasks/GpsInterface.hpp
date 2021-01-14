#ifndef GPSINTERFACE_HPP
#define GPSINTERFACE_HPP

#include <TaskManager.hpp>


struct GpsInterface : public Task{
    GpsInterface();
    void initialize() override;
    void run() override;

private:
    void prepareBufferForRead();
    [[nodiscard]] bool isBufferFilled() const;

    // Buffer is allocated on heap to avoid large stack allocation
    std::unique_ptr<std::array<uint8_t, 256>> receiveBuffer;
    int dummy;
};


#endif //GPSINTERFACE_HPP

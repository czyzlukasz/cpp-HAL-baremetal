#ifndef GPSINTERFACE_HPP
#define GPSINTERFACE_HPP

#include <TaskManager.hpp>


struct GpsInterface : public Task{
    GpsInterface();
    void initialize() override;
    void run() override;

private:
    std::array<uint8_t, 64> receiveBuffer;
    uint8_t successfullReceptions;
};


#endif //GPSINTERFACE_HPP

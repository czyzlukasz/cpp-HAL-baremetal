#ifndef LEDDEBUG_HPP
#define LEDDEBUG_HPP

#include <TaskManager.hpp>


struct LedDebug : public Task{
    LedDebug();
    void initialize() override;
    void run() override;

private:
    uint8_t data[50];
    uint8_t dataReceived[50];
};


#endif //LEDDEBUG_HPP

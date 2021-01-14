#ifndef LEDDEBUG_HPP
#define LEDDEBUG_HPP

#include <TaskManager.hpp>


struct LedDebug : public Task{
    LedDebug();
    void initialize() override;
    void run() override;
};


#endif //LEDDEBUG_HPP

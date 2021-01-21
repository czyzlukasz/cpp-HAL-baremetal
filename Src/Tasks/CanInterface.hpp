#ifndef CANINTERFACE_HPP
#define CANINTERFACE_HPP

#include <TaskManager.hpp>

struct CanInterface : Task{
    CanInterface();
    void initialize() override;
    void run() override;
};


#endif //CANINTERFACE_HPP

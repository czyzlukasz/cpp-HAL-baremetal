#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <string>
#include <array>
#include <vector>
#include <memory>
#include <FreeRTOS-Kernel/include/projdefs.h>


struct Task {
    using NameType = int;

    Task(NameType name, size_t periodicity, size_t priority);

public:
    [[noreturn]] void executeTask();
    [[nodiscard]] const char* getName() const;
    [[nodiscard]] size_t getPriority() const;
    TaskFunction_t getEntryPoint();

protected:
    virtual void initialize() {};
    virtual void run() {};

protected:
    const NameType name;
    const size_t periodicity;
    const size_t priority;
};

struct TaskManager {
    void registerTasks();
    void startTasks();
    void startRtos();

private:
    std::vector<std::shared_ptr<Task>> tasks;
};


#endif //TASKMANAGER_HPP

#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <string>
#include <array>
#include <vector>
#include <memory>


struct Task {
    using NameType = std::array<char, 15>;

    Task(NameType name, size_t periodicity, size_t priority);
    virtual void initialize() = 0;
    virtual void run() = 0;

    [[noreturn]] void executeTask();

protected:
    const NameType name;
    const size_t periodicity;
    const size_t priority;
};

struct TaskManager {
    void registerTasks();

private:
//    std::vector<std::shared_ptr<Task>> tasks;
};


#endif //TASKMANAGER_HPP

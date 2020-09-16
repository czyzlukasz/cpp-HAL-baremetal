#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include <string>
#include <array>
#include <vector>
#include <memory>
#include <FreeRTOS-Kernel/include/projdefs.h>


// Abstract class that wraps user's tasks.
// If user wants to create new task:
// Create object derived from Task
// Configure Task using Task's constructor
// Implement initialize() and run()
// Add this new task to TaskManager's tasks vector in registerTasks method

struct Task {
    using NameType = std::string;

    Task(NameType name, size_t periodicity, size_t priority);

public:
    [[noreturn]] void executeTask();
    [[nodiscard]] const char* getName() const;
    [[nodiscard]] size_t getPriority() const;

protected:
    // Setup method that is run single time at the start of the task
    virtual void initialize() = 0;
    // Method that is called once per 'periodicity' milliseconds
    virtual void run() = 0;

protected:
    const NameType name;
    const size_t periodicity;
    const size_t priority;
};

struct TaskManager {
    // Place where all tasks are being registered
    void registerTasks();
    // Register all tasks in FreeRTOS - allocate local stack etc.
    void startTasks();
    // Start scheduler - this function theoretically should not return
    void startRtos();

private:
    std::vector<std::shared_ptr<Task>> tasks;
};


#endif //TASKMANAGER_HPP

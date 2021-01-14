#include <TaskManager.hpp>
#include <FreeRTOS.h>
#include <queue.h>
#include <functional>

#include <LedDebug.hpp>

// TODO: move that to main.cpp
extern "C" {
    // Starting point of heap
    extern uint8_t __heap_start[];
    uint8_t* ucHeap = __heap_start;
}

// This is "main" - entry function that is called after system initialization
void entryPoint(){
    TaskManager taskManager;
    taskManager.registerTasks();
    taskManager.startTasks();
    taskManager.startRtos();
}

Task::Task(const Task::NameType name, size_t periodicity, size_t priority)
    : name(name), periodicity(periodicity), priority(priority){

}

void Task::executeTask() {
    initialize();

    while (true){
        run();
        vTaskDelay(periodicity);
    }
}

const char* Task::getName() const {
    return "Test";//name.c_str();
}

size_t Task::getPriority() const {
    return priority;
}

void TaskManager::registerTasks() {
    tasks.push_back(std::make_shared<LedDebug>());
}

void TaskManager::startTasks() {
    // This method simply calls main task's method that executes that task
    static const auto taskEntryPoint = [](void* task){
        reinterpret_cast<Task*>(task)->executeTask();
    };

    // Create all task by calling executeTask(). This is done by passing task pointer to taskEntryPoint.
    // TODO: Add stack size as a parameter
    for(const auto& task : tasks){
        xTaskCreate(taskEntryPoint, task->getName(), 128, task.get(), task->getPriority(), nullptr);
    }
}

void TaskManager::startRtos() {
    vTaskStartScheduler();
}

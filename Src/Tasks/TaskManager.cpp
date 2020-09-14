#include <TaskManager.hpp>
#include <FreeRTOS.h>
#include <queue.h>

#include <LedDebug.hpp>
#include <functional>


extern "C" {
    extern uint8_t __heap_start[];
    uint8_t* ucHeap = __heap_start;

    void taskEntryPoint(void* task){
        reinterpret_cast<Task*>(task)->executeTask();
    }
}

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
    return "Name";
}

size_t Task::getPriority() const {
    return priority;
}

TaskFunction_t Task::getEntryPoint() {
    return [](void* member){reinterpret_cast<Task*>(member)->executeTask();};
}

void TaskManager::registerTasks() {
    tasks.push_back(std::make_shared<LedDebug>());
}

void TaskManager::startTasks() {
    void* k = pvPortMalloc(250);
    for(const auto& task : tasks){
        xTaskCreate(taskEntryPoint, task->getName(), 128, task.get(), task->getPriority(), nullptr);
    }
    vPortFree(k);
}

void TaskManager::startRtos() {
    vTaskStartScheduler();
}

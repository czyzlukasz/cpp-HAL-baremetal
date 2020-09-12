#include <TaskManager.hpp>
#include <FreeRTOS.h>
#include <queue.h>

#include <LedDebug.hpp>


extern "C" {
    extern uint8_t __heap_start[];
    uint8_t* ucHeap = __heap_start;
}

void entryPoint(){
    TaskManager taskManager;
    taskManager.registerTasks();
}

Task::Task(const Task::NameType name, size_t periodicity, size_t priority)
    : name(name), periodicity(periodicity), priority(priority){

}

void Task::executeTask() {
    initialize();

    while (true){
        run();
        //Wait()
    }
}

void TaskManager::registerTasks() {
    auto m = std::make_shared<int>();
    auto n = std::make_shared<int>();
    auto o = std::make_shared<int>();
    tasks.push_back(std::make_shared<LedDebug>());
    tasks.push_back(std::make_shared<LedDebug>());
    tasks.push_back(std::make_shared<LedDebug>());
    tasks.push_back(std::make_shared<LedDebug>());
}

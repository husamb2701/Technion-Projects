#include "SortedList.h"
#include "TaskManager.h"
#include "Person.h"
#include "Task.h"
#include <iostream>
void TaskManager::assignTask(const string &personName, const Task &task) {
    Task tempTask = task;
    tempTask.setId(current_id);
    current_id++;
    int index = 0;
    while (index < this -> size) {
        if (persons [index] == nullptr) {
            index++;
            continue;
        }
        if (persons[index]->getName() == personName) {
            persons[index]->assignTask(tempTask);
            return;
        }
        index++;
    }
    index = 0;
    while (index < MAX_PERSONS) {
        if (persons[index] == nullptr) {
            persons[index] = new Person(personName);
            persons[index]->assignTask(tempTask);
            this -> size++;
            return;
        }
        index++;
    }
    throw std::runtime_error("out of bounds");
}
void TaskManager::completeTask(const string &personName) const{
    for (int i = 0; i < this -> size; i++) {
        if (persons[i] == nullptr) continue;
        if (persons[i]->getName() == personName) {
            persons[i]->completeTask();
            return;
        }
    }
}

void TaskManager::bumpPriorityByType(TaskType type, int priority) const {
    if (priority < 0) {
        return;
    }
    for (int i = 0; i < this->size; i++) {
        mtm::SortedList<Task> temp_to_bump = persons[i]->getTasks();

        for (auto it = temp_to_bump.begin(); it != temp_to_bump.end(); ) {
            Task task = *it;

            if (task.getType() == type) {
                int new_priority = task.getPriority() + priority;
                if (new_priority > 100) {
                    new_priority = 100;
                }

                Task updatedTask(new_priority, task.getType(), task.getDescription());
                updatedTask.setId(task.getId());

                auto to_erase = it;
                ++it;
                temp_to_bump.remove(to_erase);

                temp_to_bump.insert(updatedTask);
            } else {
                ++it;
            }
        }

        persons[i]->setTasks(temp_to_bump);
    }
}
void TaskManager::printAllEmployees() const {
    for (int i = 0; i < this -> size; i++) {
        if (persons[i] != nullptr) {
            std::cout << *persons[i] << std::endl;
        }
    }
}
void TaskManager::printAllTasks() const {
    mtm::SortedList<Task> all_list;
    for (int i = 0; i < this -> size; i++) {
        if (persons[i] == nullptr) continue;
        mtm::SortedList<Task> TempList = persons[i]->getTasks();
        for (const auto& task : TempList) {
            all_list.insert(task);
        }
    }
    for (const auto& task : all_list) {
        std::cout << task << std::endl;
    }
}
void TaskManager::printTasksByType(TaskType type) const {
    mtm::SortedList<Task> all_list;
    for (int i = 0; i < this -> size; i++) {
        if (persons[i] == nullptr) continue;
        mtm::SortedList<Task> temp_list = persons[i]->getTasks();
        for (const auto& task : temp_list) {
            if (task.getType() == type) {
                all_list.insert(task);
            }
        }
    }
    for (const auto& task : all_list) {
        std::cout << task << std::endl;
    }
}

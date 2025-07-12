// ***************************************************************************
//
//   Joel John Thomas 
//   Z2004783
//   CSCI 480 - PE1 
//
//   I certify that this is my own work and where appropriate an extension 
//   of the starter code provided for the assignment. 
// ***************************************************************************

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include <vector>
#include <memory>

// The Scheduler manages all processes, tracks their states,
// and selects the next process to run based on simple round-robin logic.
class Scheduler {
private:
    std::vector<std::shared_ptr<PCB>> processes; // All PCBs
    size_t currentIndex = 0; // Index of the last process selected

public:
    // Add a new process to the list
    void addProcess(std::shared_ptr<PCB> pcb);

    // Get the next process in round-robin order that is Ready or New
    std::shared_ptr<PCB> getNextProcess();

    // Check if any processes are still runnable
    bool hasRunnableProcess();

    // Mark a process as terminated based on its PID
    void markTerminated(uint32_t pid);
};

#endif
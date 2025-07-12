#include "Scheduler.h"

// Add a new process to the scheduler’s process list
void Scheduler::addProcess(std::shared_ptr<PCB> pcb) {
    processes.push_back(pcb);
}

// Get the next process to run using round-robin scheduling
std::shared_ptr<PCB> Scheduler::getNextProcess() {
    if (processes.empty()) return nullptr;

    // Loop through all processes starting from current index
    for (size_t i = 0; i < processes.size(); ++i) {
        size_t idx = (currentIndex + i) % processes.size();
        auto& pcb = processes[idx];

        // Only return a process that is ready or has never run (New)
        if (pcb->state == ProcessState::Ready || pcb->state == ProcessState::New) {
            currentIndex = (idx + 1) % processes.size(); // update for next round
            return pcb;
        }
    }

    return nullptr; // No runnable process found
}

// Check if there are any processes left that are not terminated
bool Scheduler::hasRunnableProcess() {
    for (auto& pcb : processes) {
        if (pcb->state == ProcessState::Ready || pcb->state == ProcessState::New)
            return true;
    }
    return false;
}

// Mark a process as terminated by PID
void Scheduler::markTerminated(uint32_t pid) {
    for (auto& pcb : processes) {
        if (pcb->pid == pid) {
            pcb->state = ProcessState::Terminated;
            break;
        }
    }
}

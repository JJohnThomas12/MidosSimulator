// ***************************************************************************
//
//   Joel John Thomas 
//   Z2004783
//   CSCI 480 - PE1 
//
//   I certify that this is my own work and where appropriate an extension 
//   of the starter code provided for the assignment. 
// ***************************************************************************

#include "CPU.h"
#include "MMU.h"
#include "ProgramLoader.h"
#include "Scheduler.h"
#include <memory>
#include <iostream>

int main() {
    // Create MMU with 4KB of physical memory
    MMU mmu(4096);

    // Create the process scheduler
    Scheduler scheduler;

    // Load two different programs from external .txt files
    // Ensure programs use non-overlapping memory regions
    auto p1 = ProgramLoader::loadProgram(mmu, 1, 0, "program1.txt");
    auto p2 = ProgramLoader::loadProgram(mmu, 2, 1024, "program2.txt");


    // Register PCBs with the scheduler
    scheduler.addProcess(p1);
    scheduler.addProcess(p2);

    // Initialize CPU with references to MMU and Scheduler
    CPU cpu(mmu, scheduler);

    // Loop until all processes have terminated
    while (scheduler.hasRunnableProcess()) {
        auto process = scheduler.getNextProcess();
        if (!process) break;
        cpu.runProcess(process);
    }

    std::cout << "All processes completed.\n";
    return 0;
}
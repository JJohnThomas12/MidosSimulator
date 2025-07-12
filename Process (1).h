// ***************************************************************************
//
//   Joel John Thomas 
//   Z2004783
//   CSCI 480 - PE1 
//
//   I certify that this is my own work and where appropriate an extension 
//   of the starter code provided for the assignment. 
// ***************************************************************************

#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <unordered_map>
#include <cstdint>

// Possible lifecycle states for a process
enum class ProcessState {
    New,   
    Ready,
    Running,
    Waiting, 
    Terminated
};

// Process Control Block (PCB)
// Stores all essential state information about a process
struct PCB {
    uint32_t pid;               // Process ID
    uint32_t registers[14];     // CPU register values
    bool signFlag = false;      // CPU sign flag
    bool zeroFlag = false;      // CPU zero flag

    uint32_t priority = 1;      // Priority for scheduling (lower = higher priority)
    uint32_t timeQuantum = 10;  // Max ticks allowed before context switch
    uint64_t clockCycles = 0;   // Total CPU time used
    uint32_t sleepCounter = 0;  // Remaining sleep time (if any)
    uint32_t contextSwitches = 0; // Number of times the process was switched in/out

    ProcessState state = ProcessState::New; // Current state of the process

    // Memory segmentation (virtual layout)
    uint32_t codeStart = 0;
    uint32_t codeSize = 0;

    uint32_t stackStart = 0;
    uint32_t stackSize = 256;

    uint32_t dataStart = 0;
    uint32_t dataSize = 512;

    uint32_t heapStart = 0;
    uint32_t heapEnd = 0;

    // Virtual page number → physical page address
    std::unordered_map<uint32_t, uint32_t> pageTable;
};

#endif
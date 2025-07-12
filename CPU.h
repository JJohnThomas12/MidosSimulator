#ifndef CPU_H
#define CPU_H

#include "MMU.h"
#include "Scheduler.h"
#include <memory>
#include <vector>

class CPU {
public:
    // Constructor: Initializes CPU with MMU and Scheduler references
    CPU(MMU& mmu, Scheduler& scheduler);

    // Executes the given process for one time quantum or until it terminates
    void runProcess(std::shared_ptr<PCB> pcb);

private:
    MMU& mmu;               // Memory Management Unit reference
    Scheduler& scheduler;   // Scheduler reference

    // General-purpose and special-purpose registers (e.g., r1-r10, PC, etc.)
    uint32_t registers[14] = {0};

    // Condition flags (optional use for future extension)
    bool signFlag = false;
    bool zeroFlag = false;

    // Load process state (registers + flags) from PCB into CPU
    void loadState(std::shared_ptr<PCB> pcb);

    // Save current CPU state back into the PCB
    void saveState(std::shared_ptr<PCB> pcb);

    //SYSTEM-WIDE SHARED STRUCTURES FOR ALL PROCESSES

    // Shared memory configuration
    static constexpr uint32_t sharedMemBase   = 0x00004000; // Virtual base address of shared memory
    static constexpr uint32_t sharedPhysAddr  = 0x0F00;     // Physical base address of shared memory
    static constexpr uint32_t sharedPageCount = 2;          // Number of shared pages (each 256 bytes)

    // Lock array: 10 global locks (index 0–9), true = held, false = available
    std::vector<bool> locks = std::vector<bool>(10, false);

    // Event array: 10 global event flags, true = signaled, false = waiting
    std::vector<bool> events = std::vector<bool>(10, false);
};

#endif

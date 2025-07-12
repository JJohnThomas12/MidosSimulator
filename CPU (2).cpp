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
#include "Instructions.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
using namespace std;

// Constructor: Initializes CPU with references to MMU and Scheduler
CPU::CPU(MMU& mmuRef, Scheduler& schedRef)
    : mmu(mmuRef), scheduler(schedRef) {}

// Loads saved process state into CPU registers and flags
void CPU::loadState(shared_ptr<PCB> pcb) {
    for (int i = 0; i < 14; ++i)
        registers[i] = pcb->registers[i];
    signFlag = pcb->signFlag;
    zeroFlag = pcb->zeroFlag;
    mmu.setPageTable(pcb->pageTable); // Set MMU's active page table
}

// Saves current CPU state back into PCB
void CPU::saveState(shared_ptr<PCB> pcb) {
    for (int i = 0; i < 14; ++i)
        pcb->registers[i] = registers[i];
    pcb->signFlag = signFlag;
    pcb->zeroFlag = zeroFlag;
}

// Main execution loop for a single process
void CPU::runProcess(shared_ptr<PCB> pcb) {
    loadState(pcb);
    pcb->state = ProcessState::Running;
    uint32_t localTicks = 0;
    stringstream ss;

    // Execute up to pcb->timeQuantum instructions
    while (localTicks < pcb->timeQuantum) {
        uint32_t ip = registers[11];         // Instruction pointer
        uint8_t opcode = mmu.readByte(ip);   // Read instruction opcode
        uint32_t op1 = mmu.readWord(ip + 1); // First operand
        uint32_t op2 = mmu.readWord(ip + 5); // Second operand

        ss.str(""); ss.clear(); // Clear formatting buffer

        switch (opcode) {
            case MOVI:
                // Move immediate value to register
                if (op1 < 10) {
                    registers[op1] = op2;
                    ss << "[PID " << pcb->pid << "] " << left << setw(10) << "MOVI"
                       << "r" << (op1 + 1) << ", " << setw(12) << op2
                       << "→ r" << (op1 + 1) << " = " << registers[op1];
                }
                break;

            case INCR:
                // Increment register
                if (op1 < 10) {
                    registers[op1]++;
                    ss << "[PID " << pcb->pid << "] " << left << setw(10) << "INCR"
                       << "r" << (op1 + 1) << setw(17) << " "
                       << "→ r" << (op1 + 1) << " = " << registers[op1];
                }
                break;

            case ADDI:
                // Add immediate value to register
                if (op1 < 10) {
                    registers[op1] += op2;
                    ss << "[PID " << pcb->pid << "] " << left << setw(10) << "ADDI"
                       << "r" << (op1 + 1) << ", " << setw(12) << op2
                       << "→ r" << (op1 + 1) << " = " << registers[op1];
                }
                break;

            case PRINTR:
                // Print register contents
                if (op1 < 10) {
                    ss << "[PID " << pcb->pid << "] " << left << setw(10) << "PRINTR"
                       << "r" << (op1 + 1) << setw(17) << " "
                       << "→ output: " << registers[op1];
                }
                break;

            case STORE:
                // Store register value into memory address from another register
                if (op1 < 10 && op2 < 10) {
                    uint32_t addr = registers[op2];
                    mmu.writeWord(addr, registers[op1]);
                    ss << "[PID " << pcb->pid << "] " << left << setw(10) << "STORE"
                       << "r" << (op1 + 1) << " → [r" << (op2 + 1) << "] = " << registers[op1];
                }
                break;

            case LOAD:
                // Load memory value into register using address from another register
                if (op1 < 10 && op2 < 10) {
                    uint32_t addr = registers[op2];
                    registers[op1] = mmu.readWord(addr);
                    ss << "[PID " << pcb->pid << "] " << left << setw(10) << "LOAD"
                       << "r" << (op1 + 1) << " ← [r" << (op2 + 1) << "] = " << registers[op1];
                }
                break;

            case MAPSHARED:
                // Map shared memory pages into the process’s page table
                if (op1 == 0 && op2 < 10) {
                    for (uint32_t i = 0; i < sharedPageCount; ++i) {
                        uint32_t virtPage = (sharedMemBase + i * 256) >> 8;
                        uint32_t physPage = sharedPhysAddr + i * 256;
                        pcb->pageTable[virtPage] = physPage;
                    }
                    registers[op2] = sharedMemBase;
                    ss << "[PID " << pcb->pid << "] " << left << setw(10) << "MAPSHARED"
                       << "region 0 → r" << (op2 + 1) << " = " << sharedMemBase;
                }
                break;

            case ACQUIRELOCKI:
                // Attempt to acquire a lock at index `op1`
                if (op1 < locks.size()) {
                    ss << "[PID " << pcb->pid << "] " << left << setw(10) << "ACQUIRELOCKI"
                       << "index " << op1;
                    if (!locks[op1]) {
                        locks[op1] = true;
                        ss << " → lock acquired";
                    } else {
                        ss << " → already held";
                    }
                }
                break;

            case WAITEVENTI:
                // Wait for event to be signaled; blocks if not signaled
                if (op1 < events.size()) {
                    ss << "[PID " << pcb->pid << "] " << left << setw(10) << "WAITEVENTI"
                       << "index " << op1;
                    if (!events[op1]) {
                        ss << " → blocked";
                        cout << ss.str() << endl;
                        saveState(pcb);
                        pcb->state = ProcessState::Waiting;
                        pcb->clockCycles += localTicks;
                        pcb->contextSwitches++;
                        return;
                    } else {
                        ss << " → passed";
                    }
                }
                break;

            case SIGNALEVENTI:
                // Signal the event at index `op1`, allowing other processes to continue
                if (op1 < events.size()) {
                    events[op1] = true;
                    ss << "[PID " << pcb->pid << "] " << left << setw(10) << "SIGNALEVENTI"
                       << "index " << op1 << " → signaled";
                }
                break;

            case EXIT:
                // Terminate process execution
                ss << "[PID " << pcb->pid << "] " << left << setw(10) << "EXIT"
                   << setw(20) << " "
                   << "→ process terminated";
                cout << ss.str() << endl;
                saveState(pcb);
                pcb->state = ProcessState::Terminated;
                pcb->clockCycles += localTicks;
                pcb->contextSwitches++;
                return;

            default:
                // Unrecognized instruction
                throw runtime_error("Unknown opcode");
        }

        // Print current instruction output
        cout << ss.str() << endl;
        registers[11] += 9; // Advance to next instruction
        localTicks++;
    }

    // Time quantum expired; mark process as ready and save state
    pcb->clockCycles += localTicks;
    pcb->contextSwitches++;
    saveState(pcb);
    pcb->state = ProcessState::Ready;
}
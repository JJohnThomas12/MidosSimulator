// ***************************************************************************
//
//   Joel John Thomas 
//   Z2004783
//   CSCI 480 - PE1 
//
//   I certify that this is my own work and where appropriate an extension 
//   of the starter code provided for the assignment. 
// ***************************************************************************

#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H

#include "MMU.h"
#include "Process.h"
#include <memory>
#include <string>

// ProgramLoader reads machine code from a .txt file and loads it into memory.
class ProgramLoader {
public:
    // Load a program from filePath into memory at startPhysAddr and return a PCB
    static std::shared_ptr<PCB> loadProgram(MMU& mmu, uint32_t pid, uint32_t startPhysAddr, const std::string& filePath);
};

#endif
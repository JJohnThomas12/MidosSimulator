#include "ProgramLoader.h"
#include "Instructions.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>

std::shared_ptr<PCB> ProgramLoader::loadProgram(MMU& mmu, uint32_t pid, uint32_t startPhysAddr, const std::string& filePath) {
    auto pcb = std::make_shared<PCB>();
    pcb->pid = pid;
    pcb->priority = (pid == 1) ? 5 : 10;
    pcb->timeQuantum = 10;

    const uint32_t dataSize = 512;
    const uint32_t heapSize = 256;
    const uint32_t stackSize = 256;
    const uint32_t pageSize = 256;

    // Load the machine code bytes from the text file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open program file: " + filePath);
    }

    std::vector<uint8_t> program;
    std::string line, byteStr;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        while (iss >> byteStr) {
            uint8_t byte = static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16));
            program.push_back(byte);
        }
    }

    uint32_t codeSize = program.size();
    uint32_t codeBase = startPhysAddr;
    uint32_t dataBase = codeBase + codeSize;
    uint32_t heapBase = dataBase + dataSize;
    uint32_t stackBase = heapBase + heapSize;

    // Assign virtual memory layout
    pcb->codeStart = 0x00000000;
    pcb->codeSize = codeSize;
    pcb->dataStart = 0x00001000;
    pcb->dataSize = dataSize;
    pcb->heapStart = 0x00002000;
    pcb->heapEnd = heapBase + heapSize;
    pcb->stackStart = 0x00003000;
    pcb->stackSize = stackSize;

    // Set registers
    pcb->registers[11] = pcb->codeStart;                   // IP
    pcb->registers[12] = pcb->dataStart;                   // GMEM
    pcb->registers[13] = pcb->stackStart + stackSize;      // SP

    // Load the program into physical memory
    for (size_t i = 0; i < program.size(); ++i) {
        mmu.writePhysical(codeBase + i, program[i]);
    }

    // Virtual to physical memory mapping
    for (uint32_t vAddr = pcb->codeStart; vAddr < pcb->codeStart + codeSize; vAddr += pageSize)
        pcb->pageTable[vAddr >> 8] = ((codeBase + (vAddr - pcb->codeStart)) / pageSize) * pageSize;

    for (uint32_t vAddr = pcb->dataStart; vAddr < pcb->dataStart + dataSize; vAddr += pageSize)
        pcb->pageTable[vAddr >> 8] = ((dataBase + (vAddr - pcb->dataStart)) / pageSize) * pageSize;

    for (uint32_t vAddr = pcb->heapStart; vAddr < pcb->heapStart + heapSize; vAddr += pageSize)
        pcb->pageTable[vAddr >> 8] = ((heapBase + (vAddr - pcb->heapStart)) / pageSize) * pageSize;

    for (uint32_t vAddr = pcb->stackStart; vAddr < pcb->stackStart + stackSize; vAddr += pageSize)
        pcb->pageTable[vAddr >> 8] = ((stackBase + (vAddr - pcb->stackStart)) / pageSize) * pageSize;

    pcb->state = ProcessState::Ready;
    return pcb;
}

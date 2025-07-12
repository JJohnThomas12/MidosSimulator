#include "MMU.h"
#include <stdexcept>
#include <cstring>

// Constructor
MMU::MMU(uint32_t memorySize)
    : memory(memorySize, 0) {} // Initialize memory with 0s

// Set the page table for the currently running process
void MMU::setPageTable(const std::unordered_map<uint32_t, uint32_t>& pt) {
    pageTable = pt;
}

// PHYSICAL MEMORY ACCESS (unsafe - no translation)

// Direct read from physical memory
uint8_t MMU::readPhysical(uint32_t addr) const {
    if (addr >= memory.size())
        throw std::out_of_range("readPhysical: out of bounds");
    return memory[addr];
}

// Direct write to physical memory
void MMU::writePhysical(uint32_t addr, uint8_t value) {
    if (addr >= memory.size())
        throw std::out_of_range("writePhysical: out of bounds");
    memory[addr] = value;
}

// VIRTUAL MEMORY ACCESS (safe - uses translation)

// Read a byte from virtual memory
uint8_t MMU::readByte(uint32_t vAddr) const {
    return memory[translate(vAddr)];
}

// Write a byte to virtual memory
void MMU::writeByte(uint32_t vAddr, uint8_t value) {
    memory[translate(vAddr)] = value;
}

// Read a 32-bit word from virtual memory
uint32_t MMU::readWord(uint32_t vAddr) const {
    uint32_t phys = translate(vAddr);
    uint32_t result = 0;
    std::memcpy(&result, &memory[phys], sizeof(uint32_t));
    return result;
}

// Write a 32-bit word to virtual memory
void MMU::writeWord(uint32_t vAddr, uint32_t value) {
    uint32_t phys = translate(vAddr);
    std::memcpy(&memory[phys], &value, sizeof(uint32_t));
}

// VIRTUAL → PHYSICAL ADDRESS TRANSLATION

// Converts virtual address to physical address using the current page table
uint32_t MMU::translate(uint32_t vAddr) const {
    const uint32_t pageSize = 256;
    uint32_t vpn = vAddr >> 8;        // Virtual Page Number
    uint32_t offset = vAddr & 0xFF;   // Offset within the page

    auto it = pageTable.find(vpn);
    if (it == pageTable.end())
        throw std::runtime_error("MMU: page fault (unmapped virtual page)");

    uint32_t physBase = it->second;   // Base address of physical page
    return physBase + offset;
}

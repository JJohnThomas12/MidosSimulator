// ***************************************************************************
//
//   Joel John Thomas 
//   Z2004783
//   CSCI 480 - PE1 
//
//   I certify that this is my own work and where appropriate an extension 
//   of the starter code provided for the assignment. 
// ***************************************************************************

#ifndef MMU_H
#define MMU_H

#include <cstdint>
#include <vector>
#include <unordered_map>

class MMU {
public:
    // Initialize memory with a given size
    MMU(uint32_t memorySize);

    // Load page table from a process
    void setPageTable(const std::unordered_map<uint32_t, uint32_t>& pt);

    // Read/write using physical memory
    uint8_t  readPhysical(uint32_t addr) const;
    void     writePhysical(uint32_t addr, uint8_t value);

    // Read/write using virtual memory (with translation)
    uint8_t  readByte(uint32_t vAddr) const;
    void     writeByte(uint32_t vAddr, uint8_t value);
    uint32_t readWord(uint32_t vAddr) const;
    void     writeWord(uint32_t vAddr, uint32_t value);

private:
    std::vector<uint8_t> memory; // Physical memory
    std::unordered_map<uint32_t, uint32_t> pageTable; // Virtual → Physical page mapping

    uint32_t translate(uint32_t vAddr) const; // Page-based address translation
};

#endif
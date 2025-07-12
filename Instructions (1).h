// ***************************************************************************
//
//   Joel John Thomas 
//   Z2004783
//   CSCI 480 - PE1 
//
//   I certify that this is my own work and where appropriate an extension 
//   of the starter code provided for the assignment. 
// ***************************************************************************

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

// Core Instructions (Module 1–3)
const uint8_t MOVI       = 0x01;  // MOVI rx, imm: Set register rx = imm
const uint8_t INCR       = 0x02;  // INCR rx: Increment register rx
const uint8_t ADDI       = 0x03;  // ADDI rx, imm: Add imm to register rx
const uint8_t PRINTR     = 0x0B;  // PRINTR rx: Output register rx
const uint8_t EXIT       = 0xFF;  // EXIT: Terminate the process

const uint8_t STORE = 0x10;  // STORE rx → [ry]
const uint8_t LOAD  = 0x11;  // LOAD rx ← [ry]

// Shared Memory Instructions
const uint8_t MAPSHARED     = 0x20;  // MAPSHARED rx, ry: map shared mem region rx to virtual addr ry

// Lock (Mutex) Instructions
const uint8_t ACQUIRELOCK   = 0x30;  // ACQUIRELOCK rx: Acquire lock at index in rx
const uint8_t ACQUIRELOCKI  = 0x31;  // ACQUIRELOCKI imm: Acquire lock at index imm

const uint8_t RELEASELOCK   = 0x32;  // RELEASELOCK rx: Release lock at index in rx
const uint8_t RELEASELOCKI  = 0x33;  // RELEASELOCKI imm: Release lock at index imm

// Event Instructions
const uint8_t WAITEVENT     = 0x40;  // WAITEVENT rx: Wait for event index in rx
const uint8_t WAITEVENTI    = 0x41;  // WAITEVENTI imm: Wait for event imm

const uint8_t SIGNALEVENT   = 0x42;  // SIGNALEVENT rx: Signal event at index in rx
const uint8_t SIGNALEVENTI  = 0x43;  // SIGNALEVENTI imm: Signal event at index imm

#endif
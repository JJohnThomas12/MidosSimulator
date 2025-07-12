# MidOS – A Simplified Operating System Simulator in C++

**MidOS** is an educational operating system simulator written in C++ that models core OS components such as process scheduling, memory management, and instruction execution. Built for **CSCI 480: Principles of Operating Systems** at Northern Illinois University, this project emphasizes hands-on implementation of a real-world OS environment, including CPU cycles, PCBs, virtual memory, and context switching.

---

## 🚀 Features

- 🧠 **Process Management**  
  Implements round-robin scheduling with support for multiple concurrent processes, context switches, and lifecycle states (New, Ready, Running, Waiting, Terminated).

- 🧮 **Virtual Memory with Paging**  
  Each process has its own virtual address space (code, data, heap, and stack segments) with translation to physical memory via a custom MMU.

- 💾 **Program Loader**  
  Loads binary-encoded programs from `.txt` files into memory with non-overlapping memory regions and sets up virtual-to-physical page tables.

- ⚙️ **Instruction Execution**  
  Custom CPU simulator capable of decoding and executing core instructions (e.g., `MOVI`, `INCR`, `ADDI`, `STORE`, `LOAD`, `EXIT`, `PRINTR`).

- 🔐 **Locks & Events (Extendable)**  
  Prepares shared memory, mutexes, and event signaling structures for interprocess communication and synchronization.

- 📈 **Statistics Tracking**  
  Tracks CPU clock cycles, time quantum expiration, and context switch counts per process.
  
---

  ## 🧠 Concepts Demonstrated

- Custom implementation of a scheduler, loader, MMU, and CPU  
- Process lifecycle transitions and context preservation  
- Virtual memory paging and address translation  
- Instruction decoding and control flow  
- Simulated interprocess isolation and memory safety  
- Low-level systems programming using C++ with pointers, maps, and manual memory control  

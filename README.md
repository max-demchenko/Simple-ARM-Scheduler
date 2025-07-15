# Tiny ARM Scheduler

This is a simple preemptive scheduler I wrote to learn more about ARM Cortex-M architecture and the embedded build process.

The target device is the STM32F103C6T6 (ARM Cortex-M3).  
The scheduler is written from scratch — no external libraries, no RTOS, just raw C and assembly.
To build the project, you need the ARM GNU toolchain.

---

## Overview

This project helped me explore:

- Exception and interrupt handling on Cortex-M
- How to manage stacks and switch context using `PendSV`
- ARM stack frame layout and handler modes
- How linker scripts, memory layout, and startup code work together
- Bare-metal build flow using GCC

---
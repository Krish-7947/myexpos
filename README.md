# Expos NITC OS Lab Project

**Expos NITC** is the experimental operating system developed using the **XSM** (eXperimental String Machine) architecture and the **XFS** (eXperimental File System) interface. This repository contains the complete toolchain and stage-wise implementation artifacts for building **eXpOS** — a multiprogrammed, demand-paged OS with file management, process synchronization, user management, and virtual memory (pager).

---

## Overview

eXpOS is implemented incrementally across **28 lab stages**. Each stage introduces a new OS concept — from raw machine code and filesystem layout to a fully functional shell with login, file utilities, and paging. The final system is assembled via the `Batchfile`, which loads all kernel modules, interrupt handlers, and user programs onto a simulated disk (`disk.xfs`) and runs them on the XSM simulator.

| Component                            | Language                | Purpose                                                    |
| ------------------------------------ | ----------------------- | ---------------------------------------------------------- |
| **XSM Simulator** (`xsm/`)           | C                       | Simulates XSM hardware (CPU, memory, timer, disk, console) |
| **XFS Interface** (`xfs-interface/`) | C                       | Formats and manages the virtual disk filesystem            |
| **SPL Compiler** (`spl/`)            | C / Lex / Yacc          | Compiles System Programmer's Language (kernel code)        |
| **ExpL Compiler** (`expl/`)          | C / Lex / Yacc          | Compiles Experimental Language (user programs)             |
| **Test artifacts** (`test/stage*/`)  | `.spl`, `.expl`, `.xsm` | Stage-wise solutions and sample programs                   |

---

## Stage Roadmap

### Preparatory stages

- Stage 1 — [Setting up the system](https://exposnitc.github.io/expos-docs/roadmap/stage-01/)
- Stage 2 — [Understanding the file system](https://exposnitc.github.io/expos-docs/roadmap/stage-02/)
- Stage 3 — [Bootstrap loader](https://exposnitc.github.io/expos-docs/roadmap/stage-03/)
- Stage 4 — [Learning the SPL language](https://exposnitc.github.io/expos-docs/roadmap/stage-04/)
- Stage 5 — [XSM debugging](https://exposnitc.github.io/expos-docs/roadmap/stage-05/)
- Stage 6 — [Running a user program](https://exposnitc.github.io/expos-docs/roadmap/stage-06/)
- Stage 7 — [ABI and XEXE format](https://exposnitc.github.io/expos-docs/roadmap/stage-07/)
- Stage 8 — [Handling timer interrupt](https://exposnitc.github.io/expos-docs/roadmap/stage-08/)
- Stage 9 — [Handling kernel stack](https://exposnitc.github.io/expos-docs/roadmap/stage-09/)
- Stage 10 — [Console output](https://exposnitc.github.io/expos-docs/roadmap/stage-10/)
- Stage 11 — [Introduction to ExpL](https://exposnitc.github.io/expos-docs/roadmap/stage-11/)
- Stage 12 — [Introduction to multiprogramming](https://exposnitc.github.io/expos-docs/roadmap/stage-12/)

### Intermediate stages

- Stage 13 — [Boot module](https://exposnitc.github.io/expos-docs/roadmap/stage-13/)
- Stage 14 — [Round robin scheduler](https://exposnitc.github.io/expos-docs/roadmap/stage-14/)
- Stage 15 — [Resource manager module](https://exposnitc.github.io/expos-docs/roadmap/stage-15/)
- Stage 16 — [Console input](https://exposnitc.github.io/expos-docs/roadmap/stage-16/)
- Stage 17 — [Program loader](https://exposnitc.github.io/expos-docs/roadmap/stage-17/)
- Stage 18 — [Disk interrupt handler](https://exposnitc.github.io/expos-docs/roadmap/stage-18/)
- Stage 19 — [Exception handler](https://exposnitc.github.io/expos-docs/roadmap/stage-19/)

### Final stages

- Stage 20 — [Process creation and termination](https://exposnitc.github.io/expos-docs/roadmap/stage-20/)
- Stage 21 — [Process synchronization](https://exposnitc.github.io/expos-docs/roadmap/stage-21/)
- Stage 22 — [Semaphores](https://exposnitc.github.io/expos-docs/roadmap/stage-22/)
- Stage 23 — [File creation and deletion](https://exposnitc.github.io/expos-docs/roadmap/stage-23/)
- Stage 24 — [File read](https://exposnitc.github.io/expos-docs/roadmap/stage-24/)
- Stage 25 — [File write](https://exposnitc.github.io/expos-docs/roadmap/stage-25/)
- Stage 26 — [User management](https://exposnitc.github.io/expos-docs/roadmap/stage-26/)
- Stage 27 — [Pager module](https://exposnitc.github.io/expos-docs/roadmap/stage-27/)
- Stage 28 — [Multi-core extension](https://exposnitc.github.io/expos-docs/roadmap/stage-28/)

---

## Prerequisites

Install the following on a Linux system:

- **GCC** (GNU C/C++ compiler)
- **Flex / Lex** (lexical analyzer generator)
- **Bison / Yacc** (parser generator)
- **libreadline-dev** (command-line completion for XFS interface)

```bash
# Debian / Ubuntu
sudo apt install build-essential flex bison libreadline-dev
```

---

## Compiling Programs

### SPL (kernel / OS code)

```bash
cd spl
./spl ../test/stage14/scheduler.spl    # produces scheduler.xsm
```

### ExpL (user programs)

```bash
cd expl
./expl-bin ../test/stage11/assg1.expl  # produces assg1.xsm
```

ExpL programs use `exposcall("SyscallName", …)` to invoke OS services. Link against a library (`.lib`) when required (see Stage 11).

---

## eXpOS Architecture

For the full XFS interface command syntax and semantics, see the [XFS Interface Usage Specification](https://exposnitc.github.io/support_tools-files/xfs-interface.html). Type `help` inside `./xfs-interface` for the in-tool command list.

The complete OS loaded by `Batchfile` consists of the following components:

### Kernel Modules

| Module                          | File (Stage 27)              | Responsibility                                                    |
| ------------------------------- | ---------------------------- | ----------------------------------------------------------------- |
| **Module 0** — Resource Manager | `test/stage24/module0.xsm`   | Acquire/release disk, buffer, inode, semaphore, terminal          |
| **Module 1** — Process Manager  | `test/stage25/module1.xsm`   | PCB allocation, exit, page-table cleanup, kill-all                |
| **Module 2** — Memory Manager   | `test/stage27/module2.xsm`   | Free-page allocation, release, disk block management, swap blocks |
| **Module 3** — File Manager     | `test/stage25/module3.xsm`   | Buffered read/write, open, close                                  |
| **Module 4** — Device Manager   | `test/stage23/module4.xsm`   | Disk store/load, terminal read/write                              |
| **Module 5** — Scheduler        | `test/stage27/scheduler.xsm` | Round-robin context switch; swapper daemon integration            |
| **Module 6** — Pager            | `test/stage27/module6.xsm`   | Swap-out / swap-in for demand paging                              |
| **Module 7** — Boot Module      | `test/stage27/module7.xsm`   | Loads all modules and interrupt routines into memory              |

### Interrupt Handlers

| Interrupt | File                                  | Handles                                                             |
| --------- | ------------------------------------- | ------------------------------------------------------------------- |
| Timer     | `test/stage27/timer.xsm`              | Preemptive scheduling via Module 5                                  |
| Disk      | `test/stage18/int2.xsm`               | Wake processes waiting on disk I/O                                  |
| Console   | `test/stage16/console_interrupt.xsm`  | Terminal input (Module 0 acquire/release)                           |
| INT 4     | `test/stage23/int4.xsm`               | Create, Delete file syscalls                                        |
| INT 5     | `test/stage25/int5.xsm`               | Open, Close file syscalls                                           |
| INT 6     | `test/stage24/int6.xsm`               | Read file syscall                                                   |
| INT 7     | `test/stage25/int7.xsm`               | Write file syscall                                                  |
| INT 8     | `test/stage24/int8.xsm`               | Seek file syscall                                                   |
| INT 9     | `test/stage19/int9.xsm`               | Exec syscall (program loader)                                       |
| INT 10    | `test/stage20/INT_10.xsm`             | Exit syscall                                                        |
| INT 11    | `test/stage21/int11.xsm`              | GetPid, GetPPid, Wait, Signal                                       |
| INT 12    | `test/stage26/int12.xsm`              | Fork, Exec syscalls                                                 |
| INT 13–14 | `test/stage22/int13.xsm`, `int14.xsm` | Semget, Semrelease, Semlock, Semunlock                              |
| INT 15    | `test/stage26/int15.xsm`              | Shutdown syscall                                                    |
| INT 16–17 | `test/stage26/int16.xsm`, `int17.xsm` | User management (Newusr, Remusr, Setpwd, Login, Logout, …)          |
| Exception | `test/stage19/exhandler.xsm`          | Stack overflow, illegal instruction, page faults, arithmetic errors |

### User Processes

| Process      | File                              | Role                                  |
| ------------ | --------------------------------- | ------------------------------------- |
| Idle         | `test/stage12/idle_process.xsm`   | Runs when no other process is ready   |
| Login (INIT) | `test/stage26/programs/login.xsm` | Authentication loop                   |
| Shell        | `test/stage26/programs/shell.xsm` | Command interpreter (Fork + Exec)     |
| Utilities    | `test/stage25/commands/`          | `ls`, `cat`, `cp`, `rm` ExpL programs |

---

## Useful Resources

- [Documentation — ExposNITC](https://exposnitc.github.io/expos-docs/)

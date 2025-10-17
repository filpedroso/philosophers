# Philosophers

## Overview

This project is part of the 42 curriculum.  
It explores **multithreading**, **process synchronization**, and **deadlock avoidance** by simulating the classic *Dining Philosophers* problem.

The goal is to implement a simulation where philosophers alternately **eat**, **sleep**, and **think**, without ever starving — using either **threads** (mandatory part) or **processes** (bonus part).

---

## 🧩 Mandatory Part (Threads)

Each philosopher is a **thread**.  
Forks' states are represented by **mutexes** to ensure proper synchronization and avoid data races.

### Key points
- No philosopher dies if rules are followed.
- Uses **pthread** library.
- Proper protection of shared data (no data races).
- Each action is timestamped and printed in order.

---

## 🚀 Bonus Part (Processes)

Each philosopher is a **process**, and semaphores are used instead of mutexes.

### Key points
- Processes communicate via **POSIX semaphores**.
- Forks and message printing are protected by semaphores.
- Each philosopher has its own process; a monitor process supervises death conditions.

---

## ⚙️ Usage

### Compilation
```bash
make        # builds
make clean  # removes object files
make fclean # removes all binaries
make re     # rebuilds everything

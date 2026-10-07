# CPU Scheduling Simulator

A discrete-time simulator of short-term CPU scheduling algorithms, written in C.

The simulator models a single-core processor and a set of CPU-bound processes
that arrive over time. The user selects a scheduling algorithm and the number
of processes, and the simulator reports per-process metrics and the resulting
schedule.

---

## Features

- Tick-based simulation (logical time, not wall-clock time).
- Single processor, CPU-bound processes only (no I/O, no blocking).
- Random process arrival (probabilistic per tick).
- Random burst time per process.
- Per-process metrics: arrival, priority, burst, start, finish, wait.

---

## Supported Algorithms

| # | Algorithm | Preemptive | Selection criterion              |
|---|-----------|------------|----------------------------------|
| 1 | FCFS      | No         | First to arrive                  |
| 2 | RR        | Yes        | FIFO with a configurable quantum |
| 3 | SPN       | No         | Shortest burst time              |
| 4 | PSPN      | Yes        | Shortest remaining time          |
| 5 | HPRN      | No         | Highest penalty ratio            |

---

## Build

Requirements:

- A C11 compiler (`gcc` or `clang`).
- `make`.

Compile the project:

```sh
make
```
---

## Usage

Run the simulator:
```sh
./sim
```
You will be prompted to:

1. Select an algorithm (1–5).
2. Enter the number of processes (1–27).
3. If the algorithm is preemptive, enter a quantum (1–256).

# 🖥️ CPU Cache Simulator
A high-performance, configurable CPU cache simulator written in C++17. Designed to model L1/L2 cache behavior, measure hit/miss rates, and simulate replacement policies (LRU, FIFO).

## 🚀 Features

- **Configurable Hierarchy:** Define Cache Size, Block Size, and Associativity via CLI.
- **Replacement Policies:** Supports **LRU** (Least Recently Used) and **FIFO** (First-In-First-Out).
- **Trace Driven:** Reads standard memory trace files (Valgrind format compatible).
- **Detailed Metrics:** Outputs Hits, Misses, Evictions, and Miss Rate %.
- **Unit Tested:** Includes automated test suite for validation.

## 🏗️ Architecture

The simulator maps 64-bit physical addresses to cache sets using the following formula:

```text
|       Tag       |   Set Index   |  Block Offset  |
|-----------------|---------------|----------------|
   64 - (i + o)        i bits          o bits

## 🛠️ Build Instructions

Requires CMake and a C++17 compatible compiler (g++ or clang).

```bash
mkdir build && cd build
cmake ..
make
```

## 📖 Usage

Run the simulator with command-line arguments:

```bash
./cache_sim -s <size> -b <block> -a <assoc> -p <policy> -t <trace>
```

### Example: 32KB 8-Way LRU Cache

```bash
./cache_sim -s 32768 -b 64 -a 8 -p LRU -t ../traces/sample.trace
```

### Sample Output

```text
=== Cache Statistics ===
Configuration: 32KB, 64B Block, 8-Way, LRU
Total Accesses: 10000
Hits: 8540
Misses: 1460
Miss Rate: 14.60%
Evictions: 1200
```

## 🧪 Testing

Run the unit test suite to verify core logic:

```bash
cd build
./test_runner
```

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

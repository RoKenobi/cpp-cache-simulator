#ifndef CACHE_SIM_H
#define CACHE_SIM_H

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

// Configuration Structure
struct CacheConfig {
    uint64_t size = 0;          // Total cache size in bytes
    uint64_t block_size = 0;    // Cache line size in bytes
    uint64_t associativity = 0; // Number of ways
    std::string policy = "LRU"; // Replacement policy
};

// Statistics Structure
struct CacheStats {
    uint64_t hits = 0;
    uint64_t misses = 0;
    uint64_t evictions = 0;
    
    double getMissRate() const {
        uint64_t total = hits + misses;
        return total ? (double)misses / total * 100.0 : 0.0;
    }
};

// Main Simulator Class
class CacheSim {
public:
    explicit CacheSim(CacheConfig config);
    
    // Simulate a memory access
    bool access(uint64_t address);
    
    // Get current statistics
    CacheStats getStats() const;
    
    // Print formatted statistics to stdout
    void printStats() const;

private:
    // Represents a single cache line
    struct Line {
        uint64_t tag = 0;
        bool valid = false;
        uint64_t lru_counter = 0; // Timestamp for LRU
        uint64_t fifo_index = 0;  // Index for FIFO
    };

    CacheConfig config;
    CacheStats stats;
    std::vector<std::vector<Line>> sets;
    uint64_t global_counter = 0;

    // Helper functions for address parsing
    uint64_t getSetIndex(uint64_t addr) const;
    uint64_t getTag(uint64_t addr) const;
    
    // Helper functions for replacement logic
    int findLine(uint64_t set_index, uint64_t tag) const;
    int findVictim(uint64_t set_index);
};

#endif

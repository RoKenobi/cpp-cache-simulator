#ifndef CACHE_SIM_H
#define CACHE_SIM_H

#include <vector>
#include <string>
#include <cstdint>

struct CacheConfig {
    uint64_t size;          // Total size in bytes
    uint64_t block_size;    // Line size in bytes
    uint64_t associativity; // Ways
    std::string policy;     // "LRU" or "FIFO"
};

struct CacheStats {
    uint64_t hits = 0;
    uint64_t misses = 0;
    uint64_t evictions = 0;
};

class CacheSim {
public:
    CacheSim(CacheConfig config);
    bool access(uint64_t address); // Returns true if hit
    CacheStats getStats() const;
    void printStats() const;

private:
    struct Line {
        uint64_t tag = 0;
        bool valid = false;
        uint64_t lru_counter = 0; // For LRU
        uint64_t fifo_index = 0;  // For FIFO
    };

    CacheConfig config;
    CacheStats stats;
    std::vector<std::vector<Line>> sets;
    uint64_t global_counter = 0;

    uint64_t getSetIndex(uint64_t addr);
    uint64_t getTag(uint64_t addr);
    int findLine(uint64_t set_index, uint64_t tag);
    int findVictim(uint64_t set_index);
};

#endif

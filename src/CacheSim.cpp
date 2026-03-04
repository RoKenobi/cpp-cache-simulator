#include "CacheSim.h"
#include <iostream>
#include <cmath>
#include <algorithm>

CacheSim::CacheSim(CacheConfig cfg) : config(cfg) {
    uint64_t num_sets = (config.size / config.block_size) / config.associativity;
    sets.resize(num_sets, std::vector<Line>(config.associativity));
}

uint64_t CacheSim::getSetIndex(uint64_t addr) {
    uint64_t offset_bits = log2(config.block_size);
    uint64_t num_sets = (config.size / config.block_size) / config.associativity;
    uint64_t index_bits = log2(num_sets);
    return (addr >> offset_bits) & ((1ULL << index_bits) - 1);
}

uint64_t CacheSim::getTag(uint64_t addr) {
    uint64_t offset_bits = log2(config.block_size);
    uint64_t num_sets = (config.size / config.block_size) / config.associativity;
    uint64_t index_bits = log2(num_sets);
    return addr >> (offset_bits + index_bits);
}

int CacheSim::findLine(uint64_t set_index, uint64_t tag) {
    for (size_t i = 0; i < sets[set_index].size(); ++i) {
        if (sets[set_index][i].valid && sets[set_index][i].tag == tag) {
            return i;
        }
    }
    return -1;
}

int CacheSim::findVictim(uint64_t set_index) {
    if (config.policy == "FIFO") {
        // Simple round robin for demo
        return stats.evictions % config.associativity; 
    } else { // LRU
        int victim = 0;
        uint64_t min_counter = sets[set_index][0].lru_counter;
        for (size_t i = 1; i < sets[set_index].size(); ++i) {
            if (sets[set_index][i].lru_counter < min_counter) {
                min_counter = sets[set_index][i].lru_counter;
                victim = i;
            }
        }
        return victim;
    }
}

bool CacheSim::access(uint64_t address) {
    global_counter++;
    uint64_t set_idx = getSetIndex(address);
    uint64_t tag = getTag(address);

    int line_idx = findLine(set_idx, tag);

    if (line_idx != -1) {
        // Hit
        stats.hits++;
        if (config.policy == "LRU") {
            sets[set_idx][line_idx].lru_counter = global_counter;
        }
        return true;
    } else {
        // Miss
        stats.misses++;
        int victim_idx = -1;
        
        // Find empty line first
        for (size_t i = 0; i < sets[set_idx].size(); ++i) {
            if (!sets[set_idx][i].valid) {
                victim_idx = i;
                break;
            }
        }

        // If full, evict
        if (victim_idx == -1) {
            stats.evictions++;
            victim_idx = findVictim(set_idx);
        }

        sets[set_idx][victim_idx].tag = tag;
        sets[set_idx][victim_idx].valid = true;
        sets[set_idx][victim_idx].lru_counter = global_counter;
        return false;
    }
}

CacheStats CacheSim::getStats() const { return stats; }

void CacheSim::printStats() const {
    uint64_t total = stats.hits + stats.misses;
    double miss_rate = total ? (double)stats.misses / total * 100.0 : 0.0;
    std::cout << "=== Cache Statistics ===" << std::endl;
    std::cout << "Hits: " << stats.hits << std::endl;
    std::cout << "Misses: " << stats.misses << std::endl;
    std::cout << "Miss Rate: " << miss_rate << "%" << std::endl;
}

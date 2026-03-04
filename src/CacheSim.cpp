#include "CacheSim.h"
#include <cmath>
#include <algorithm>
#include <iomanip>

CacheSim::CacheSim(CacheConfig cfg) : config(cfg) {
    if (config.size == 0 || config.block_size == 0 || config.associativity == 0) {
        throw std::invalid_argument("Invalid cache configuration");
    }

    // Calculate number of sets
    uint64_t num_sets = (config.size / config.block_size) / config.associativity;
    
    // Initialize cache sets
    sets.resize(num_sets, std::vector<Line>(config.associativity));
}

uint64_t CacheSim::getSetIndex(uint64_t addr) const {
    uint64_t offset_bits = static_cast<uint64_t>(log2(config.block_size));
    uint64_t num_sets = (config.size / config.block_size) / config.associativity;
    uint64_t index_bits = static_cast<uint64_t>(log2(num_sets));
    
    // Mask to extract index bits
    uint64_t mask = (1ULL << index_bits) - 1;
    return (addr >> offset_bits) & mask;
}

uint64_t CacheSim::getTag(uint64_t addr) const {
    uint64_t offset_bits = static_cast<uint64_t>(log2(config.block_size));
    uint64_t num_sets = (config.size / config.block_size) / config.associativity;
    uint64_t index_bits = static_cast<uint64_t>(log2(num_sets));
    
    return addr >> (offset_bits + index_bits);
}

int CacheSim::findLine(uint64_t set_index, uint64_t tag) const {
    const auto& set = sets[set_index];
    for (size_t i = 0; i < set.size(); ++i) {
        if (set[i].valid && set[i].tag == tag) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int CacheSim::findVictim(uint64_t set_index) {
    auto& set = sets[set_index];
    
    if (config.policy == "FIFO") {
        // Simple FIFO: evict based on insertion order (simulated by index here for simplicity)
        // In a real impl, you'd track a head pointer.
        return stats.evictions % config.associativity;
    } else { // LRU
        int victim = 0;
        uint64_t min_counter = set[0].lru_counter;
        
        for (size_t i = 1; i < set.size(); ++i) {
            if (set[i].lru_counter < min_counter) {
                min_counter = set[i].lru_counter;
                victim = static_cast<int>(i);
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
        // HIT
        stats.hits++;
        if (config.policy == "LRU") {
            sets[set_idx][line_idx].lru_counter = global_counter;
        }
        return true;
    } else {
        // MISS
        stats.misses++;
        int victim_idx = -1;
        
        // 1. Try to find an empty line
        for (size_t i = 0; i < sets[set_idx].size(); ++i) {
            if (!sets[set_idx][i].valid) {
                victim_idx = static_cast<int>(i);
                break;
            }
        }

        // 2. If full, evict a victim
        if (victim_idx == -1) {
            stats.evictions++;
            victim_idx = findVictim(set_idx);
        }

        // Update the line
        sets[set_idx][victim_idx].tag = tag;
        sets[set_idx][victim_idx].valid = true;
        sets[set_idx][victim_idx].lru_counter = global_counter;
        
        return false;
    }
}

CacheStats CacheSim::getStats() const { return stats; }

void CacheSim::printStats() const {
    std::cout << "=== Cache Statistics ===" << std::endl;
    std::cout << "Config: " << config.size/1024 << "KB, " 
              << config.block_size << "B Block, " 
              << config.associativity << "-Way, " 
              << config.policy << std::endl;
    std::cout << "Total Accesses: " << (stats.hits + stats.misses) << std::endl;
    std::cout << "Hits: " << stats.hits << std::endl;
    std::cout << "Misses: " << stats.misses << std::endl;
    std::cout << "Miss Rate: " << std::fixed << std::setprecision(2) << stats.getMissRate() << "%" << std::endl;
    std::cout << "Evictions: " << stats.evictions << std::endl;
}

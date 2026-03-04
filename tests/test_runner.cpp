#include <iostream>
#include <cassert>
#include "CacheSim.h"

void test_basic_write_read() {
    CacheConfig cfg {32768, 64, 8, "LRU"};
    CacheSim sim(cfg);
    
    // Access address 0x1000
    bool miss1 = !sim.access(0x1000);
    // Access same address again (should hit)
    bool hit1 = sim.access(0x1000);
    
    assert(miss1 && "First access should be a miss");
    assert(hit1 && "Second access should be a hit");
    std::cout << "[PASS] test_basic_write_read" << std::endl;
}

void test_eviction() {
    // Tiny cache: 1 set, 1 way, 64 byte block
    CacheConfig cfg {64, 64, 1, "LRU"};
    CacheSim sim(cfg);
    
    sim.access(0x0000); // Fill line
    sim.access(0x0040); // Evict previous (different tag, same set)
    
    // Check stats manually via access behavior
    // Accessing 0x0000 again should be a miss because it was evicted
    bool miss_after_evict = !sim.access(0x0000);
    
    assert(miss_after_evict && "Access after eviction should be a miss");
    std::cout << "[PASS] test_eviction" << std::endl;
}

int main() {
    std::cout << "Running Cache Simulator Tests..." << std::endl;
    try {
        test_basic_write_read();
        test_eviction();
        std::cout << "All Tests Passed." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test Failed: " << e.what() << std::endl;
        return 1;
    }
}

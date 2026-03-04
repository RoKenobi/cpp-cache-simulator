#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "CacheSim.h"

void printUsage() {
    std::cout << "Usage: ./cache_sim -s <size> -b <block> -a <assoc> -p <policy> -t <trace>" << std::endl;
    std::cout << "Example: ./cache_sim -s 32768 -b 64 -a 8 -p LRU -t trace.txt" << std::endl;
}

int main(int argc, char** argv) {
    CacheConfig cfg;
    std::string traceFile;

    // Simple CLI parsing
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-s" && i+1 < argc) cfg.size = std::stoul(argv[++i]);
        else if (arg == "-b" && i+1 < argc) cfg.block_size = std::stoul(argv[++i]);
        else if (arg == "-a" && i+1 < argc) cfg.associativity = std::stoul(argv[++i]);
        else if (arg == "-p" && i+1 < argc) cfg.policy = argv[++i];
        else if (arg == "-t" && i+1 < argc) traceFile = argv[++i];
    }

    if (cfg.size == 0 || traceFile.empty()) {
        printUsage();
        return 1;
    }

    CacheSim sim(cfg);
    std::ifstream trace(traceFile);
    std::string line;

    while (std::getline(trace, line)) {
        if (line.empty() || line[0] == '#') continue;
        // Assume trace format: "R 0x1234" or just "0x1234"
        uint64_t addr = std::stoull(line.substr(line.find("0x")), nullptr, 16);
        sim.access(addr);
    }

    sim.printStats();
    return 0;
}

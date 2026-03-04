#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <getopt.h>
#include "CacheSim.h"

void printUsage(const char* progname) {
    std::cout << "Usage: " << progname << " -s <size> -b <block> -a <assoc> -p <policy> -t <trace>" << std::endl;
    std::cout << "  -s : Cache size in bytes (e.g., 32768)" << std::endl;
    std::cout << "  -b : Block size in bytes (e.g., 64)" << std::endl;
    std::cout << "  -a : Associativity (e.g., 8)" << std::endl;
    std::cout << "  -p : Policy (LRU or FIFO)" << std::endl;
    std::cout << "  -t : Trace file path" << std::endl;
}

int main(int argc, char** argv) {
    CacheConfig cfg;
    std::string traceFile;
    int opt;

    while ((opt = getopt(argc, argv, "s:b:a:p:t:")) != -1) {
        switch (opt) {
            case 's': cfg.size = std::stoul(optarg); break;
            case 'b': cfg.block_size = std::stoul(optarg); break;
            case 'a': cfg.associativity = std::stoul(optarg); break;
            case 'p': cfg.policy = optarg; break;
            case 't': traceFile = optarg; break;
            default:
                printUsage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (cfg.size == 0 || traceFile.empty()) {
        std::cerr << "Error: Missing required arguments." << std::endl;
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    try {
        CacheSim sim(cfg);
        std::ifstream trace(traceFile);
        
        if (!trace.is_open()) {
            std::cerr << "Error: Could not open trace file: " << traceFile << std::endl;
            return EXIT_FAILURE;
        }

        std::string line;
        uint64_t lines_processed = 0;
        
        while (std::getline(trace, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            // Parse hex address (supports "0x1234" or "1234")
            size_t pos = line.find("0x");
            uint64_t addr = std::stoull(line.substr(pos), nullptr, 16);
            
            sim.access(addr);
            lines_processed++;
        }

        std::cout << "Processed " << lines_processed << " memory accesses." << std::endl;
        sim.printStats();

    } catch (const std::exception& e) {
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

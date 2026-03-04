#!/usr/bin/env python3
import random

def generate_trace(filename, num_lines=1000):
    with open(filename, 'w') as f:
        f.write("# Generated Trace\n")
        for _ in range(num_lines):
            # Generate random 64-bit aligned address
            addr = random.randint(0, 0xFFFFFFFF) & ~0x3F
            f.write(f"0x{addr:08X}\n")
    print(f"Generated {num_lines} lines in {filename}")

if __name__ == "__main__":
    generate_trace("traces/random.trace")

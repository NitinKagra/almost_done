#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include "config.h"

void generateRandomAddresses(size_t startAddress, size_t sectionSize, size_t pageSize, std::vector<std::pair<size_t, size_t>> &addresses) {
    size_t alignedStart = startAddress;
    size_t alignedEnd = alignedStart + sectionSize - pageSize;

    // Random number of allocations
    size_t numAllocations = rand() % 10 + 1; // Randomly between 1 and 10 allocations

    for (size_t i = 0; i < numAllocations; ++i) {
        size_t address = alignedStart + (rand() % (alignedEnd - alignedStart + 1));
        size_t size = (rand() % 10 + 1) * pageSize; // Random size between 1 and 10 pages
        addresses.push_back({address, size});
    }
}

void writeTraceFile(const std::string &filename, size_t numTasks) {
    std::ofstream traceFile(filename);

    if (!traceFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::vector<std::pair<size_t, size_t>> addresses;

    // Seed random number generator
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    for (size_t taskID = 1; taskID <= numTasks; ++taskID) {
        addresses.clear();

        // Generate random addresses for each section
        generateRandomAddresses(TEXT_SECTION_START, TEXT_SECTION_SIZE, PAGE_SIZE_1KB, addresses);
        generateRandomAddresses(DATA_SECTION_START, DATA_SECTION_SIZE, PAGE_SIZE_1KB, addresses);
        generateRandomAddresses(STACK_SECTION_START, STACK_SECTION_SIZE, PAGE_SIZE_1KB, addresses);
        generateRandomAddresses(SHARED_LIB_SECTION_START, SHARED_LIB_SECTION_SIZE, PAGE_SIZE_1KB, addresses);
        generateRandomAddresses(HEAP_SECTION_START, HEAP_SECTION_SIZE, PAGE_SIZE_1KB, addresses);

        // Write addresses to trace file
        for (const auto &entry : addresses) {
            size_t address = entry.first;
            size_t size = entry.second;
            std::string sizeStr;

            // Convert size to KB or MB
            if (size >= 1024 * 1024) {
                sizeStr = std::to_string(size / (1024 * 1024)) + "MB";
            } else if (size >= 1024) {
                sizeStr = std::to_string(size / 1024) + "KB";
            } else {
                sizeStr = std::to_string(size) + "B";
            }

            traceFile << "T" << taskID << ":" << std::hex << address << ":" << sizeStr << std::endl;
        }
    }

    traceFile.close();
}

int main() {
    size_t numTasks;
    std::cout << "Enter the number of tasks: ";
    std::cin >> numTasks;

    if (numTasks < 1) {
        std::cerr << "Number of tasks must be at least 1." << std::endl;
        return 1;
    }

    std::string filename = "tracefile.txt";
    writeTraceFile(filename, numTasks);

    std::cout << "Trace file generated: " << filename << std::endl;
    return 0;
}


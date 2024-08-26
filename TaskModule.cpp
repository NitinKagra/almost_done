#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip> // For hex printing
#include <sstream> // For stringstream
#include "MemoryManager.cpp"
#include "config.h"


enum PageTableType {
    MAP,
    SINGLE_LEVEL
};

class Task {
private:
    PageTableType tableType;
    union {
        std::unordered_map<size_t, size_t>* mapPageTable; // Pointer to map-based page table
        std::vector<size_t>* singleLevelPageTable; // Pointer to single-level page table
    };
    MemoryManager& memoryManager;
    size_t taskID; // Unique ID for each task
    size_t pageSize; // Size of each physical page in KB
    size_t numPages; // Number of pages in single-level page table (if applicable)
    size_t pageHits; // Counter for page hits

public:
    // Constructor for map-based page table
    Task(size_t id, MemoryManager& memManager, size_t pageSizeKB)
        : tableType(MAP), memoryManager(memManager), taskID(id), pageSize(pageSizeKB), numPages(0), pageHits(0) {
        mapPageTable = new std::unordered_map<size_t, size_t>();
    }

    // Constructor for single-level page table
    Task(size_t id, MemoryManager& memManager, size_t pageSizeKB, size_t numPages)
        : tableType(SINGLE_LEVEL), memoryManager(memManager), taskID(id), pageSize(pageSizeKB), numPages(numPages), pageHits(0) {
        singleLevelPageTable = new std::vector<size_t>(numPages, SIZE_MAX);
    }

    // Destructor
    ~Task() {
        if (tableType == MAP) {
            delete mapPageTable;
        } else if (tableType == SINGLE_LEVEL) {
            delete singleLevelPageTable;
        }
    }

    // Function to request memory for a virtual address and size
    void requestMemory(size_t virtualAddress, size_t sizeKB) {
        size_t numPagesNeeded = (sizeKB + pageSize - 1) / pageSize; // Calculate the number of pages needed

        size_t startPage = virtualAddress / pageSize;
        size_t endPage = startPage + numPagesNeeded - 1;

        if (tableType == MAP) {
            for (size_t i = startPage; i <= endPage; ++i) {
                auto it = mapPageTable->find(i);
                if (it != mapPageTable->end()) { // Page is already allocated
                    ++pageHits; // Increment page hit counter
                } else { // Page not yet allocated
                    size_t physicalPage = memoryManager.allocatePage();
                    (*mapPageTable)[i] = physicalPage;
                }
            }
        } else if (tableType == SINGLE_LEVEL) {
            for (size_t i = startPage; i <= endPage; ++i) {
                if ((*singleLevelPageTable)[i] != SIZE_MAX) { // Page is already allocated
                    ++pageHits; // Increment page hit counter
                } else { // Page not yet allocated
                    size_t physicalPage = memoryManager.allocatePage();
                    (*singleLevelPageTable)[i] = physicalPage;
                }
            }
        }

        std::cout << "Task " << taskID << " allocated pages for virtual address "
                  << std::hex << std::showbase << std::setw(4) << std::setfill('0') << virtualAddress
                  << std::dec << std::endl;
    }

    // Function to free the physical pages associated with a virtual address
    void releaseMemory(size_t virtualAddress, size_t sizeKB) {
        size_t numPagesNeeded = (sizeKB + pageSize - 1) / pageSize;
        size_t startPage = virtualAddress / pageSize;
        size_t endPage = startPage + numPagesNeeded - 1;

        if (tableType == MAP) {
            for (size_t i = startPage; i <= endPage; ++i) {
                auto it = mapPageTable->find(i);
                if (it != mapPageTable->end()) { // Page is allocated
                    memoryManager.freePage(it->second);
                    mapPageTable->erase(it);
                }
            }
            std::cout << "Task " << taskID << " released memory for virtual address "
                      << std::hex << std::showbase << std::setw(4) << std::setfill('0') << virtualAddress
                      << std::dec << std::endl;
        } else if (tableType == SINGLE_LEVEL) {
            for (size_t i = startPage; i <= endPage; ++i) {
                if ((*singleLevelPageTable)[i] != SIZE_MAX) { // Page is allocated
                    memoryManager.freePage((*singleLevelPageTable)[i]);
                    (*singleLevelPageTable)[i] = SIZE_MAX;
                }
            }
            std::cout << "Task " << taskID << " released memory for virtual address "
                      << std::hex << std::showbase << std::setw(4) << std::setfill('0') << virtualAddress
                      << std::dec << std::endl;
        }
    }

    // Function to print the current page table for the task
    void printPageTable() const {
        std::cout << "Task " << taskID << " Page Table:" << std::endl;
        if (tableType == MAP) {
            for (const auto& entry : *mapPageTable) {
                std::cout << "Virtual Page: " << entry.first << " -> Physical Page: "
                          << std::hex << std::showbase << std::setw(4) << std::setfill('0') << entry.second
                          << std::dec << std::endl;
            }
        } else if (tableType == SINGLE_LEVEL) {
            for (size_t i = 0; i < singleLevelPageTable->size(); ++i) {
                if ((*singleLevelPageTable)[i] != SIZE_MAX) {
                    std::cout << "Virtual Page: " << i << " -> Physical Page: "
                              << std::hex << std::showbase << std::setw(4) << std::setfill('0') << (*singleLevelPageTable)[i]
                              << std::dec << std::endl;
                }
            }
        }
    }

    // Calculate page table memory in pages
    size_t getPageTableMemory() const {
        if (tableType == MAP) {
            return mapPageTable->size(); // In the case of map, it only shows the number of entries, not the total pages
        } else if (tableType == SINGLE_LEVEL) {
            return singleLevelPageTable->size();
        }
        return 0;
    }

    // Check if a virtual address is a page hit
    bool isPageHit(size_t virtualAddress) const {
        size_t pageIndex = virtualAddress / pageSize;
        if (tableType == MAP) {
            return mapPageTable->find(pageIndex) != mapPageTable->end();
        } else if (tableType == SINGLE_LEVEL) {
            return (*singleLevelPageTable)[pageIndex] != SIZE_MAX;
        }
        return false;
    }

    // Get the total number of page hits
    size_t getPageHits() const {
        return pageHits;
    }
};



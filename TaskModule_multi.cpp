#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>
#include "MemoryManager.cpp"
#include "config.h"
// Define constants
const size_t PAGE_SIZE = PAGE_SIZE_1KB;
const size_t TOP_LEVEL_ENTRIES = 1024;
const size_t SECOND_LEVEL_ENTRIES = 256;

class MultilevelPageTable {
private:
    std::unordered_map<size_t, std::unique_ptr<std::unordered_map<size_t, size_t>>> topLevelTable;
    std::unordered_map<size_t, bool> accessedAddresses; // Track accessed addresses
    MemoryManager& memoryManager;
    size_t pageSize;
    size_t pageHits; // Counter for page hits

public:
    MultilevelPageTable(MemoryManager& memManager, size_t pageSize = PAGE_SIZE)
        : memoryManager(memManager), pageSize(pageSize), pageHits(0) {}

    size_t translateAddress(size_t virtualAddress) {
        size_t topLevelIndex = (virtualAddress >> 22) & 0x3FF; // Extract top-level index
        size_t secondLevelIndex = (virtualAddress >> 12) & 0x3FF; // Extract second-level index
        size_t offset = virtualAddress & 0xFFF; // Extract offset

        // Check if top-level entry exists
        if (topLevelTable.find(topLevelIndex) == topLevelTable.end()) {
            throw std::runtime_error("Page table entry not found.");
        }

        auto& secondLevelTable = topLevelTable[topLevelIndex];
        // Check if second-level entry exists
        if (secondLevelTable->find(secondLevelIndex) == secondLevelTable->end()) {
            throw std::runtime_error("Page table entry not found.");
        }

        size_t physicalPage = (*secondLevelTable)[secondLevelIndex];
        size_t physicalAddress = (physicalPage << 12) | offset;

        // Track page hits
        if (accessedAddresses.find(virtualAddress) != accessedAddresses.end()) {
            ++pageHits; // Increment hit count
        } else {
            accessedAddresses[virtualAddress] = true; // Mark address as accessed
        }

        return physicalAddress;
    }

    void mapAddress(size_t virtualAddress, size_t physicalPage) {
        size_t topLevelIndex = (virtualAddress >> 22) & 0x3FF;
        size_t secondLevelIndex = (virtualAddress >> 12) & 0x3FF;

        if (topLevelTable.find(topLevelIndex) == topLevelTable.end()) {
            topLevelTable[topLevelIndex] = std::make_unique<std::unordered_map<size_t, size_t>>();
        }

        (*topLevelTable[topLevelIndex])[secondLevelIndex] = physicalPage;
    }

    void allocateAndMapAddress(size_t virtualAddress, size_t size) {
        size_t numPages = (size + pageSize - 1) / pageSize; // Calculate number of pages needed
        std::vector<size_t> physicalPages = memoryManager.allocatePages(numPages);

        for (size_t i = 0; i < numPages; ++i) {
            mapAddress(virtualAddress + i * pageSize, physicalPages[i]);
        }
    }

    void freePagesForAddress(size_t virtualAddress, size_t size) {
        size_t numPages = (size + pageSize - 1) / pageSize; // Calculate number of pages
        std::vector<size_t> pagesToFree;

        for (size_t i = 0; i < numPages; ++i) {
            size_t topLevelIndex = (virtualAddress >> 22) & 0x3FF;
            size_t secondLevelIndex = (virtualAddress >> 12) & 0x3FF;

            if (topLevelTable.find(topLevelIndex) == topLevelTable.end()) {
                continue;
            }

            auto& secondLevelTable = topLevelTable[topLevelIndex];
            if (secondLevelTable->find(secondLevelIndex) != secondLevelTable->end()) {
                size_t physicalPage = (*secondLevelTable)[secondLevelIndex];
                pagesToFree.push_back(physicalPage);
                secondLevelTable->erase(secondLevelIndex);
            }

            virtualAddress += pageSize;
        }

        // Clean up empty second-level tables
        for (auto it = topLevelTable.begin(); it != topLevelTable.end();) {
            if (it->second->empty()) {
                it = topLevelTable.erase(it);
            } else {
                ++it;
            }
        }

        memoryManager.freeMultiplePages(pagesToFree);
    }

    void printPageTable() const {
        std::cout << "Page Table:" << std::endl;
        for (const auto& topLevelEntry : topLevelTable) {
            size_t topLevelIndex = topLevelEntry.first;
            const auto& secondLevelTable = topLevelEntry.second;
            std::cout << "Top-Level Index: " << topLevelIndex << std::endl;
            for (const auto& secondLevelEntry : *secondLevelTable) {
                size_t secondLevelIndex = secondLevelEntry.first;
                size_t physicalPage = secondLevelEntry.second;
                std::cout << "  Second-Level Index: " << secondLevelIndex
                          << " -> Physical Page: " << physicalPage << std::endl;
            }
        }
    }

    size_t getPageHits() const {
        return pageHits;
    }
};


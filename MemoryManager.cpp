#include <iostream>
#include <vector>
#include <queue>
#include <stdexcept>

class MemoryManager {
private:
    std::queue<size_t> freePages; // Queue to store free page indices
    size_t totalPages; // Total number of physical pages

public:
    MemoryManager(size_t numPages) : totalPages(numPages) {
        // Initially, all pages are free
        for (size_t i = 0; i < numPages; ++i) {
            freePages.push(i);
        }
    }

    // Function to allocate a physical page
    size_t allocatePage() {
        if (freePages.empty()) {
            throw std::runtime_error("No free pages available.");
        }

        size_t pageIndex = freePages.front();
        freePages.pop();
        return pageIndex;
    }

    // Function to allocate multiple pages
    std::vector<size_t> allocatePages(size_t numPages) {
        if (numPages > freePages.size()) {
            throw std::runtime_error("Not enough free pages available.");
        }

        std::vector<size_t> allocatedPages;
        for (size_t i = 0; i < numPages; ++i) {
            allocatedPages.push_back(allocatePage());
        }
        return allocatedPages;
    }

    // Function to free a physical page
    void freePage(size_t pageIndex) {
        if (pageIndex >= totalPages) {
            throw std::invalid_argument("Invalid page index.");
        }

        freePages.push(pageIndex);
    }

    // Function to free multiple pages
    void freeMultiplePages(const std::vector<size_t>& pages) {
        for (size_t pageIndex : pages) {
            freePage(pageIndex);
        }
    }

    // Function to check how many pages are free
    size_t availablePages() const {
        return freePages.size();
    }

    // Function to check total number of pages
    size_t getTotalPages() const {
        return totalPages;
    }
};

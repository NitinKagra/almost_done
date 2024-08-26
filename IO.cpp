#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct TraceEntry {
    int taskID;
    size_t virtualAddress;
    size_t size;
};

std::vector<TraceEntry> readTraceFile(const std::string &filename) {
    std::vector<TraceEntry> traceEntries;
    std::ifstream traceFile(filename);

    if (!traceFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return traceEntries;
    }

    std::string line;
    while (std::getline(traceFile, line)) {
        std::istringstream iss(line);
        std::string token;
        
        TraceEntry entry;

        // Parsing task ID
        std::getline(iss, token, ':');
        entry.taskID = std::stoi(token.substr(1));  // Removing 'T' and converting to int

        // Parsing virtual address
        std::getline(iss, token, ':');
        entry.virtualAddress = std::stoul(token, nullptr, 16);  // Converting hex string to size_t

        // Parsing size
        std::getline(iss, token, ' ');
        size_t sizeValue = std::stoul(token.substr(0, token.size() - 2));  // Removing 'KB' and converting to size_t
        entry.size = sizeValue;

        traceEntries.push_back(entry);
    }

    traceFile.close();
    return traceEntries;
}

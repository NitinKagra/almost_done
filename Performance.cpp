#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include "TaskModule.cpp"
#include "TaskModule_multi.cpp"
// Including the IO.cpp file directly
#include "IO.cpp"

// Function to process the trace entries
void processTraceEntries(const std::vector<TraceEntry> &entries) {
    for(auto it: TraceEntry){
	    std::cout<<it.id;}
}

int main() {
    std::string filename = "tracefile.txt";
    std::vector<TraceEntry> traceEntries = readTraceFile(filename);

    if (!traceEntries.empty()) {
        processTraceEntries(traceEntries);
    } else {
        std::cerr << "No trace entries were read from the file." << std::endl;
    }


    return 0;
}


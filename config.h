#ifndef CONFIG_H
#define CONFIG_H

// Define page sizes in bytes
const size_t PAGE_SIZE_1KB = 1024;          // 1 KB
const size_t PAGE_SIZE_2KB = 2048;          // 2 KB
const size_t PAGE_SIZE_4KB = 4096;          // 4 KB
const size_t PAGE_SIZE_8KB = 8192;          // 8 KB
const size_t PAGE_SIZE_16KB = 16384;        // 16 KB
const size_t PAGE_SIZE_32KB = 32768;        // 32 KB

// Define virtual and physical memory sizes (in bytes)
const size_t VIRTUAL_MEMORY_SIZE = 1 << 24; // 16 MB (e.g., 2^24 bytes)
const size_t PHYSICAL_MEMORY_SIZE = 1 << 23; // 8 MB (e.g., 2^23 bytes)

// Page table configurations
const size_t SINGLE_LEVEL_PAGE_TABLE_ENTRIES = 1024; // Number of entries in single-level page table
const size_t TOP_LEVEL_PAGE_TABLE_ENTRIES = 256;    // Number of entries in top-level page table for multi-level page tables
const size_t SECOND_LEVEL_PAGE_TABLE_ENTRIES = 256; // Number of entries in second-level page table for multi-level page tables

// Starting addresses (aligned to page size boundaries)
const size_t TEXT_SECTION_START = 0x10000000;   // Example starting address for text section
const size_t DATA_SECTION_START = 0x20000000;   // Example starting address for data section
const size_t STACK_SECTION_START = 0x30000000;  // Example starting address for stack section
const size_t SHARED_LIB_SECTION_START = 0x40000000; // Example starting address for shared library section
const size_t HEAP_SECTION_START = 0x50000000;   // Example starting address for heap section

// Memory sizes for sections (in bytes)
const size_t TEXT_SECTION_SIZE = 1 << 20;   // 1 MB
const size_t DATA_SECTION_SIZE = 1 << 20;   // 1 MB
const size_t STACK_SECTION_SIZE = 1 << 20;  // 1 MB
const size_t SHARED_LIB_SECTION_SIZE = 1 << 20; // 1 MB
const size_t HEAP_SECTION_SIZE = 1 << 20;   // 1 MB

#endif // CONFIG_H


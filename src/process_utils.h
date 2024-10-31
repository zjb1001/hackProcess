#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// Structure to hold process information
typedef struct {
    pid_t pid;
    char name[256];
    unsigned long addr;
    size_t size;
} ProcessInfo;

// Function declarations
ProcessInfo* find_process_by_name(const char* process_name);
void* map_process_memory(ProcessInfo* proc_info);
int unmap_process_memory(void* mapped_addr, size_t size);
unsigned long find_memory_region(pid_t pid);

#endif // PROCESS_UTILS_H
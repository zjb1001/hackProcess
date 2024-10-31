#include "process_utils.h"
#include <ctype.h>

ProcessInfo* find_process_by_name(const char* process_name) {
    DIR* proc_dir;
    struct dirent* entry;
    char path[512];
    char buffer[256];
    ProcessInfo* info = NULL;

    proc_dir = opendir("/proc");
    if (proc_dir == NULL) {
        perror("opendir");
        return NULL;
    }

    info = (ProcessInfo*)malloc(sizeof(ProcessInfo));
    if (!info) {
        closedir(proc_dir);
        return NULL;
    }

    while ((entry = readdir(proc_dir)) != NULL) {
        // Check if the directory entry is a process ID (numeric)
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name);
            
            FILE* comm_file = fopen(path, "r");
            if (comm_file) {
                if (fgets(buffer, sizeof(buffer), comm_file)) {
                    // Remove trailing newline
                    buffer[strcspn(buffer, "\n")] = 0;
                    
                    if (strcmp(buffer, process_name) == 0) {
                        info->pid = atoi(entry->d_name);
                        strncpy(info->name, process_name, sizeof(info->name) - 1);
                        info->addr = find_memory_region(info->pid);
                        info->size = 4096; // Default page size, adjust as needed
                        fclose(comm_file);
                        closedir(proc_dir);
                        return info;
                    }
                }
                fclose(comm_file);
            }
        }
    }

    free(info);
    closedir(proc_dir);
    return NULL;
}

unsigned long find_memory_region(pid_t pid) {
    char path[512];
    char line[256];
    unsigned long addr = 0;
    
    snprintf(path, sizeof(path), "/proc/%d/maps", pid);
    FILE* maps_file = fopen(path, "r");
    
    if (!maps_file) {
        perror("fopen maps");
        return 0;
    }

    // Read the first readable/writable memory region
    while (fgets(line, sizeof(line), maps_file)) {
        unsigned long start, end;
        char perms[5];
        
        if (sscanf(line, "%lx-%lx %4s", &start, &end, perms) == 3) {
            if (strstr(perms, "rw")) {
                addr = start;
                break;
            }
        }
    }
    
    fclose(maps_file);
    return addr;
}

void* map_process_memory(ProcessInfo* proc_info) {
    char path[512];
    void* mapped_addr;
    
    snprintf(path, sizeof(path), "/proc/%d/mem", proc_info->pid);
    int fd = open(path, O_RDWR);
    if (fd == -1) {
        perror("open mem");
        return NULL;
    }

    mapped_addr = mmap(NULL, proc_info->size, 
                      PROT_READ | PROT_WRITE, 
                      MAP_SHARED, 
                      fd, 
                      proc_info->addr);
    
    close(fd);
    
    if (mapped_addr == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    
    return mapped_addr;
}

int unmap_process_memory(void* mapped_addr, size_t size) {
    if (munmap(mapped_addr, size) == -1) {
        perror("munmap");
        return -1;
    }
    return 0;
}
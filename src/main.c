#include <stdio.h>
#include <stdlib.h>
#include "process_utils.h"

int main() {
    const char* process_name = "hello";
    
    printf("Looking for process: %s\n", process_name);
    
    // Find process by name
    ProcessInfo* target = find_process_by_name(process_name);
    if (!target) {
        fprintf(stderr, "Process '%s' not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    printf("Found process: %s (PID: %d)\n", target->name, target->pid);
    printf("Memory region: 0x%lx (size: %zu)\n", target->addr, target->size);

    // Map the process memory
    void* mapped_memory = map_process_memory(target);
    if (!mapped_memory) {
        free(target);
        return EXIT_FAILURE;
    }

    // Read and modify the counter value
    int* counter_ptr = (int*)mapped_memory;
    printf("Current counter value: %d\n", *counter_ptr);
    
    // Modify the counter
    *counter_ptr = 1000;
    printf("Modified counter value to: %d\n", *counter_ptr);
    
    // Wait a bit and read again to verify
    sleep(1);
    printf("Verifying counter value: %d\n", *counter_ptr);

    // Cleanup
    unmap_process_memory(mapped_memory, target->size);
    free(target);

    return EXIT_SUCCESS;
}
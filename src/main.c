#include <stdio.h>
#include <stdlib.h>
#include "process_utils.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <process_name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Find process by name
    ProcessInfo* target = find_process_by_name(argv[1]);
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

    // Example: Read and modify an integer value
    int* value_ptr = (int*)mapped_memory;
    printf("Current value at address 0x%lx: %d\n", target->addr, *value_ptr);
    
    // Modify the value
    *value_ptr = 42;
    printf("Modified value to: %d\n", *value_ptr);

    // Cleanup
    unmap_process_memory(mapped_memory, target->size);
    free(target);

    return EXIT_SUCCESS;
}
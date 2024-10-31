Certainly! Here is the consolidated markdown documentation in a single block:

```markdown
# Project Title

## Guide to Hacking Another Process Using System Calls `mmap` or `pmap`

This guide explains how to hack another process by using system calls `mmap` or `pmap`. Assuming there is a process `a.out` that has data in memory, the hack process can access its data and update it. Follow these steps:

### Step 1: Identify the Target Process

First, identify the process ID (PID) of the target process (`a.out`). You can use the `ps` command to find the PID:

```sh
ps aux | grep a.out
```

### Step 2: Locate the Memory Address

Use the `pmap` command to find the memory address of the target process:

```sh
pmap <pid>
```

Replace `<pid>` with the actual PID of the target process. Note the memory address you want to access.

### Step 3: Open the Memory File

Open the memory file of the target process. This requires root privileges:

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int fd = open("/proc/<pid>/mem", O_RDWR);
if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
}
```

Replace `<pid>` with the actual PID of the target process.

### Step 4: Map the Memory

Map the memory of the target process to your process's address space:

```c
#include <sys/mman.h>

void *map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, address);
if (map == MAP_FAILED) {
    perror("mmap");
    close(fd);
    exit(EXIT_FAILURE);
}
```

Replace `size` and `address` with the appropriate values obtained from the `pmap` command.

### Step 5: Modify the Memory

Modify the memory as needed. For example, to change an integer value:

```c
*((int *)map) = 42;
```

### Step 6: Unmap the Memory and Close the File

Unmap the memory and close the file:

```c
if (munmap(map, size) == -1) {
    perror("munmap");
}
close(fd);
```

### Important Notes

- **Root Privileges**: This guide assumes you have root privileges.
- **Cautious Modification**: Be cautious when modifying another process's memory, as it can lead to unexpected behavior or crashes.

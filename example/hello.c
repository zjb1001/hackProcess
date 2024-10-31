#include <stdio.h>
#include <unistd.h>

int main() {
    int counter = 0;
    
    printf("Hello Process (PID: %d)\n", getpid());
    
    while(1) {
        printf("Counter value: %d\n", counter);
        sleep(2);
    }
    
    return 0;
}
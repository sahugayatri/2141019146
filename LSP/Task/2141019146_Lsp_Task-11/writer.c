#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024 // Shared memory size
#define SHM_KEY 1234  // Unique key

int main() {
    int shmid;
    char *shm_ptr;

    // Create shared memory segment
    shmid = shmget(SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory to address space
    shm_ptr = (char *)shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    // Write message
    const char *message = "Hello from Shared Memory Writer , gayatri sahu !";
    strncpy(shm_ptr, message, SHM_SIZE);

    printf("Writer: Message written to shared memory: %s\n", message);

    // Detach shared memory
    shmdt(shm_ptr);

    return 0;
}

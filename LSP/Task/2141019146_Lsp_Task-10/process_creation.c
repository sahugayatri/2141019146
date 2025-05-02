#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  // Create a child process

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) is executing 'ls -l'\n", getpid());

        char *args[] = {"ls", "-l", NULL};  // Command to execute
        execvp(args[0], args);  // Replace child with 'ls -l'

        // If execvp returns, it must have failed
        perror("execvp failed");
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);  // Wait for child to finish

        if (WIFEXITED(status)) {
            printf("Child process (PID: %d) terminated with status %d\n", pid, WEXITSTATUS(status));
        } else {
            printf("Child process (PID: %d) terminated abnormally\n", pid);
        }
    }

    return 0;
}

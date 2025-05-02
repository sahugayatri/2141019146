#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    struct dirent *entry;
    DIR *dir;

    // 1. List files and directories in current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current Working Directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    printf("Contents of current directory:\n");
    while ((entry = readdir(dir)) != NULL) {
        printf(" - %s\n", entry->d_name);
    }
    closedir(dir);

    // 2. Change working directory to "/tmp"
    if (chdir("/tmp") != 0) {
        perror("chdir to /tmp failed");
        return 1;
    }

    // Confirm new current directory
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("\nChanged Working Directory: %s\n", cwd);
    }

    // 3. Create a new directory "testdir" with 755 permissions
    if (mkdir("testdir", 0755) == 0) {
        printf("Directory 'testdir' created in /tmp with 755 permissions.\n");
    } else {
        perror("mkdir");
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    char buffer[100];
    const char *filename = "data.txt";

    // Open the file for read and write. Create if it doesn't exist.
    file = fopen(filename, "w+");  // "w+" = write + read (file is created if not found)
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Write to the file
    const char *text = "Hello, File Handling in C!";
    fprintf(file, "%s", text);

    // Move file pointer to the beginning
    rewind(file);  // or fseek(file, 0, SEEK_SET);

    // Read contents back
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("File Content: %s\n", buffer);
    } else {
        printf("Failed to read from file.\n");
    }

    // Close the file
    fclose(file);

    return 0;
}

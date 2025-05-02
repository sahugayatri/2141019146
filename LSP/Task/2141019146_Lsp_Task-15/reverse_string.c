#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>

#define __NR_reverse_string 451  // Define the syscall number (should match your syscall table number)

int main() {
    // Step 1: Declare variables for input and output strings
    char *input = NULL;
    char *output = NULL;
    size_t input_len = 0;
    ssize_t read_len;

    // Step 2: Prompt user for input
    printf("Enter a string to reverse: ");
    read_len = getline(&input, &input_len, stdin);
    
    // Step 3: Handle input errors (empty or invalid input)
    if (read_len == -1) {
        perror("Error reading input");
        free(input);  // Free allocated memory
        return 1;
    }

    // Remove the newline character that getline captures
    if (input[read_len - 1] == '\n') {
        input[read_len - 1] = '\0';
    }

    // Step 4: Allocate memory for the output string
    output = (char *)malloc(read_len);  // Allocate space for the reversed string (same size as input)
    if (!output) {
        perror("Memory allocation failed for output string");
        free(input);  // Free the input memory
        return 1;
    }

    // Step 5: Call the custom syscall to reverse the string
    long ret = syscall(__NR_reverse_string, input, output);
    if (ret == 0) {
        // Step 6: Print the reversed string
        printf("Reversed string: %s\n", output);
    } else {
        // Handle syscall error
        perror("Syscall failed");
    }

    // Step 7: Free allocated memory
    free(input);   // Free memory used for input string
    free(output);  // Free memory used for output string

    return 0;
}


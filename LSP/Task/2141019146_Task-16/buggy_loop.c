#include <stdio.h>

int main() {
    int i;

    printf("Looping from 1 to 5...\n");
    
    // Bug: condition should be i <= 5, not i >= 5
    for (i = 1; i >= 5; i++) {
        printf("i = %d\n", i);
    }

    printf("Done looping.\n");

    return 0;
}


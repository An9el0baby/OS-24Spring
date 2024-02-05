#include <stdio.h>
#include <stdlib.h> 

int main(int argc, char *argv[]) {
    int year = atoi(argv[1]);

    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        printf("%d was a leap year\n", year);
    } else {
        printf("%d was not a leap year\n", year);
    }

    return 0; // Successful execution
}

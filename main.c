#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    char buffer[256];
    while (fgets(buffer, 255, stdin) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strcmp(buffer, "exit") == 0) {
            exit(0);
        }
        printf("your string: %s\n\n", buffer);
    }
    return 0;
}

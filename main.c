#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    printf("Entry to update: ");
    fgets(buffer, 255, stdin);
    sscanf(buffer, "%d", &line);
    return 0;
}

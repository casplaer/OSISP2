#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <string.h>

#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[]) {
    FILE *output = stdout;
    if (argc > 1) {
        output = fopen(argv[1], "w");
        if (output == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = 0;
        reverse_string(line);
        fprintf(output, "%s\n", line);
    }

    if (output != stdout) {
        fclose(output);
    }

    return 0;
}
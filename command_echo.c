#include <stdio.h>
#include <string.h>
#include "command_echo.h"

// Function to handle the 'echo' command
void handle_echo(char **args, char *command) {
    if (command[strlen("echo") + 1] == '"' && command[strlen(command) - 1] == '"') {
        for (int i = strlen("echo") + 2; i < strlen(command) - 1; i++)
            printf("%c", command[i]);
        printf("\n");
        return;
    }

    int i = 1;
    while (args[i] != NULL) {
        if (i > 1)
            printf(" ");
        printf("%s", args[i]);
        i++;
    }
    printf("\n");
}

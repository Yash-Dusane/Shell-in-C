#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>
#include <errno.h>

#include "command_cd.h"
#include "command_echo.h"
#include "command_pwd.h"
#include "command_history.h"
#include "utilities.h"

#define MAX_COMMAND_LENGTH 1024
#define DEFAULT_HISTORY_OUTPUT 10

int main() {
    char command[MAX_COMMAND_LENGTH];
    char *args[100];
    char username[128], system_name[128];
    char home[PATH_MAX];

    if (getcwd(home, sizeof(home)) == NULL) {
        fprintf(stderr, "Error: could not determine current directory\n");
        return 1;
    }

    get_username(username, sizeof(username));
    get_system_name(system_name, sizeof(system_name));
    load_history(); // Load command history

    while (1) {
        print_prompt(home, username, system_name);

        if (fgets(command, sizeof(command), stdin) == NULL) {
            printf("\n");
            break;
        }

        command[strcspn(command, "\n")] = '\0'; // Remove trailing newline
        add_to_history(command);                // Add command to history

        int i = 0;
        char *copy_comand = strdup(command);
        args[i] = strtok(command, " \t");
        while (args[i] != NULL)
        {
            i++;
            args[i] = strtok(NULL, " \t");
        }

        if (args[0] == NULL) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            save_history(); // Save history before exiting
            break;
        } else if (strcmp(args[0], "cd") == 0) {
            handle_cd(args, home);
        } else if (strcmp(args[0], "echo") == 0) {
            handle_echo(args, copy_comand);
        } else if (strcmp(args[0], "pwd") == 0) {
            handle_pwd();
        } else if (strcmp(args[0], "history") == 0) {
            int count = (args[1] != NULL) ? atoi(args[1]) : DEFAULT_HISTORY_OUTPUT;
            handle_history(count);
        } else {
            printf("Command not found: %s\n", args[0]);
        }
    }

    return 0;
}

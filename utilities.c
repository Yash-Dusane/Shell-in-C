#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include "utilities.h"

void get_username(char *username, size_t size) {
    struct passwd *pw = getpwuid(getuid());
    if (pw == NULL) {
        perror("getpwuid");
        strncpy(username, "unknown", size - 1);
    } else {
        strncpy(username, pw->pw_name, size - 1);
    }
    username[size - 1] = '\0';
}

void get_system_name(char *system_name, size_t size) {
    if (gethostname(system_name, size - 1) != 0) {
        perror("gethostname");
        strncpy(system_name, "unknown", size - 1);
    }
    system_name[size - 1] = '\0';
}

void print_prompt(const char *home, const char *username, const char *system_name) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return;
    }
    
    if (strncmp(cwd, home, strlen(home)) == 0) {
        printf("<%s@%s:~%s> ", username, system_name, cwd + strlen(home));
    } else {
        printf("<%s@%s:%s> ", username, system_name, cwd);
    }
    fflush(stdout);
}

void parse_command(char *command, char **args) {
    int i = 0;
    args[i] = strtok(command, " \t");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " \t");
    }
}

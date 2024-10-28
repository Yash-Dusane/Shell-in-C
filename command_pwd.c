#include <stdio.h>
#include <unistd.h>
#include<limits.h>
#include <pwd.h>

#include "command_pwd.h"

// Function to handle the 'pwd' command
void handle_pwd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}

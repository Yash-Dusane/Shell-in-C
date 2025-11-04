#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include "command_cd.h"

char previous_dir[PATH_MAX] = "";

// Function to handle the 'cd' command
void handle_cd(char **args, const char *home)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        return;
    }

    if (args[1] == NULL || strcmp(args[1], "~") == 0)
    {
        if (chdir(home) == 0)
        {
            strcpy(previous_dir, cwd);
        }
        else
        {
            perror("cd");
        }
    }
    else if (strcmp(args[1], "..") == 0)
    {
        if (chdir("..") == 0)
        {
            strcpy(previous_dir, cwd);
        }
        else
        {
            perror("cd");
        }
    }
    else if (strcmp(args[1], "-") == 0)
    {
        if (previous_dir[0] == '\0')
        {
            fprintf(stderr, "cd: no previous directory\n");
            return;
        }
        if (chdir(previous_dir) == 0)
        {
            strcpy(previous_dir, cwd);
            printf("%s\n", previous_dir);
        }
        else
        {
            perror("cd");
        }
    }
    else
    {
        if (chdir(args[1]) == 0)
        {
            strcpy(previous_dir, cwd);
        }
        else
        {
            perror("cd");
        }
    }
}

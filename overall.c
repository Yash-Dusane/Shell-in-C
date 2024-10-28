#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>
#include <errno.h>

#define HISTORY_FILE ".shell_history"
#define MAX_HISTORY 20
#define DEFAULT_HISTORY_OUTPUT 10

char previous_dir[PATH_MAX] = "";
char history[MAX_HISTORY][1024];
int history_count = 0;

// Function to get the username
void get_username(char *username, size_t size)
{
    struct passwd *pw = getpwuid(getuid());
    if (pw == NULL)
    {
        perror("getpwuid");
        strncpy(username, "unknown", size - 1);
    }
    else
    {
        strncpy(username, pw->pw_name, size - 1);
    }
    username[size - 1] = '\0';
}

// Function to get the system name
void get_system_name(char *system_name, size_t size)
{
    if (gethostname(system_name, size - 1) != 0)
    {
        perror("gethostname");
        strncpy(system_name, "unknown", size - 1);
    }
    system_name[size - 1] = '\0';
}

void print_prompt(const char *home, const char *username, const char *system_name)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        return;
    }

    if (strncmp(cwd, home, strlen(home)) == 0)
    {
        printf("<%s@%s:~%s> ", username, system_name, cwd + strlen(home));
    }
    else
    {
        printf("<%s@%s:%s> ", username, system_name, cwd);
    }
    fflush(stdout);
}

// Function to load command history from a file
void load_history()
{
    FILE *file = fopen(HISTORY_FILE, "r");
    if (file != NULL)
    {
        while (fgets(history[history_count], sizeof(history[0]), file) && history_count < MAX_HISTORY)
        {
            history[history_count][strcspn(history[history_count], "\n")] = '\0'; // Remove newline
            history_count++;
        }
        fclose(file);
    }
}

// Function to save command history to a file
void save_history()
{
    FILE *file = fopen(HISTORY_FILE, "w");
    if (file != NULL)
    {
        for (int i = 0; i < history_count; i++)
        {
            fprintf(file, "%s\n", history[i]);
        }
        fclose(file);
    }
}

// Function to add a command to history
void add_to_history(const char *command)
{
    if (history_count > 0 && strcmp(history[history_count - 1], command) == 0)
    {
        return; // Do not add duplicate
    }

    if (history_count < MAX_HISTORY)
    {
        strncpy(history[history_count], command, sizeof(history[history_count]));
        history_count++;
    }
    else
    {
        // Shift history left to make room for the new command
        for (int i = 1; i < MAX_HISTORY; i++)
        {
            strncpy(history[i - 1], history[i], sizeof(history[i - 1]));
        }
        strncpy(history[MAX_HISTORY - 1], command, sizeof(history[MAX_HISTORY - 1]));
    }
}

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

// Function to handle the 'echo' command
void handle_echo(char **args, char *command)
{

    if (command[strlen("echo") + 1] == '"' & command[strlen(command) - 1] == '"')
    {
        for (int i = strlen("echo") + 2; i < strlen(command) - 1; i++)
            printf("%c", command[i]);
        printf("\n");
        return;
    }

    int i = 1;
    while (args[i] != NULL)
    {
        if (i > 1)
            printf(" ");
        printf("%s", args[i]);
        i++;
    }
    printf("\n");
}

// Function to handle the 'pwd' command
void handle_pwd()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("pwd");
    }
}

// Function to handle the 'history' command
void handle_history(int count)
{
    int start = (history_count > count) ? history_count - count : 0;
    for (int i = start; i < history_count; i++)
    {
        printf("%s\n", history[i]);
    }
}

// Main function
int main()
{
    char command[1024];
    char *args[100];
    char username[32], system_name[32];

    // const char *home = getenv("HOME");
    // if (home == NULL)
    // {
    //     fprintf(stderr, "Error: HOME environment variable not set\n");
    //     return 1;
    // }

    // Set home to the current working directory
    char home[PATH_MAX];
    if (getcwd(home, sizeof(home)) == NULL)
    {
        fprintf(stderr, "Error: could not determine current directory\n");
        return 1;
    }
    
    get_username(username, sizeof(username));
    get_system_name(system_name, sizeof(system_name));
    load_history(); // Load command history

    while (1)
    {
        print_prompt(home, username, system_name);

        if (fgets(command, sizeof(command), stdin) == NULL)
        {
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

        if (args[0] == NULL)
        {
            continue;
        }

        if (strcmp(args[0], "exit") == 0)
        {
            save_history(); // Save history before exiting
            break;
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            handle_cd(args, home);
        }
        else if (strcmp(args[0], "echo") == 0)
        {
            handle_echo(args, copy_comand);
        }
        else if (strcmp(args[0], "pwd") == 0)
        {
            handle_pwd();
        }
        else if (strcmp(args[0], "history") == 0)
        {
            int count = (args[1] != NULL) ? atoi(args[1]) : DEFAULT_HISTORY_OUTPUT;
            handle_history(count);
        }
        else
        {
            printf("Command not found: %s\n", args[0]);
        }
    }

    return 0;
}

#include "headers.h"

// Splits input string in individual commands and calls execute for them in background or foreground process
void split_and_exec(char *input)
{
    setpgid(0,0);
    char curr_command[strlen(input)];
    char temp[strlen(input)];
    int t_out=dup(1);
    int t_in=dup(0);
    for (int i = 0, j = 0; i < (int)(strlen(input)); i++, j++)
    {
        if (input[i] == '\n')
        {
            curr_command[j] = '\0';
            break;
        }
        if (input[i] == '&')
        {
            curr_command[j] = '\0';
            int pid = fork();
            char cpy[strlen(curr_command)];
            strcpy(cpy,curr_command);
            if (pid == 0)
            {
                setpgid(0,0);
                execc(curr_command, 1);
                exit(0);
            }
            else
            {
                dup2(t_out,STDOUT_FILENO);
                dup2(t_in,STDIN_FILENO);
                addbgproc(pid,strtok(cpy," \t"));
                fprintf(stdout,"%d\n", pid);
            }
            strcpy(curr_command, "");
            j = -1;
        }
        else if (input[i] == ';')
        {
            curr_command[j] = '\0';
            struct timeval start_time, end_time;
            gettimeofday(&start_time, NULL);
            strcpy(temp, curr_command);
            execc(curr_command, 0);
            dup2(t_out,STDOUT_FILENO);
            dup2(t_in,STDIN_FILENO);
            gettimeofday(&end_time, NULL);
            double start_seconds = start_time.tv_sec + start_time.tv_usec / 1000000.0;
            double end_seconds = end_time.tv_sec + end_time.tv_usec / 1000000.0;
            int runtime = (int)(end_seconds - start_seconds);
            if (runtime > 2.0)
            {
                fprintf(stdout,"%s took %d seconds\n", strtok(temp, " \t"), runtime);
            }
            strcpy(curr_command, "");
            j = -1;
        }
        else
        {
            curr_command[j] = input[i];
        }
    }
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    strcpy(temp, curr_command);
    execc(curr_command, 0);
    dup2(t_out,STDOUT_FILENO);
    gettimeofday(&end_time, NULL);
    double start_seconds = start_time.tv_sec + start_time.tv_usec / 1000000.0;
    double end_seconds = end_time.tv_sec + end_time.tv_usec / 1000000.0;
    int runtime = (int)(end_seconds - start_seconds);
    if (runtime > 2.0)
    {
        fprintf(stdout,"%s took %d seconds\n", strtok(temp, " \t"), runtime);
    }
    close(t_out);
}

int main()
{
    // Retrives the previous commands stored for pastevents
    init_prev_commands();
    init();
    signal(SIGINT,ctrlchandle);
    signal(SIGTSTP,ctrlzhandle);
    // Keep accepting commands
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt();
        char input[4096];
        strlen(fgets(input, 4096, stdin));
        if(feof(stdin))
        {
            kill(0,SIGTERM);
        }
        if (strlen(input) > 1)
        {
            split_and_exec(input);
        }
    }
}

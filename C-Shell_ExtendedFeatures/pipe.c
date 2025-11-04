#include "headers.h"
int pexe(char *s, int mode);
int split(char *c, int first) {
    char cpy[4096];
    strcpy(cpy, c);

    int ind = 1;
    char *arg = strtok(c, " \t\n");
    while (arg != NULL) {
        ind++;
        arg = strtok(NULL, " \t");
    }

    char **argv = (char **)malloc(sizeof(char *) * (ind + 1));
    arg = strtok(cpy, " \t\n");
    int i = 0;
    int input_redir = 0;
    int write_redir = 0;
    int append_redir=0;
    char *input_file = NULL;
    char *output_file = NULL;

    while (arg != NULL) {
        if (strcmp(arg, ">") == 0) {
            write_redir=1;
            output_file = strtok(NULL, " \t\n");
            break;
        }
        else if(strcmp(arg, ">>") == 0){
            append_redir=1;
            output_file=strtok(NULL, " \t\n");
            break;
        } 
        else if (strcmp(arg, "<") == 0) {
            input_redir = 1;
            input_file = argv[i + 1];
            break;
        }
        else
        {
            argv[i] = (char *)malloc(sizeof(char) * strlen(arg) + 1);
            strcpy(argv[i], arg);
            arg = strtok(NULL, " \t\n");   
        }
        i++;
    }
    argv[i] = NULL;

    // Redirection handling
    if (input_redir) {
        int fd_in = open(input_file, O_RDONLY);
        if (fd_in == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    if (write_redir || append_redir) {
        int fd_out;
        if (write_redir) {
            fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        } else { 
            fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        if (fd_out == -1) {
            perror("open");
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }

    int d = execvp(argv[0], argv);
    if (d == -1) {
        for (int i = 0; i < ind; i++) {
            free(argv[i]);
        }
        free(argv);
        perror("execvp");
    }

    for (int i = 0; i < ind; i++) {
        free(argv[i]);
    }
    free(argv);
    return 0;
}

int pipeexec(char *s, int mode)
{
    int num_commands = 1;
    if (s[0] == '|')
    {
        printf(RED "No command on left of pipe\n" RESET);
        return -1;
    }
    if (s[strlen(s) - 1] == '|')
    {
        printf(RED "No command to right of pipe\n" RESET);
        return -1;
    }
    for (int i = 0; i < strlen(s) - 1; i++)
    {
        if (s[i] == '|')
        {
            num_commands++;
        }
    }
    char commands[num_commands][4096];
    int i, j, ind;
    for (i = 0, j = 0, ind = 0; i < strlen(s); i++, j++)
    {
        if (s[i] == '|')
        {
            commands[ind][j] = '\0';
            ind++;
            j = -1;
            continue;
        }
        commands[ind][j] = s[i];
    }
    commands[num_commands - 1][j] = '\0';
    int pipes[num_commands - 1][2];

    for (int i = 0; i < num_commands - 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            return -1;
        }
    }

    pid_t child_pids[num_commands];

    for (i = 0; i < num_commands;i++)
    {
        child_pids[i] = fork();

        if (child_pids[i] == -1)
        {
            perror("fork");
            return -1;
        }

        if (child_pids[i] == 0)
        {
        
            // Child process
            if (i > 0)

            {
                // Redirect stdin to the read end of the previous pipe
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
            }

            if (i < num_commands - 1)
            {
                // Redirect stdout to the write end of the current pipe
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][1]);
            }
            if(strstr(commands[i],"peek")!=NULL || strstr(commands[i],"warp")!=NULL || strstr(commands[i],"proclore")!=NULL || strstr(commands[i],"seek")!=NULL || strstr(commands[i],"fg")!=NULL || strstr(commands[i],"pastevents")!=NULL || 
              strstr(commands[i],"bg")!=NULL || strstr(commands[i],"activities")!=NULL || strstr(commands[i],"neonate")!=NULL || strstr(commands[i],"iman")!=NULL)
              {
                 pexe(commands[i],mode);
              }
              else
              {
                split(commands[i],(i==0));
              }
                

            exit(1);
        }
        else
        {
           wait(NULL);
            if(i>0){
                close(pipes[i-1][0]);
            }
            if(i<num_commands-1){
                close(pipes[i][1]);
            }
        }
    }

    // Close all pipe ends in the parent process
    // for (int i = 0; i < num_commands - 1; i++)
    // {
    //     close(pipes[i][0]);
    //     close(pipes[i][1]);
    // }

    return 0;
}
int pexe(char *s, int mode)
{
    char temp[strlen(s)];
    strcpy(temp, s);
    if (strstr(s, ">>") != NULL)
    {
        char cpy[strlen(s)];
        strcpy(cpy, s);
        strtok(strstr(cpy, ">"), " \t");
        char *fname = strtok(NULL, " \t");
        int fd = open(fname, O_APPEND | O_CREAT, 0644);
        strtok(s, ">");
        dup2(fd, 1);
    }
    else if (strstr(s, ">") != NULL)
    {
        char cpy[strlen(s)];
        strcpy(cpy, s);
        strtok(strstr(cpy, ">"), " \t");
        char *fname = strtok(NULL, " \t");
        int fd = open(fname, O_WRONLY | O_CREAT, 0644);
        strtok(s, ">");
        dup2(fd, 1);
    }
    else if (strstr(s, "<") != NULL)
    {
        strtok(s, "<");
        char *fname = strtok(NULL, " \t");
        int fd = open(fname, O_APPEND, S_IRUSR);
        if (fd == -1)
        {
            printf(RED "The input file doesn't exist! \n" RESET);
            return -1;
        }
    }
    char *comm = strtok(s, " \t");
    if (comm == NULL)
    {
        return 0;
    }
    if (strcmp(comm, "warp") == 0)
    {
        char *arg = strtok(NULL, " \t");
        if (arg == NULL)
        {
            warp(NULL);
        }
        while (arg != NULL)
        {

            warp(arg);
            arg = strtok(NULL, " \t");
        }
    }
    else if (strcmp(comm, "pastevents") == 0)
    {
        char *arg = strtok(NULL, " \t");
        if (arg == NULL)
        {
            pastevents();
            return 0;
        }
        else if (strcmp(arg, "purge") == 0)
        {
            pastevents_purge();
            return 0;
        }
        else if (strcmp(arg, "execute") == 0)
        {
            arg = strtok(NULL, " \t");
            int ind = 0;
            if (strlen(arg) == 2)
            {
                ind = (arg[0] - '0') * 10;
                ind = arg[1] - '0';
            }
            else
            {
                ind = (arg[0] - '0');
            }
            if (ind <= curr_capacity && strcmp(temp, prev_commands[ind - 1]) != 0)
            {
                pastevents_execute(ind, mode);
            }
            else
            {
                fprintf(stdout, "Index out of range \n");
                return -1;
            }
        }
    }
    else if (strcmp(comm, "peek") == 0)
    {
        char *arg = strtok(NULL, " \t");
        if (arg == NULL)
        {
            fprintf(stdout, RED "Expected a path/directory\n" RESET);
        }
        else if (strcmp(arg, "-l") == 0)
        {
            arg = strtok(NULL, " \t");
            if (arg == NULL)
            {
                fprintf(stdout, RED "Expected a path/directory\n" RESET);
            }
            else if (strcmp(arg, "-a") == 0)
            {
                arg = strtok(NULL, " \t");
                if (arg == NULL)
                {
                    fprintf(stdout, RED "Expected a path/directory\n" RESET);
                }
                else
                {
                    peek(arg, 3);
                }
            }
            else
            {
                peek(arg, 1);
            }
        }
        else if (strcmp(arg, "-a") == 0)
        {
            arg = strtok(NULL, " \t");
            if (arg == NULL)
            {
                fprintf(stdout, RED "Expected a path/directory\n" RESET);
            }
            else if (strcmp(arg, "-l") == 0)
            {
                arg = strtok(NULL, " \t");
                if (arg == NULL)
                {
                    fprintf(stdout, RED "Expected a path/directory\n" RESET);
                }
                else
                {
                    peek(arg, 3);
                }
            }
            else
            {
                peek(arg, 2);
            }
        }
        else if (strcmp(arg, "-la") == 0 || strcmp(arg, "-al") == 0)
        {
            arg = strtok(NULL, " \t");
            if (arg == NULL)
            {
                fprintf(stdout, RED "Expected a path/directory\n" RESET);
            }
            else
            {
                peek(arg, 3);
            }
        }
        else
        {
            peek(arg, 0);
        }
    }
    else if (strcmp(comm, "proclore") == 0)
    {
        char *arg = strtok(NULL, " \t");
        int procid = 0;
        if (arg == NULL)
        {
            proclore(-1);
        }
        else
        {
            for (int i = 0; i < strlen(arg); i++)
            {
                procid = procid * 10 + (arg[i] - '0');
            }
            proclore(procid);
        }
    }
    else if (strcmp(comm, "seek") == 0)
    {
        char *arg = strtok(NULL, " \t");
        int d = 0, f = 0, e = 0;
        int entries=0;
        while (arg != NULL)
        {
            if (strcmp(arg, "-d") == 0)
            {
                d = 1;
            }
            else if (strcmp(arg, "-f") == 0)
            {
                f = 1;
            }
            else if (strcmp(arg, "-e") == 0)
            {
                e = 1;
            }
            else
            {
                char *arg2 = strtok(NULL, " \t");
                if (arg2 == NULL)
                {
                    entries=seek(start_dir, arg, d, f);
                }
                else
                {
                    entries=seek(arg2, arg, d, f);
                }
                if(entries==1 && e==1)
                {
                    struct stat fperm;
                    if(arg2==NULL)
                    {
                        stat(start_dir,&fperm);
                    }
                    else
                    {
                        stat(arg2,&fperm);
                    }
                    
                }
            }
            
            arg = strtok(NULL, " \t");
        }
    }
    else if (strcmp("iman", comm) == 0)
    {
        char *arg = strtok(NULL, " \t");
        iman(arg);
    }
    else if (strcmp("fg", comm) == 0)
    {
        char *arg = strtok(NULL, " \t");
        int procid = 0;
        if (arg == NULL)
        {
            fprintf(stdout, "pid required\n");
            return -1;
        }
        for (int i = 0; i < strlen(arg); i++)
        {
            procid = procid * 10 + (arg[i] - '0');
        }
        fg(procid);
    }
    else if (strcmp("bg", comm) == 0)
    {
        char *arg = strtok(NULL, " \t");
        int procid = 0;
        if (arg == NULL)
        {
            fprintf(stdout, "pid required\n");
            return -1;
        }
        for (int i = 0; i < strlen(arg); i++)
        {
            procid = procid * 10 + (arg[i] - '0');
        }
        bg(procid);
    }
    else if (strcmp("activities", comm) == 0)
    {
        activities();
    }
    else if (strcmp("ping", comm) == 0)
    {
        int procid = 0, sig = 0;
        char *arg = strtok(NULL, " \t");
        if (arg == NULL)
        {
            fprintf(stdout, "pid required\n");
            return -1;
        }
        for (int i = 0; i < strlen(arg); i++)
        {
            procid = procid * 10 + (arg[i] - '0');
        }
        arg = strtok(NULL, " \t");
        if (arg == NULL)
        {
            fprintf(stdout, "signal required\n");
            return -1;
        }
        for (int i = 0; i < strlen(arg); i++)
        {
            sig = sig * 10 + (arg[i] - '0');
        }
        ping(procid, sig);
    }
    else if(strcmp("neonate", comm) == 0)
    {
            char* arg= strtok(NULL, " \t");
            if(arg==NULL)
            {
                printf(RED"No argument\n"RESET);
                return -1;
            }
            if(strcmp("-n",arg)!=0)
            {
                printf(RED"Invalid flag\n"RESET);
                return -1;
            }
            arg= strtok(NULL, " \t");
            if(arg==NULL)
            {
                printf(RED"No time argument\n"RESET);
                return -1;
            }
            neonate(arg);
    }
        return 0;
}
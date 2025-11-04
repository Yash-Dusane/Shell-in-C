#include "headers.h"
// Parses commands and arguments and calls functions accordingly
int execc(char *s, int mode)
{
    char temp[strlen(s)];
    strcpy(temp, s);
    if (strstr(s, "|") != NULL)
    {
        pipeexec(s,mode);
    }
    else
    {
        int of=0;
        if(strstr(s,">>")!=NULL)
        {
            char cpy[strlen(s)];
            strcpy(cpy,s);
            strtok(strstr(cpy,">")," \t");
            char* fname=strtok(NULL," \t");
            int fd=open(fname,O_WRONLY | O_APPEND | O_CREAT,0644);
            strtok(s,">");
            dup2(fd,1);
            of=1;
        }
        else if(strstr(s,">")!=NULL)
        {
            char cpy[strlen(s)];
            strcpy(cpy,s);
            strtok(strstr(cpy,">")," \t");
            char* fname=strtok(NULL," \t");
            int fd=open(fname,O_WRONLY | O_CREAT | O_TRUNC,0644);
            
            strtok(s,">");
            dup2(fd,1);
            of=1;
        }
        else if(strstr(s,"<")!=NULL)
        {
            strtok(s,"<");
            char* fname=strtok(NULL," \t");
            int fd=open(fname,O_RDONLY,S_IRUSR);
            if(fd==-1)
            {
                printf(RED"The input file doesn't exist! \n" RESET);
                return -1;
            }  
            dup2(fd,0); 
            of=1;
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
                        seek(start_dir, arg, d, f);
                    }
                    else
                    {
                        seek(arg2, arg, d, f);
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
        else
        {
            // Assuming 10 arguments at max
            char cpy[4096];
            strcpy(cpy, temp);
            if(of==1)
            {
                strtok(cpy,"<>");
            }
            int ind = 1;
            char *arg = strtok(NULL, " \t");
            while (arg != NULL)
            {
                ind++;
                arg = strtok(NULL, " \t");
            }
            char **argv = (char **)malloc(sizeof(char *) * (ind + 1));
            arg = strtok(cpy, " \t");
            int i = 0;
            while (arg != NULL)
            {
                argv[i] = (char *)malloc(sizeof(char) * strlen(arg));
                strcpy(argv[i], arg);
                arg = strtok(NULL, " \t");
                i++;
            }
            argv[i] = NULL;
            int pid;
            if(mode!=1)
            {
                pid = fork();
            }
            else
            {
                pid=0;
            }
            if (pid == 0)
            {
                if (execvp(comm, argv) == -1)
                {
                    for (int i = 0; i < ind; i++)
                    {
                        free(argv[i]);
                    }
                    free(argv);
                    perror("execvp");
                    exit(-1);
                }

                for (int i = 0; i < ind; i++)
                {
                    free(argv[i]);
                }
                free(argv);
                if(mode==0)
                  exit(0);
            }
            else
            {
                int s;
                waitpid(pid, &s, 0);
                if (WEXITSTATUS(s) != 0 && mode == 1)
                {
                    fprintf(stdout, "\n%s exited with an error(%d)\n", comm, pid);
                    return -1;
                }
                else if (mode == 1)
                {
                    fprintf(stdout, "\n%s exited normally(%d)\n", comm, pid);
                }
            }
        }
    }
    if (curr_capacity == 0 || strcmp(temp, prev_commands[0]) != 0)
    {
        write_command(temp);
    }
    return 0;
}
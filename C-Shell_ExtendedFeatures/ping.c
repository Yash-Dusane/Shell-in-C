#include "headers.h"

void ping(int pid, int signal)
{
    short true_signal = signal % 32;
    if (kill(pid, true_signal) < 0)
    {
        perror("kill");
    }
}
void ctrlchandle()
{
    pid_t pg = (STDIN_FILENO);
    if (pg > 0)
    {
        if (kill(-pg, SIGINT) == -1)
        {
            perror("kill");
        }
    }
    fprintf(stdout,"\n");
}
void ctrlzhandle()
{
       pid_t pg= (STDIN_FILENO);
       if(pg>0)
       {
         if(kill(pg,SIGTSTP)==-1)
         {
            perror("kill");
         }
       }
       printf("\n");
}
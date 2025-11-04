#include "headers.h"
int fg(int pid)
{
    int s;
    if(kill(pid,SIGCONT)==-1)
    {
        return -1;
    }
    waitpid(pid,&s,0);
    return 0;

}
int bg(int pid)
{
    return kill(pid,SIGCONT);
}
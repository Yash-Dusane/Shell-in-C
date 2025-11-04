#include "headers.h"
void proclore(int pid)
{
    if(pid==-1)
    {
        pid=getpid();
    }
    char process_path[100];
    char exec_path[100];
    snprintf(process_path, sizeof(process_path), "/proc/%d/status", pid);
    snprintf(exec_path,sizeof(exec_path), "/proc/%d/exe",pid);
    FILE* pfile=fopen(process_path,"r");
    if(pfile==NULL)
    {
        fprintf(stdout,RED "Invalid process id\n" RESET);
        return;
    }
    fprintf(stdout,"pid: %d\n",pid);
    char line[256];
    while (fgets(line, sizeof(line), pfile)) {
        if (strncmp(line, "Groups:", 7) == 0) {
            fprintf(stdout,"Process Groups: %s", line + 7);
        } else if (strncmp(line, "State:", 6) == 0) {
            fprintf(stdout,"Process status: %c", line[7]);
            if(!isbgproc(pid))
            {
                fprintf(stdout,"+\n");
            }
            else
            {
                fprintf(stdout,"\n");
            }
        }
        else if(strncmp(line,"VmSize:",7)==0)
        {
            fprintf(stdout,"Virtual memory: %s",line+10);
        }
    }
    char ep[4096];
    long size;
    size=readlink(exec_path,ep,sizeof(ep)-1);
    if(size!=-1)
    {
        ep[size]='\0';
        get_relative_path(ep,ep);
        fprintf(stdout,"executable Path: %s\n",ep);
    }
    else
    {
        fprintf(stdout,"Can't retrieve executable path\n");
    }
    fclose(pfile);
}
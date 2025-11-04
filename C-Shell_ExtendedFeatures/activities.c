#include "headers.h"

BgList *bghead;
// Linked List functions
void init()
{
    bghead=(BgList *)malloc(sizeof(BgList));
    bghead->pid=-1;
    bghead->next = NULL;
    bghead->name = NULL;
}
void addbgproc(int pid, char *name)
{
    BgList *newproc = (BgList *)malloc(sizeof(BgList));
    newproc->pid = pid;
    newproc->name = (char *)malloc(sizeof(char) * strlen(name));
    strcpy(newproc->name, name);
    newproc->next = bghead->next;
    bghead->next = newproc;
}
void delproc(BgList *prev, BgList *curr)
{
    prev->next = curr->next;
    free(curr->name);
    free(curr);
}
int isbgproc(int pid)
{
    BgList* t=bghead->next;
    while(t!=NULL)
    {
        if(pid==t->pid)
        {
            return 1;
        }
        t=t->next;
    }
    return 0;
}
// Main activities function
void activities()
{

    char process_path[100];
    BgList *i = bghead->next;
    BgList* prev=bghead;
    while (i != NULL)
    {
        snprintf(process_path, sizeof(process_path), "/proc/%d/status", i->pid);
        FILE *pfile = fopen(process_path, "r");
        if (pfile == NULL)
        {
            delproc(prev,i);
            i=prev->next;
            continue;
        }
        char line[4096];
        while (fgets(line, sizeof(line), pfile))
        {
            if (strncmp(line, "State:", 6) == 0)
            {
                if(line[7]=='Z')
                {
                    break;
                }
                if(line[7]=='T')
                {
                    fprintf(stdout,"%d %s - Stopped\n", i->pid, i->name);
                }
                else
                {
                     fprintf(stdout,"%d %s - Running\n", i->pid, i->name);    
                }
            }
        }
        fclose(pfile);
        prev=i;
        i = i->next;
    }
    return;
}
#ifndef __ACTIVITIES_H_
#define __ACTIVITIES_H_
typedef struct BgList
{
    int pid;
    char* name;
    struct BgList* next;
} BgList;
void addbgproc(int pid,char* name);
void delproc(BgList *prev, BgList *curr);
int isbgproc(int pid);
void activities();
void init();
#endif
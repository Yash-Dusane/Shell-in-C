#include "headers.h"
int start_flag=0;
int old_set=0;
char old_dir[256];
char start_dir[256];



void prompt() {
    // Do not hardcode the prompt
    struct passwd *uinfo=getpwuid(getuid());
    // 256 char due to limits enforced by Linux
    char hostname[256];
    // Current working directory
    char cwd[4096];
    gethostname(hostname,sizeof(hostname));
    getcwd(cwd,sizeof(cwd));
    if(!start_flag)
    {
        strcpy(start_dir,cwd);
        strcpy(cwd,"~");
        start_flag=1;
    }
    else
    {
        get_relative_path(cwd,cwd);
    }
    fprintf(stdout,"<%s@%s:%s>",uinfo->pw_name,hostname,cwd);    
}

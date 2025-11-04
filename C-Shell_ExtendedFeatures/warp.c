#include "headers.h"

int warp(char *arg)
{
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));
    int change=0;
    if (arg==NULL || strcmp(arg, "~") == 0)
    {
        if(!chdir(start_dir))
        {
            change=1;
        }
    }
    else if (strcmp(arg, "..") == 0)
    {
        if(!chdir("../"))
        {
            change=1;
        }
    }
    else if (strcmp(arg, "-") == 0)
    {
        if (!old_set)
        {
            fprintf(stdout,"OLDPWD not set\n");
        }
        else
        {
            char *temp = (char *)malloc(sizeof(char) * strlen(cwd));
            strcpy(temp, cwd);
            if (!chdir(old_dir) && strcmp(old_dir,temp)!=0)
            {
                strcpy(old_dir, temp);
            }
            free(temp);
        }
    }
    else if (strcmp(arg, ".") == 0)
    {
        // Do nothing
    }
    else
    {
        char real_path[4096];
        if (arg[0] == '~' && arg[1] == '/')
        {
            get_real_path(arg,real_path);
        }
        else
        {
            strcpy(real_path,arg);
        }
        if(!chdir(real_path))
        {
           change=1;
        }
        else
        {
            fprintf(stdout,"%s Failed\n",real_path);
        }
    }
    if(change)
    {
        strcpy(old_dir,cwd);
        old_set=1;
    }
    getcwd(cwd,sizeof(cwd));
    fprintf(stdout,"%s\n",cwd);
    return 0;
}
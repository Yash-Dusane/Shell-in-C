#include "headers.h"
void peek(char *arg, int flag)
{
    struct dirent **entries;
    int num_entries = 0;
    char real_path[4096];
    if (arg[0] == '~')
    {
        get_real_path(arg, real_path);
    }
    else
    {
        strcpy(real_path, arg);
    }
    num_entries = scandir(real_path, &entries, NULL, alphasort);
    strcat(real_path,"/");
    char temp[4096];
    for (int i = 0; i < num_entries; i++)
    {
        strcpy(temp,real_path);
        if ((flag == 0 || flag == 1) && entries[i]->d_name[0] == '.')
        {
            continue;
        }
        switch (entries[i]->d_type)
        {
        case DT_REG:
            if (access(strcat(temp,entries[i]->d_name),X_OK)==0)
            {
                fprintf(stdout,GREEN "EXE %s ",entries[i]->d_name);
            }
            else
            {
                fprintf(stdout,RESET "FILE %s ",entries[i]->d_name);
            }
            break;
        case DT_DIR:
           fprintf(stdout,BLUE "DIR %s",entries[i]->d_name);break;
        }
        if(flag==1 || flag==3)
        {
            struct stat fperm;
            stat(temp,&fperm);
            fprintf(stdout,"%o %d %ld ",fperm.st_mode,fperm.st_uid,fperm.st_size);
            time_t last_accessed=fperm.st_atime;
            struct tm *tm_info=localtime(&last_accessed);
            char timebuff[100];
            strftime(timebuff,100,"%Y-%m-%d %H:%M:%S",tm_info);
            fprintf(stdout,"%s \n",timebuff);
        }
        else
        {
            fprintf(stdout,"\n");
        }
    }
    fprintf(stdout,RESET);
}
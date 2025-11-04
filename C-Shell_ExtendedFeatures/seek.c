#include "headers.h"
int seek(char *directory, char *target,int d,int f)
{
    if(d && f)
    {
        fprintf(stdout,"Invalid flags! \n");
        return -1;
    }
    DIR *dir = opendir(directory);
    if (dir == NULL)
    {
        perror("opendir");
        return -1;
    }

    struct dirent *entry=readdir(dir);
    int total_entries=0;
    while (entry != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            entry=readdir(dir);
            continue;   
        }

        char full_path[4096];
        snprintf(full_path, sizeof(full_path), "%s/%s", directory, entry->d_name);

        struct stat entry_stat;
        if (stat(full_path, &entry_stat) == -1)
        {
            perror("stat");
            continue;
        }

        if (S_ISDIR(entry_stat.st_mode))
        {
            // It's a directory, search recursively
            if (strstr(entry->d_name, target) != NULL && !f)
            {
                fprintf(stdout,BLUE "%s\n" RESET, full_path);
                total_entries++;
            }
            seek(full_path, target,d,f);
        }
        else if (S_ISREG(entry_stat.st_mode))
        {
            // It's a regular file, check if it matches the target
            if (strstr(entry->d_name, target) != NULL && !d)
            {
                fprintf(stdout,GREEN "%s\n" RESET, full_path);
                total_entries++;
            }
        }
        entry=readdir(dir);
    }

    closedir(dir);
    return total_entries;
}

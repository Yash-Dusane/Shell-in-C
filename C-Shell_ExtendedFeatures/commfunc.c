#include "headers.h"
// Get common prefix length
int get_common_prefix_len(char *start_dir, char *curr_dir)
{
    int i;
    int min_len = strlen(start_dir) > strlen(curr_dir) ? strlen(curr_dir) : strlen(start_dir);
    for (i = 0; i < min_len; i++)
    {
        if (start_dir[i] != curr_dir[i])
        {
            break;
        }
    }
    return i;
}
// Get absolute path from relative path
void get_real_path(char *rel_path, char *abs_path)
{
    char temp[strlen(rel_path) - 1 + strlen(start_dir)];

    for (int i = 0; i < strlen(start_dir); i++)
    {
        temp[i] = start_dir[i];
    }
    for (int i = strlen(start_dir), j = 1; j < strlen(rel_path); i++, j++)
    {
        temp[i] = rel_path[j];
    }
    strcpy(abs_path, temp);
}
// Get relative path from absolute path
void get_relative_path(char *abs_path, char *rel_path)
{
    int comm_len = get_common_prefix_len(start_dir,abs_path);
    if ((comm_len - (int)strlen(start_dir)) >= 0)
    {
        char *temp = (char *)malloc(sizeof(char) * ((int)strlen(abs_path) - comm_len + 2));
        for (int i = 0; i < (strlen(abs_path) - comm_len + 2); i++)
        {
            temp[i] = '\0';
        }
        temp[0] = '~';
        int i, j;
        for (i = comm_len, j = 1; i < (int)strlen(abs_path); i++, j++)
        {
            temp[j] = abs_path[i];
        }
        strcpy(rel_path,temp);
        free(temp);
    }
}
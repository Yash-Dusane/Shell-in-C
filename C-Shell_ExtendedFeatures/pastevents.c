#include "headers.h"

char prev_commands[15][4096];
int curr_capacity = 0;
char command_dir[512];
void write_command(char *c)
{
    strcpy(command_dir,start_dir);
    strcat(command_dir,"/prev_commands.txt");
    if (c != NULL)
    {
        FILE *w = fopen(command_dir, "w");
        for (int i = curr_capacity; i > 0; i--)
        {
            if (i != 15)
            {
                strcpy(prev_commands[i], prev_commands[i - 1]);
            }
        }
        if(curr_capacity<15)
        {
            curr_capacity++;
        }
        strcpy(prev_commands[0], c);
        for (int i = 0; i < curr_capacity; i++)
        {
            fwrite(prev_commands[i], sizeof(char), strlen(prev_commands[i]), w);
            fputc('\n', w);
        }
        fclose(w);
    }
    else
    {
        FILE *w = fopen(command_dir, "w");
        fputc('\n',w);
        fclose(w);
    }
}
void init_prev_commands()
{
    FILE *c = fopen("./prev_commands.txt", "r");
    int j = 0;
    char t;
    t=fgetc(c);
    if(t=='\n')
    {
        return;
    }
    while (t != EOF)
    {
        if (t == '\n')
        {
            prev_commands[curr_capacity][j]='\0';
            j=0;
            t=fgetc(c);
            curr_capacity++;
            continue;
        }
        prev_commands[curr_capacity][j] = t;
        t=fgetc(c);
        j++;
    }
    fclose(c);
}
void pastevents_purge()
{
    for(int i=0;i<15;i++)
    {
        strcpy(prev_commands[i],"");
    }
    curr_capacity=0;
    write_command(NULL);
}
int pastevents_execute(int index,int mode)
{
    return execc(prev_commands[index-1],mode);
}
void pastevents()
{
    for(int i=0;i<curr_capacity;i++)
    {
        fprintf(stdout,"%s\n",prev_commands[i]);
    }
}


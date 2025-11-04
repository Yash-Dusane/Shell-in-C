#include "headers.h"

struct termios orig_termios;
void die(const char *s)
{
    perror(s);
    exit(1);
}
void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

void neonate(char *arg)
{
    int time_arg = atoi(arg);
    if (time_arg < 0)
    {
        printf(RED "Time is negative\n");
        return;
    }
    char c;
    setbuf(stdout, NULL);
    enableRawMode();
    int pid = fork();
    if (pid == 0)
    {
        while (1)
        {
            char fil[]="/proc/sys/kernel/ns_last_pid";
            FILE *fp = fopen(fil, "r");
            char* buf=(char*)malloc(sizeof(char)*100);
            fgets(buf, sizeof(buf), fp);
            printf("%s\n", buf);
            sleep(time_arg);
        }
    }
    else
    {
        char c;
        setbuf(stdout, NULL);
        int inb=read(STDIN_FILENO, &c, 1);
        while (inb==1)
        {
            if (c == 'x')
            {
                kill(pid, SIGINT);
                disableRawMode(); 
                return;
            }
            inb = read(STDIN_FILENO, &c, 1);
        }
    }
}
#include "headers.h"

#define MAX_BUFFER_SIZE 4096

void iman(char *key)
{
    struct hostent *host = gethostbyname("man.he.net");
    if (!host)
    {
        perror("gethostbyname");
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    server.sin_addr = *((struct in_addr *)host->h_addr);

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        perror("socket");
    }

    if (connect(sfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect");
    }

    char request[512];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", key);
    if (send(sfd, request, strlen(request), 0) < 0)
    {
        perror("send");
    }

    char buffer[MAX_BUFFER_SIZE];
    int bytes_received;
    int i=0;
    char* begin;
    while ((bytes_received = recv(sfd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes_received] = '\0';
        if (strstr(buffer, "No matches for") != NULL)
        {
            fprintf(stdout,"ERROR: No such command\n");
            return;
        }
        if(i==0)
        {
            begin=strstr(buffer,"</STRONG>")+9;
            strcpy(buffer,begin);
        }
        for(int j=0;j<strlen(buffer);j++)
        {
            if(buffer[j]=='<')
            {
                while(buffer[j]!='>')
                {
                    j++;
                }
                continue;
            }
            putchar(buffer[j]);
        }
        i++;
    }
    close(sfd);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int num_pipes = 2;  // Change this to the desired number of pipes
    int pipes[num_pipes][2];

    // Create multiple pipes
    for (int i = 0; i < num_pipes; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }
    printf("Hello there");
    // Now you have an array of pipes, each represented by a pair of file descriptors.

    // Example: Write to the first pipe
    const char *message = "Hello, Pipe 0!";
    write(pipes[0][1], message, strlen(message));
    
    // Example: Read from the second pipe
    char buffer[256];
    ssize_t bytes_read = read(pipes[1][0], buffer, sizeof(buffer));
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Received from Pipe 1: %s\n", buffer);
    }

    // Close all pipes when done

    return 0;
}
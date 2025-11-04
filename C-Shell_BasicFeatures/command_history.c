#include <stdio.h>
#include <string.h>
#include "command_history.h"

#define HISTORY_FILE ".shell_history"
#define MAX_HISTORY 20

char history[MAX_HISTORY][1024];
int history_count = 0;

// Function to load command history from a file
void load_history() {
    FILE *file = fopen(HISTORY_FILE, "r");
    if (file != NULL) {
        while (fgets(history[history_count], sizeof(history[0]), file) && history_count < MAX_HISTORY) {
            history[history_count][strcspn(history[history_count], "\n")] = '\0'; // Remove newline
            history_count++;
        }
        fclose(file);
    }
}

// Function to save command history to a file
void save_history() {
    FILE *file = fopen(HISTORY_FILE, "w");
    if (file != NULL) {
        for (int i = 0; i < history_count; i++) {
            fprintf(file, "%s\n", history[i]);
        }
        fclose(file);
    }
}

// Function to add a command to history
void add_to_history(const char *command) {
    if (history_count > 0 && strcmp(history[history_count - 1], command) == 0) {
        return; // Do not add duplicate
    }
    if (history_count < MAX_HISTORY) {
        strncpy(history[history_count], command, sizeof(history[history_count]));
        history_count++;
    } else {
        // Shift history left to make room for the new command
        for (int i = 1; i < MAX_HISTORY; i++) {
            strncpy(history[i - 1], history[i], sizeof(history[i - 1]));
        }
        strncpy(history[MAX_HISTORY - 1], command, sizeof(history[MAX_HISTORY - 1]));
    }
}

// Function to handle the 'history' command
void handle_history(int count) {
    int start = (history_count > count) ? history_count - count : 0;
    for (int i = start; i < history_count; i++) {
        printf("%s\n", history[i]);
    }
}

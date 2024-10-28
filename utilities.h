#ifndef UTILITIES_H
#define UTILITIES_H

#include <stddef.h>

void get_username(char *username, size_t size);
void get_system_name(char *system_name, size_t size);
void print_prompt(const char *home, const char *username, const char *system_name);
void parse_command(char *command, char **args);

#endif // UTILITIES_H

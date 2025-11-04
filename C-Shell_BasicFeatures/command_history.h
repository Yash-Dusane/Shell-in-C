#ifndef COMMAND_HISTORY_H
#define COMMAND_HISTORY_H

void load_history();
void save_history();
void add_to_history(const char *command);
void handle_history(int count);

#endif // COMMAND_HISTORY_H

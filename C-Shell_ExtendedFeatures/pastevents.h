#ifndef __PASTEVENTS_H_
#define __PASTEVENTS_H_

// Previous commands and their capacity
extern char prev_commands[15][4096];
extern int curr_capacity;

void init_prev_commands();
void pastevents_purge();
int pastevents_execute(int index,int mode);
void pastevents();
void write_command(char *c);
#endif
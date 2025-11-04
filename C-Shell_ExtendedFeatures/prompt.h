#ifndef __PROMPT_H
#define __PROMPT_H

// Start directory and the flag if it has been set
extern char start_dir[256];
extern int start_flag;
// Old directory for - flag of warp and whether it is set
extern char old_dir[256];
extern int old_set;
void prompt();

#endif
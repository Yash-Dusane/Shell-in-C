#ifndef __COMMFUNC_H_
#define __COMMFUNC_H_
int get_common_prefix_len(char* start_dir,char* curr_dir);
void get_real_path(char* rel_path,char* abs_path);
void get_relative_path(char* abs_path,char* rel_path);

#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define BLUE    "\x1B[34m"
#endif
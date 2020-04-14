#ifndef MYOS_SHELL_H
#define MYOS_SHELL_H

#define SCREEN_HEIGHT   256
#define CMD_LIMIT       256

void init_shell();
void init_entry();
void exec_cmd();
void put_prompt();
void start_shell();

#endif

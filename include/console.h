#ifndef MYOS_CONSOR_H
#define MYOS_CONSOR_H

#define SCREEN_START    0xb8000
#define COLUMNS         80

void get_key(char *key, unsigned char code);
void move_cursor(unsigned int x, unsigned int y);
void put_char_pos(char c, unsigned char x, unsigned char y, unsigned short color);
void put_char(char c, unsigned short color);
void put_str(char *str, unsigned short color);
void init_console();
void show_status(char *status, char *msg);
void input_code(unsigned char code);
void input_key(char key);
void backspace();
void newline();
void exec_cmd();

#endif

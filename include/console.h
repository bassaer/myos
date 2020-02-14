#ifndef MYOS_CONSOR_H
#define MYOS_CONSOR_H

#include <color.h>

#define SCREEN_START    0xb8000
#define COLUMNS         80
#define ROWS            25
#define CONSOLE_OFFSET  1
#define CHAR_COLOR      GRAY

void init_console();
void get_key(char *key, unsigned char code);
unsigned int get_y();
void move_cursor(unsigned int x, unsigned int y);
void put_char_pos(char c, unsigned char x, unsigned char y, unsigned short color);
void write_vram(unsigned short c, unsigned char x, unsigned char y);
void put_char(char c, unsigned short color);
void put_str(char *str, unsigned short color);
void backspace();
void newline();
void clear_line();
void scroll();
void put_prompt();
char* sprintf(char *str, char *format, ...);
void printf(char *format, ...);
void debug(char *format, ...);

#endif

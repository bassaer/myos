#define SCREEN_START    0xb8000
#define COLUMNS         80

void move_cursor(unsigned int x, unsigned int y);
void put_char_pos(char c, unsigned char x, unsigned char y, unsigned short color);
void put_char(char c, unsigned short color);
void put_str(char *str, unsigned short color);
void init_screen();

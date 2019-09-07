#define SCREEN_START    0xb8000
#define COLUMNS         80
#define COLOR           0x0f

void move_cursor(unsigned int x, unsigned int y);
void put_char_pos(char c, unsigned char x, unsigned char y);
void put_char(char c);
void put_str(char *str);
void init_screen();

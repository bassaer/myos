#ifndef MYOS_CONSOR_H
#define MYOS_CONSOR_H

#include <color.h>
#include <type.h>
#include <uefi.h>

#define SCREEN_START    0xb8000
#define COLUMNS         80
#define ROWS            25
#define CONSOLE_OFFSET  1
#define CHAR_COLOR      GRAY

void init_console(EFI_SIMPLE_TEXT_INPUT_PROTOCOL *In, EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *Out);
void clear_screen();
void set_color(UINTN color);
void put_char(CHAR16 c, UINTN color);
void put_str(CHAR16 *str, UINTN color);
void clear_line();
void read_key(EFI_INPUT_KEY *key);
void printf(CHAR16 *format, ...);
void debug(CHAR16 *format, ...);

#endif

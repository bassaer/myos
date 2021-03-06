#include <console.h>
#include <color.h>
#include <event.h>
#include <stdarg.h>
#include <lib/string.h>

#define KEY_TABLE_SIZE    0x54

static EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
static EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
static CHAR16 *clear_string;

void init_console(EFI_SIMPLE_TEXT_INPUT_PROTOCOL *In, EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *Out) {
  ConIn = In;
  ConOut = Out;
  clear_screen();
  ConOut->EnableCursor(ConOut, true);
  UINTN Columns;
  UINTN Rows;
  ConOut->QueryMode(ConOut, ConOut->Mode->Mode, &Columns, &Rows);
  UINTN i;
  for(i = 0; i < Columns; i++) {
    clear_string[i] = L' ';
  }
}

void clear_screen() {
  ConOut->ClearScreen(ConOut);
}

void set_color(UINTN color) {
  ConOut->SetAttribute(ConOut, color);
}

void put_str(CHAR16 *str, UINTN color) {
  ConOut->SetAttribute(ConOut, color);
  ConOut->OutputString(ConOut, str);
}

void put_char(CHAR16 c, UINTN color) {
  CHAR16 str[2] = {c, L'\0'};
  put_str(str, color);
}

void clear_line() {
  UINTN row = ConOut->Mode->CursorRow;
  ConOut->SetCursorPosition(ConOut, 0, row);
  printf(clear_string);
  ConOut->SetCursorPosition(ConOut, 0, row);
}

void read_key(EFI_INPUT_KEY *key) {
  UINTN index;
  wait_event(1, ConIn->WaitForKey, &index);
  while(ConIn->ReadKeyStroke(ConIn, key));
}

void printf(CHAR16 *format, ...) {
  CHAR16 buf[256];
  va_list list;
  va_start(list, format);
  format_str(buf, format, &list);
  put_str(buf, EFI_LIGHTGRAY);
  va_end(list);
}

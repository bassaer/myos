#include <console.h>
#include <color.h>
#include <event.h>
#include <stdarg.h>
#include <lib/string.h>

#define KEY_TABLE_SIZE    0x54

static EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
static EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;

void init_console(EFI_SIMPLE_TEXT_INPUT_PROTOCOL *In, EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *Out) {
  ConIn = In;
  ConOut = Out;
  clear_screen();
  ConOut->EnableCursor(ConOut, true);
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

CHAR16 read_key() {
  EFI_INPUT_KEY key;
  UINTN index;
  wait_event(1, ConIn->WaitForKey, &index);
  while(ConIn->ReadKeyStroke(ConIn, &key));
  return key.UnicodeChar;
}

unsigned int read_str(CHAR16 *buf, unsigned int size) {
  unsigned int index = 0;
  while(index < size - 1) {
    buf[index] = read_key();
    put_char(buf[index], GRAY);
    if (buf[index] == L'\r') {
      buf[++index] = L'\n';
      break;
    }
    index++;
  }
  put_char(L'\n', GRAY);
  buf[index + 1] = L'\0';
  return index;
}

void printf(CHAR16 *format, ...) {
  CHAR16 buf[256];
  va_list list;
  va_start(list, format);
  format_str(buf, format, &list);
  put_str(buf, EFI_LIGHTGRAY);
  va_end(list);
}

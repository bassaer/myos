#ifndef MYOS_MAIN_H
#define MYOS_MAIN_H

struct EFI_INPUT_KEY {
  unsigned short ScanCode;
  unsigned short UnicodeChar;
};

struct EFI_SYSTEM_TABLE {
  char _buf1[44];
  struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    unsigned long long _buf;
    unsigned long long (*ReadKeyStroke)(struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This, struct EFI_INPUT_KEY *Key);
  } *ConIn;
  unsigned long long _buf2;
  struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    unsigned long long _buf;
    unsigned long long (*OutputString)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, unsigned short *String);
    unsigned long long _buf2[4];
    unsigned long long (*ClearScreen)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
  } *ConOut;
};

#endif

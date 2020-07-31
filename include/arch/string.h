#ifndef MYOS_STRING_H
#define MYOS_STRING_H

#include <stdarg.h>
#include <uefi.h>

int atoi(CHAR16 *src);

CHAR16* format_str(CHAR16 *str, CHAR16 *format, va_list *arg);

#endif

#ifndef MYOS_UTIL_H
#define MYOS_UTIL_H

#include <stdarg.h>
#include <type.h>

/**
 * ひとまず同じ場合は0を返す
 */
int strcmp(CHAR16 *first, CHAR16 *second);

/**
 * 文字列の長さを返す
 */
int strlen(CHAR16 *str);

/**
 * 指定文字で分割し、分割数を返す
 */
int split(CHAR16 *src, CHAR16 *dst[], CHAR16 delim);

/**
 * 文字列のコピー
 */
void strcpy(CHAR16 *src, CHAR16 *dist);

int atoi(CHAR16 *src);

CHAR16* format_str(CHAR16 *str, CHAR16 *format, va_list *arg);

CHAR16* sprintf(CHAR16 *str, CHAR16 *format, ...);

#endif

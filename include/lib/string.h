#ifndef MYOS_UTIL_H
#define MYOS_UTIL_H

#include <stdarg.h>

/**
 * ひとまず同じ場合は0を返す
 */
int strcmp(char *first, char *second);

/**
 * 文字列の長さを返す
 */
int strlen(char *str);

/**
 * 指定文字で分割し、分割数を返す
 */
int split(char *src, char *dst[], char delim);

/**
 * 文字列のコピー
 */
void strcpy(char *src, char *dist);

int atoi(char *src);

char* format_str(char *str, char *format, va_list *arg);

char* sprintf(char *str, char *format, ...);

#endif

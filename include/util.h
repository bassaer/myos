#ifndef MYOS_UTIL_H
#define MYOS_UTIL_H

/**
 * ひとまず同じ場合は0を返す
 */
int strcmp(char *first, char *second);

/**
 * 指定文字で分割し、分割数を返す
 */
int split(char *src, char *dst[], char delim);

#endif

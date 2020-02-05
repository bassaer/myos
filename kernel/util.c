#include <util.h>
#include <stdarg.h>

int strcmp(char *first, char *second) {
  for (; *first != '\0' && *second != '\0'; ++first, ++second) {
    if (*first != *second) {
      return 1;
    }
  }
  if ( *first == '\0' && *second == '\0') {
    return 0;
  }
  return 1;
}


int split(char *src, char *dst[], char delim) {
  int count = 0;

  while(1) {
    while (*src == delim) {
      src++;
    }

    if (*src == '\0') {
      break;
    }

    dst[count++] = src;

    while(*src && *src != delim) {
      src++;
    }

    if (*src == '\0') {
      break;
    }
    *src++ = '\0';
  }

  return count;
}

int itoa(int src, char *dst, int base) {
  int len = 0;
  int buf[10];

  while(1) {
    buf[len++] = src % base;
    if (src < base) {
      break;
    }
    src /= base;
  }

  // 桁
  int digit = len;

  while(len) {
    --len;
    *(dst++) = buf[len] < 10 ? buf[len] + 0x30 : buf[len] - 9 + 0x60;
  }

  return digit;
}

void sprintf(char *str, char *format, ...) {
  va_list list;
  int len;
  va_start(list, format);
  while(*format) {
    if (*format == '%') {
      ++format;
      switch(*format) {
        case 'd':
          len = itoa(va_arg(list, int), str, 10);
          break;
        case 'x':
          len = itoa(va_arg(list, int), str, 16);
          break;
      }
      str += len;
      format++;
    } else {
      *(str++) = *(format++);
    }
  }
  // NULL終端
  *str = 0x00;
  va_end(list);
}

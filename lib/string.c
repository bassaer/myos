#include <lib/string.h>

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

int strlen(char *str) {
  int len = 0;
  while(*str++ != '\0') {
    len++;
  }
  return len;
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

void strcpy(char *src, char *dst) {
  while ((*dst++ = *src++) != '\0');
  *(dst++) = '\0';
}

int atoi(char *src) {
  int num = 0;
  while(*src != '\0') {
    num += *src - 48;
    num *= 10;
    src++;
  }
  return num / 10;
}

int itoa(int src, char *dst, int base) {
  int len = 0;
  int buf[10];
  int negative = 0;
  if (src < 0) {
    src *= -1;
    negative = 1;
  }

  while(1) {
    buf[len++] = src % base;
    if (src < base) {
      break;
    }
    src /= base;
  }

  // 桁
  int digit = len + negative;

  while(len) {
    --len;
    *(dst++) = buf[len] < 10 ? buf[len] + 0x30 : buf[len] - 9 + 0x60;
  }

  return digit;
}

char* format_str(char *str, char *format, va_list *arg) {
  va_list list;
  va_copy(list, *arg);
  int d_arg;
  char *s_arg;
  while(*format) {
    if (*format == '%') {
      ++format;
      switch(*format) {
        case 'd':
          d_arg = va_arg(list, int);
          if (d_arg < 0) {
            d_arg *= -1;
            *(str++) = '-';
          }
          str += itoa(d_arg, str, 10);
          break;
        case 'x':
          *(str++) = '0';
          *(str++) = 'x';
          str += itoa(va_arg(list, int), str, 16);
          break;
        case 'c':
          *(str++) = va_arg(list, int);
          break;
        case 's':
          s_arg = va_arg(list, char*);
          while(*s_arg) {
            *(str++) = *(s_arg++);
          }
          break;
      }
      format++;
    } else {
      *(str++) = *(format++);
    }
  }
  // NULL終端
  *str = 0x00;
  va_end(list);
  return str;
}

char* sprintf(char *str, char *format, ...) {
  va_list list;
  va_start(list, format);
  format_str(str, format, &list);
  va_end(list);
  return str;
}


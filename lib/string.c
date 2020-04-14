#include <lib/string.h>

int strcmp(CHAR16 *first, CHAR16 *second) {
  for (; *first != L'\0' && *second != L'\0'; ++first, ++second) {
    if (*first != *second) {
      return 1;
    }
  }
  if ( *first == L'\0' && *second == L'\0') {
    return 0;
  }
  return 1;
}

int strlen(CHAR16 *str) {
  int len = 0;
  while(*str++ != L'\0') {
    len++;
  }
  return len;
}


int split(CHAR16 *src, CHAR16 *dst[], CHAR16 delim) {
  int count = 0;

  while(1) {
    while (*src == delim) {
      src++;
    }

    if (*src == L'\0') {
      break;
    }

    dst[count++] = src;

    while(*src && *src != delim) {
      src++;
    }

    if (*src == L'\0') {
      break;
    }
    *src++ = L'\0';
  }

  return count;
}

void strcpy(CHAR16 *src, CHAR16 *dst) {
  while ((*dst++ = *src++) != L'\0');
  *(dst++) = L'\0';
}

int atoi(CHAR16 *src) {
  int num = 0;
  while(*src != '\0') {
    num += *src - 48;
    num *= 10;
    src++;
  }
  return num / 10;
}

unsigned long itoa(long src, CHAR16 *dst, int base) {
  int len = 0;
  int buf[100];
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

CHAR16* format_str(CHAR16 *str, CHAR16 *format, va_list *arg) {
  va_list list;
  va_copy(list, *arg);
  long d_arg;
  CHAR16 *s_arg;
  while(*format) {
    if (*format == '%') {
      ++format;
      switch(*format) {
        case 'd':
          d_arg = va_arg(list, long);
          if (d_arg < 0) {
            d_arg *= -1;
            *(str++) = '-';
          }
          str += itoa(d_arg, str, 10);
          break;
        case 'x':
          *(str++) = '0';
          *(str++) = 'x';
          str += itoa(va_arg(list, unsigned long), str, 16);
          break;
        case 'c':
          *(str++) = va_arg(list, int);
          break;
        case 's':
          s_arg = va_arg(list, CHAR16*);
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

CHAR16* sprintf(CHAR16 *str, CHAR16 *format, ...) {
  va_list list;
  va_start(list, format);
  format_str(str, format, &list);
  va_end(list);
  return str;
}


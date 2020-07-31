#include <arch/string.h>

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

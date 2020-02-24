#include <string.h>

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

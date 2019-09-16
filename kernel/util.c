#include <util.h>


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

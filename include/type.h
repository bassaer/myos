#ifndef MYOS_TYPE_H
#define MYOS_TYPE_H

#define NULL ((void *)0)

#define	EFIAPI
#define OUT
#define IN
#define OPTIONAL
#define CONST const

typedef enum {
  false,
  true
} BOOLEAN;

typedef void VOID;

typedef unsigned long long INTN;
typedef unsigned long long UINTN;

typedef char INT8;
typedef unsigned char UINT8;

typedef short INT16;
typedef unsigned short UINT16;

typedef int INT32;
typedef unsigned int UINT32;

typedef long long INT64;
typedef unsigned long long UINT64;

typedef long long INT128;
typedef unsigned long long UINT128;

typedef unsigned char CHAR8;
typedef unsigned short CHAR16;

#endif

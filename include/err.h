#ifndef MYOS_ERR_H
#define MYOS_ERR_H

#include <type.h>

#define MAX_BIT                     0x8000000000000000ULL
#define ENCODE_ERROR(StatusCode)    ((EFI_STATUS)(MAX_BIT | (StatusCode)))

#define EFI_SUCCESS                 0
#define EFI_LOAD_ERROR              ENCODE_ERROR(1)
#define EFI_INVALID_PARAMETER       ENCODE_ERROR(2)
#define EFI_UNSUPPORTED             ENCODE_ERROR(3)
#define EFI_BAD_BUFFER_SIZE         ENCODE_ERROR(4)
#define EFI_BUFFER_TOO_SMALL        ENCODE_ERROR(5)
#define EFI_NOT_READY               ENCODE_ERROR(6)
#define EFI_DEVICE_ERROR            ENCODE_ERROR(7)
#define EFI_WRITE_PROTECTED         ENCODE_ERROR(8)
#define EFI_OUT_OF_RESOURCES        ENCODE_ERROR(9)
#define EFI_VOLUME_CORRUPTED        ENCODE_ERROR(10)

#endif

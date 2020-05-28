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
#define EFI_VOLUME_FULL             ENCODE_ERROR(11)
#define EFI_NO_MEDIA                ENCODE_ERROR(12)
#define EFI_MEDIA_CHANGED           ENCODE_ERROR(13)
#define EFI_NOT_FOUND               ENCODE_ERROR(14)
#define EFI_ACCESS_DENIED           ENCODE_ERROR(15)
#define EFI_NO_RESPONSE             ENCODE_ERROR(16)
#define EFI_NO_MAPPING              ENCODE_ERROR(17)
#define EFI_TIMEOUT                 ENCODE_ERROR(18)
#define EFI_NOT_STARTED             ENCODE_ERROR(19)
#define EFI_ALREADY_STARTED         ENCODE_ERROR(20)
#define EFI_ABORTED                 ENCODE_ERROR(21)
#define EFI_ICMP_ERROR              ENCODE_ERROR(22)
#define EFI_TFTP_ERROR              ENCODE_ERROR(23)
#define EFI_PROTOCOL_ERROR          ENCODE_ERROR(24)
#define EFI_INCOMPATIBLE_VERSION    ENCODE_ERROR(25)
#define EFI_SECURITY_VIOLATION      ENCODE_ERROR(26)
#define EFI_CRC_ERROR               ENCODE_ERROR(27)
#define EFI_END_OF_MEDIA            ENCODE_ERROR(28)
#define EFI_END_OF_FILE             ENCODE_ERROR(31)

#endif

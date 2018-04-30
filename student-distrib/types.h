#ifndef _TYPES_H
#define _TYPES_H

#define NULL 0

#ifndef ASM

/* Types defined here just like in <stdint.h> */

/* 4 bytes */
typedef int int32_t;
typedef unsigned int uint32_t;

/* 2 bytes */
typedef short int16_t;
typedef unsigned short uint16_t;

/* 1 byte */
typedef char int8_t;
typedef unsigned char uint8_t;

#endif /* ASM */

#endif /* _TYPES_H */

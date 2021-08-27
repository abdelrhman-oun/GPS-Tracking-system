/*--------------------------------------------------------------------------------------------------------------------------
 * file name  : Platform_Types.h
 * Author     : OUN
 * Created on : July 23, 2021
 * description: Platform types for ARM Cortex-M4F
 --------------------------------------------------------------------------------------------------------------------------*/

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

/*
 * Boolean Values
 */
#ifndef FALSE
#define FALSE       (0u)
#endif
#ifndef TRUE
#define TRUE        (1u)
#endif

#define NULL_PTR    ((void*)0)

typedef unsigned char         boolean;

typedef unsigned char         uint8;          /*           0 .. 255             */
typedef signed char           sint8;          /*        -128 .. +127            */
typedef unsigned short        uint16;         /*           0 .. 65535           */
typedef signed short          sint16;         /*      -32768 .. +32767          */
typedef unsigned long         uint32;         /*           0 .. 4294967295      */
typedef signed long           sint32;         /* -2147483648 .. +2147483647     */
typedef unsigned long long    uint64;         /*       0..18446744073709551615  */
typedef signed long long      sint64;
typedef float                 float32;
typedef double                float64;

#endif /* PLATFORM_TYPES_H */

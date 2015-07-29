#ifndef   TYPES_H
#define   TYPES_H
/*
typedef struct _uint128{
	uint64 high;
	uint64 low;
}_uint128;
typedef struct _sint128{
	sint64 high;
	sint64 low;
}_sint128;
*/
//uint128로 반환하게 될경우 uint64 이하의 값은 처리를 어떻게?
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;


typedef signed char         sint8;
typedef signed short        sint16;
typedef signed int          sint32;
typedef signed long long    sint64;


#endif     /* !TYPES_H */

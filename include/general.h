#ifndef GENERAL_H
#define GENERAL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// os specific includes
#ifdef WIN32
	#include <string.h>
	#include <stdlib.h>
#else
	#include <config.h>
#endif


#ifdef WIN32
	#define	uint64	unsigned __int64	   // these are cos of some msvc wierdness
	#define	int64	__int64
	#define	int32	int
	#define	uint32	unsigned int
	#define	int16	short
	#define	uint16	unsigned short
	#define	int8	char
	#define	uint8	unsigned char
#else
	typedef unsigned long long uint64;
	typedef long long int64;
	typedef unsigned char uint8;
	typedef char int8;
	typedef unsigned short int uint16;
	typedef short int int16;
	typedef unsigned int uint32;
	typedef int int32;
#endif

typedef void (*cpu_instruction)(void);
typedef void (*sighandler_t)(int);

// stuff for memory watching
#define WATCH_READ  1
#define WATCH_WRITE 2
#define WATCH_NUMBER 4

typedef
struct watch
{
  int Type;
  uint64 Address;
  int64 Value;
}watch;


typedef union fpureg
{
  int32 w;
  int64 l;
  float s;
  double d;
}fpureg;


// Exceptions, want to change these and put <<2 in Exception() sometime

#define INTERRUPT_EX            0
#define TLB_MOD_EX              (1<<2)
#define TLB_LOAD_EX             (2<<2)
#define TLB_STORE_EX            (3<<2)
#define SYSCALL_EX              (8<<2)
#define BREAK_EX                (9<<2)
#define COP_UNUSEABLE_EX   	(11<<2)
#define MATH_OVERFLOW_EX   	(12<<2)
#define TRAP_EX                 (13<<2)




// os specific defines

#ifdef WIN32
	#define strncasecmp strnicmp
	#define strcasecmp stricmp
	#define isnan _isnan


	#define NORMAL_LINKING
#endif
#endif


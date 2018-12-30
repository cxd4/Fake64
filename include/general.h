#include <stdio.h>
#include <config.h>

/*  put this in seperate .h */  

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef char int8;
typedef short int int16;
typedef int int32;
typedef long long int64;

typedef int (*cpu_instruction)(void);


typedef union fpureg
{
  int32 w;
  int64 l;
  float s;
  double d;
}fpureg;

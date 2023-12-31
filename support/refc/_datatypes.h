#pragma once

#include <gmp.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

#define NO_TAG 0
#define BITS32_TAG 3
#define BITS64_TAG 4
#define INT32_TAG 7
#define INT64_TAG 8
#define INTEGER_TAG 9
#define DOUBLE_TAG 10
#define STRING_TAG 12

#define CLOSURE_TAG 15
#define ARGLIST_TAG 16
#define CONSTRUCTOR_TAG 17

#define IOREF_TAG 20
#define ARRAY_TAG 21
#define POINTER_TAG 22
#define GC_POINTER_TAG 23
#define BUFFER_TAG 24

#define MUTEX_TAG 30
#define CONDITION_TAG 31

#define COMPLETE_CLOSURE_TAG 98 // for trampoline tail recursion handling

typedef struct {
  int refCounter;
  int tag;
} Value_header;

typedef struct {
  Value_header header;
  // `Value` is an "abstract" struct,
  // `Value_Xxx` structs have the same header
  // followed by type-specific payload.
} Value;

/* expcted at least 4bytes for Value_header alignment. */
#define idris2_vp_is_unboxed(p) ((uintptr_t)(p)&3)

#define idris2_vp_int_shift ((sizeof(Value *) == 4) ? 16 : 32)
#define idris2_vp_to_Bits64(p) (((Value_Bits64 *)(p))->ui64)
#define idris2_vp_to_Bits32(p)                                                 \
  ((sizeof(Value *) == 4)                                                      \
       ? (((Value_Bits32 *)(p))->ui32)                                         \
       : ((uint32_t)((uintptr_t)(p) >> idris2_vp_int_shift)))
#define idris2_vp_to_Bits16(p)                                                 \
  ((uint16_t)((uintptr_t)(p) >> idris2_vp_int_shift))
#define idris2_vp_to_Bits8(p) ((uint8_t)((uintptr_t)(p) >> idris2_vp_int_shift))
#define idris2_vp_to_Int64(p) (((Value_Int64 *)(p))->i64)
#define idris2_vp_to_Int32(p)                                                  \
  ((sizeof(Value *) == 4)                                                      \
       ? (((Value_Int32 *)(p))->i32)                                           \
       : ((int32_t)((uintptr_t)(p) >> idris2_vp_int_shift)))
#define idris2_vp_to_Int16(p) ((int16_t)((uintptr_t)(p) >> idris2_vp_int_shift))
#define idris2_vp_to_Int8(p) ((int8_t)((uintptr_t)(p) >> idris2_vp_int_shift))
#define idris2_vp_to_Char(p)                                                   \
  ((unsigned char)((uintptr_t)(p) >> idris2_vp_int_shift))
#define idris2_vp_to_Double(p) (((Value_Double *)(p))->d)
#define idris2_vp_to_Bool(p) (idris2_vp_to_Int8(p))

typedef struct {
  Value_header header;
  uint32_t ui32;
} Value_Bits32;

typedef struct {
  Value_header header;
  uint64_t ui64;
} Value_Bits64;

typedef struct {
  Value_header header;
  int32_t i32;
} Value_Int32;

typedef struct {
  Value_header header;
  int64_t i64;
} Value_Int64;

typedef struct {
  Value_header header;
  mpz_t i;
} Value_Integer;

typedef struct {
  Value_header header;
  double d;
} Value_Double;

typedef struct {
  Value_header header;
  char *str;
} Value_String;

typedef struct {
  Value_header header;
  int32_t total;
  int32_t tag;
  char *name;
  Value **args;
} Value_Constructor;

typedef struct {
  Value_header header;
  int32_t total;
  int32_t filled;
  Value **args;
} Value_Arglist;

typedef Value *(*fun_ptr_t)(Value_Arglist *);

typedef struct {
  Value_header header;
  fun_ptr_t f;
  Value_Arglist *arglist;
} Value_Closure;

typedef struct {
  Value_header header;
  int32_t index;
} Value_IORef;

typedef struct {
  Value_header header;
  void *p;
} Value_Pointer;

typedef struct {
  Value_header header;
  Value_Pointer *p;
  Value_Closure *onCollectFct;
} Value_GCPointer;

typedef struct {
  Value_header header;
  int capacity;
  Value **arr;
} Value_Array;

typedef struct {
  Value_header header;
  Buffer *buffer;
} Value_Buffer;

typedef struct {
  Value_header header;
  pthread_mutex_t *mutex;
} Value_Mutex;

typedef struct {
  Value_header header;
  pthread_cond_t *cond;
} Value_Condition;

typedef struct {
  Value **refs;
  int filled;
  int total;
} IORef_Storage;

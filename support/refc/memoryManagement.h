#pragma once

#include "cBackend.h"

Value *newValue(size_t size);
Value *newReference(Value *source);
void removeReference(Value *source);

#define IDRIS2_NEW_VALUE(t) ((t *)newValue(sizeof(t)))

Value_Constructor *newConstructor(int total, int tag);
Value_Closure *idris2_makeClosure(Value *(*f)(), uint8_t arity, uint8_t filled);

Value *idris2_mkDouble(double d);
#define idris2_mkChar(x)                                                       \
  ((Value *)(((uintptr_t)(x) << idris2_vp_int_shift) + 1))
#define idris2_mkBits8(x)                                                      \
  ((Value *)(((uintptr_t)(x) << idris2_vp_int_shift) + 1))
#define idris2_mkBits16(x)                                                     \
  ((Value *)(((uintptr_t)(x) << idris2_vp_int_shift) + 1))

#if !defined(UINTPTR_WIDTH)
#define idris2_mkBits32(x)                                                     \
  ((idris2_vp_int_shift == 16)                                                 \
       ? (idris2_mkBits32_Boxed(x))                                            \
       : ((Value *)(((uintptr_t)(x) << idris2_vp_int_shift) + 1)))
#define idris2_mkInt32(x)                                                      \
  ((idris2_vp_int_shift == 16)                                                 \
       ? (idris2_mkInt32_Boxed(x))                                             \
       : ((Value *)(((uintptr_t)(x) << idris2_vp_int_shift) + 1)))

#elif UINTPTR_WIDTH >= 64
#define idris2_mkBits32(x)                                                     \
  ((Value *)(((uintptr_t)(x) << idris2_vp_int_shift) + 1))
#define idris2_mkInt32(x)                                                      \
  ((Value *)(((uintptr_t)(x) << idris2_vp_int_shift) + 1))

#elif UINTPTR_WIDTH >= 32
#define idris2_mkBits32(x) (idris2_mkBits32_Boxed(x))
#define idris2_mkInt32(x) (idris2_mkInt32_Boxed(x)))

#else
#error "unsupported uintptr_t width"
#endif

#define idris2_mkInt8(x)                                                       \
  ((Value *)(((uintptr_t)(x) << idris2_vp_int_shift) + 1))
#define idris2_mkInt16(x)                                                      \
  ((Value *)(((uintptr_t)(x) << idris2_vp_int_shift) + 1))
#define idris2_mkBool(x) (idris2_mkInt8((x) ? 1 : 0))

Value *idris2_mkBits32_Boxed(uint32_t i);
Value *idris2_mkBits64(uint64_t i);
Value *idris2_mkInt32_Boxed(int32_t i);
Value *idris2_mkInt64(int64_t i);

Value_Integer *idris2_mkInteger();
Value *idris2_mkIntegerLiteral(char *i);
Value_String *idris2_mkEmptyString(size_t l);
Value_String *idris2_mkString(char *);

Value_Pointer *makePointer(void *);
Value_GCPointer *makeGCPointer(void *ptr_Raw, Value_Closure *onCollectFct);
Value_Buffer *makeBuffer(void *buf);
Value_Array *makeArray(int length);

extern Value_Int64 idris2_predefined_Int64[100];
extern Value_Bits64 idris2_predefined_Bits64[100];
extern Value_Integer idris2_predefined_Integer[100];
Value *idris2_getPredefinedInteger(int n);
extern Value_String idris2_predefined_nullstring;

// You need uncomment a debugging code in memoryManagement.c to use this.
void idris2_dumpMemoryStats(void);

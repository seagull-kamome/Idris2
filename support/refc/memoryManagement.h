#pragma once

#include "cBackend.h"

Value *newValue(size_t size);
Value *newReference(Value *source);
void removeReference(Value *source);

#define IDRIS2_NEW_VALUE(t) ((t *)newValue(sizeof(t)))

Value_Arglist *newArglist(int missing, int total);
Value_Constructor *newConstructor(int total, int tag, const char *name);

// copies arglist, no pointer bending
Value_Closure *makeClosureFromArglist(fun_ptr_t f, Value_Arglist *);

Value *idris2_mkDouble(double d);
#define idris2_mkChar(x) ((Value *)(((uintptr_t)(x) << 32) + 1))
#define idris2_mkBits8(x) ((Value *)(((uintptr_t)(x) << 32) + 1))
#define idris2_mkBits16(x) ((Value *)(((uintptr_t)(x) << 32) + 1))
#define idris2_mkBits32(x) ((Value *)(((uintptr_t)(x) << 32) + 1))
#define idris2_mkInt8(x) ((Value *)(((uintptr_t)(x) << 32) + 1))
#define idris2_mkInt16(x) ((Value *)(((uintptr_t)(x) << 32) + 1))
#define idris2_mkInt32(x) ((Value *)(((uintptr_t)(x) << 32) + 1))
#define idris2_mkBool(x) (idris2_mkInt8(x))

Value *idris2_mkBits64(uint64_t i);
Value *idris2_mkInt64(int64_t i);
Value_Integer *idris2_mkInteger();
Value_Integer *idris2_mkIntegerLiteral(char *i);
Value_String *idris2_mkEmptyString(size_t l);
Value_String *idris2_mkString(char *);

Value_Pointer *makePointer(void *);
Value_GCPointer *makeGCPointer(void *ptr_Raw, Value_Closure *onCollectFct);
Value_Buffer *makeBuffer(void *buf);
Value_Array *makeArray(int length);

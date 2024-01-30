#pragma once

#include "cBackend.h"

#define idris2_stringLength(x) ((Value *)makeInt64(((Value_String *)(x))->len))
#define idris2_stringHead(x) ((Value *)makeChar(((Value_String *)(x))->str[0]))
Value *idris2_stringTail(Value *str);

Value *idris2_stringReverse(Value *str);
#define idris2_stringIndex(s, i)                                               \
  ((Value *)makeChar(((Value_String *)(s))->str[((Value_Int64 *)(i))->i64]))
Value *idris2_stringCons(Value *c, Value *str);
Value *idris2_stringAppend(Value *a, Value *b);
Value *idris2_stringSubstr(Value *s, Value *start, Value *len);

Value *idris2_stringFastPack(Value *charList);
Value *idris2_stringFastUnpack(Value *str);
Value *idris2_stringFastConcat(Value *strList);

Value *idris2_stringIteratorNew(char *str);
Value *idris2_onCollectStringIterator(Value_Pointer *ptr, void *null);
Value *idris2_onCollectStringIterator_arglist(Value_Arglist *arglist);
Value *idris2_stringIteratorToString(void *a, char *str, Value *it_p,
                                     Value_Closure *f);
Value *idris2_stringIteratorNext(char *s, Value *it_p);

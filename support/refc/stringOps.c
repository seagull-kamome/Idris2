#include "stringOps.h"
#include "refc_util.h"

Value *idris2_stringTail(Value *_s) {
  Value_String const *const s = (Value_String const *)_s;
  if (s->len == 0)
    return newReference(_s);

  return (Value *)idris2_makeString(s->len - 1, s->str + 1);
}

Value *idris2_stringReverse(Value *_s) {
  Value_String const *src = (Value_String const *)_s;
  int n = src->len;
  Value_String *dst = idris2_makeEmptyString(n);

  char *p = dst->str;
  char const *q = src->str + n;
  while (n--)
    *p++ = *q--;

  return (Value *)dst;
}

Value *idris2_stringCons(Value *c, Value *_s) {
  Value_String const *s = (Value_String const *)_s;
  Value_String *r = idris2_makeEmptyString(s->len + 1);
  r->str[0] = ((Value_Char const *)c)->c;
  memcpy(r->str + 1, s->str, s->len);
  return (Value *)r;
}

Value *idris2_stringAppend(Value *a, Value *b) {
  Value_String const *stra = (Value_String const *)a;
  Value_String const *strb = (Value_String const *)b;

  if (stra->len == 0)
    return newReference(b);
  if (strb->len == 0)
    return newReference(a);

  Value_String *retVal = idris2_makeEmptyString(stra->len + strb->len);
  memcpy(retVal->str, ((Value_String *)a)->str, stra->len);
  memcpy(retVal->str + stra->len, ((Value_String *)b)->str, strb->len);

  return (Value *)retVal;
}

Value *idris2_stringSubstr(Value *start, Value *len, Value *s) {
  Value_String const *input = (Value_String *)s;
  int offset = extractInt(start); /* start and len is Nat. */
  int l = extractInt(len);

  int tailLen = input->len - offset;
  if (tailLen < l) {
    l = tailLen;
  }

  Value_String *retVal = idris2_makeEmptyString(l);
  memcpy(retVal->str, input->str + offset, l);

  return (Value *)retVal;
}

Value *idris2_stringFastPack(Value *charList) {
  Value_Constructor *current;

  int l = 0;
  current = (Value_Constructor *)charList;
  while (current->total == 2) {
    l++;
    current = (Value_Constructor *)current->args[1];
  }

  Value_String *retVal = idris2_makeEmptyString(l);
  char *p = retVal->str;

  current = (Value_Constructor *)charList;
  while (current->total == 2) {
    *p++ = ((Value_Char *)current->args[0])->c;
    current = (Value_Constructor *)current->args[1];
  }

  return (Value *)retVal;
}

Value *idris2_stringFastUnpack(Value *str) {
  Value_String const *src = (Value_String const *)str;
  if (src->len == 0) {
    return (Value *)newConstructor(0, 0, "Prelude_Types_Nil");
  }

  Value_Constructor *retVal =
      newConstructor(2, 1, "Prelude_Types__colon_colon");
  retVal->args[0] = (Value *)makeChar(src->str[0]);

  int i = 1;
  Value_Constructor *current = retVal;
  Value_Constructor *next;
  while (src->len > i) {
    next = newConstructor(2, 1, "Prelude_Types__colon_colon");
    next->args[0] = (Value *)makeChar(src->str[i]);
    current->args[1] = (Value *)next;

    i++;
    current = next;
  }
  current->args[1] = (Value *)newConstructor(0, 0, "Prelude_Types_Nil");

  return (Value *)retVal;
}

Value *idris2_stringFastConcat(Value *strList) {
  Value_Constructor *current;

  int totalLength = 0;
  current = (Value_Constructor *)strList;
  while (current->total == 2) {
    totalLength += strlen(((Value_String *)current->args[0])->str);
    current = (Value_Constructor *)current->args[1];
  }

  Value_String *retVal = idris2_makeEmptyString(totalLength);

  int offset = 0;
  current = (Value_Constructor *)strList;
  while (current->total == 2) {
    Value_String *currentStr = (Value_String *)current->args[0];
    memcpy(retVal->str + offset, currentStr->str, currentStr->len);

    offset += currentStr->len;
    current = (Value_Constructor *)current->args[1];
  }

  return (Value *)retVal;
}

typedef struct {
  char *str;
  int pos;
} String_Iterator;

Value *idris2_stringIteratorNew(char *str) {
  int l = strlen(str);

  String_Iterator *it = (String_Iterator *)malloc(sizeof(String_Iterator));
  IDRIS2_REFC_VERIFY(it, "malloc failed");
  it->str = (char *)malloc(l + 1);
  it->pos = 0;
  memcpy(it->str, str, l + 1); // Take a copy of str, in case it gets GCed

  Value_Arglist *arglist = newArglist(2, 2);
  Value *(*onCollectRaw)(Value_Arglist *) =
      idris2_onCollectStringIterator_arglist;
  Value_Closure *onCollect = makeClosureFromArglist(onCollectRaw, arglist);

  return (Value *)makeGCPointer(it, onCollect);
}

Value *idris2_onCollectStringIterator(Value_Pointer *ptr, void *null) {
  String_Iterator *it = (String_Iterator *)ptr->p;
  free(it->str);
  free(it);
  return NULL;
}

Value *idris2_onCollectStringIterator_arglist(Value_Arglist *arglist) {
  return idris2_onCollectStringIterator((Value_Pointer *)arglist->args[0],
                                        arglist->args[1]);
}

Value *idris2_stringIteratorToString(void *a, char *str, Value *it_p,
                                     Value_Closure *f) {
  String_Iterator *it = ((Value_GCPointer *)it_p)->p->p;
  Value *var_1 = (Value *)idris2_makeString(strlen(it->str + it->pos), it->str + it->pos);
  Value *retVal =  apply_closure((Value *)f, var_1);
  removeReference(var_1);
  return retVal;
}

Value *idris2_stringIteratorNext(char *s, Value *it_p) {
  String_Iterator *it = (String_Iterator *)((Value_GCPointer *)it_p)->p->p;
  char c = it->str[it->pos];

  if (c == '\0') {
    return (Value *)newConstructor(0, 0, "Data_String_Iterator_EOF");
  }

  it->pos++; // Ok to do this as StringIterator linear

  Value_Constructor *retVal =
      newConstructor(2, 1, "Data_String_Iterator_Character");
  retVal->args[0] = (Value *)makeChar(c);
  retVal->args[1] = newReference(it_p);

  return (Value *)retVal;
}

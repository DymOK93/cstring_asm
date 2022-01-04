#pragma once
#define EXTERN_C extern "C"  // NOLINT(cppcoreguidelines-macro-usage)

EXTERN_C char* StrCpy(char* dest, const char* src);
void TestStrCpy();

EXTERN_C char* StrNCpy(char* dest, const char* src, size_t count);
void TestStrNCpy();

EXTERN_C char* StrCat(char* dest, const char* src);
EXTERN_C char* StrNCat(char* dest, const char* src, size_t count);

EXTERN_C size_t StrLen(const char* str);
void TestStrLen();

EXTERN_C int StrCmp(const char* lhs, const char* rhs);
EXTERN_C int StrNCmp(const char* lhs, const char* rhs, size_t count);

EXTERN_C const char* StrChr(const char* str, int ch);

EXTERN_C const char* StrRChr(const char* str, int ch);

EXTERN_C size_t StrSpn(const char* dest, const char* src);

EXTERN_C size_t StrCSpn(const char* dest, const char* src);

EXTERN_C const char* StrPbrk(const char* dest, const char* breakset);

EXTERN_C const char* StrStr(const char* haystack, const char* needle);

EXTERN_C char* StrTok(char* str, const char* delim);

EXTERN_C const void* MemChr(const void* ptr, int ch, size_t count);

EXTERN_C int MemCmp(const void* lhs, const void* rhs, size_t count);

EXTERN_C void* MemSet(void* dest, int ch, size_t count);

EXTERN_C void* MemCpy(void* dest, const void* src, size_t count);
void TestMemCpy();

EXTERN_C void* MemMove(void* dest, const void* src, size_t count);

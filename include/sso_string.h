#ifndef SSO_STRING_H
#define SSO_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define __SSO_STRING_MAX_CAP (UINT64_MAX/2)
#define __SSO_STRING_64th_BIT_MAX ((uint64_t)1<<63)
#define __SSO_STRING_LOAD_FACTOR (float)1.5;

typedef struct SsoString {
    uint64_t __field_1;
    uint64_t __field_2;
    uint64_t __field_3;
} SsoString;

typedef struct __StackSsoStr {
    uint8_t chars[23];
    uint8_t type_flag;
} __StackSsoStr;

typedef struct __HeapSsoStr {
    uint8_t* ptr;
    uint64_t capacity;
    uint64_t length;
} __HeapSsoStr;


SsoString SsoString_from_cstr(const char* c_str);
char* SsoString_as_cstr(const SsoString* str);
int32_t SsoString_cmp(const SsoString* s1, const SsoString* s2);
bool SsoString_equals(const SsoString* s1, const SsoString* s2);
bool SsoString_free(SsoString* str);
bool SsoString_is_heap_allocated(const SsoString* str);
uint64_t SsoString_len(const SsoString* str);
SsoString SsoString_clone(const SsoString* str);
void SsoString_push_cstr(SsoString* str, char* c_str);
int64_t SsoString_find(const SsoString* str, const char* c_str);
int64_t SsoString_rfind(const SsoString* str, const char* c_str);
void SsoString_trim(SsoString* str);
int32_t SsoString_split(const SsoString* str, SsoString* output_buffer, uint64_t buffer_len);

#endif // SSO_STRING_H

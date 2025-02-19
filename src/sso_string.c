#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../include/sso_string.h"

/// @brief Creates an SsoString object from a regular C String
/// @param c_str
/// @return
SsoString SsoString_from_cstr(const char* c_str) {
	uint64_t length = strlen(c_str);
	SsoString str;

	if (length <= 22) {
		__StackSsoStr* str_ptr = (__StackSsoStr*) &str;
		memcpy(&str_ptr->chars[0], c_str, length);
		str_ptr->chars[length] = '\0';
		str_ptr->type_flag = 0;
	} else if (length >= __SSO_STRING_MAX_CAP) {
		perror("length of this string exceeds the maximum supported size (and almost certainly your available memory)");
		exit(1);
	} else {
		__HeapSsoStr* str_ptr = (__HeapSsoStr*) &str;
		str_ptr->capacity = (length + 1);
		str_ptr->length = length;
		str_ptr->ptr = malloc(str_ptr->capacity);
		memcpy(str_ptr->ptr, c_str, length);
		str_ptr->ptr[length] = '\0';

		str_ptr->length |= __SSO_STRING_64th_BIT_MAX;
	}

	return str;
}

/// @brief Returns a C String (a char pointer to a null terminated string) to the data held within the object itself
/// @param str
/// @return
char* SsoString_as_cstr(const SsoString* str) {
	uint64_t tag = str->__field_3 & __SSO_STRING_64th_BIT_MAX;
	if (tag) {
		__HeapSsoStr* str_ptr = (__HeapSsoStr*) str;
		return (char*) str_ptr->ptr;
	}
	__StackSsoStr* str_ptr = (__StackSsoStr*) str;
	return (char*) &str_ptr->chars[0];
}

/// @brief Exact same semantics as `strcmp`
/// @param s1
/// @param s2
/// @return
int32_t SsoString_cmp(const SsoString* s1, const SsoString* s2) {
	char* c_s1 = SsoString_as_cstr(s1);
	char* c_s2 = SsoString_as_cstr(s2);
	return strcmp(c_s1, c_s2);
}

/// @brief
/// @param s1
/// @param s2
/// @return Returns true if the two strings are equal
bool SsoString_equals(const SsoString* s1, const SsoString* s2) {
	char* c_s1 = SsoString_as_cstr(s1);
	char* c_s2 = SsoString_as_cstr(s2);
	return strcmp(c_s1, c_s2) == 0;
}

/// @brief Frees the heap memory used by the string (if any)
/// @param str
/// @return Returns `true` if the value was heap allocated and `false` if stack allocated
bool SsoString_free(SsoString* str) {
	uint64_t tag = str->__field_3 & __SSO_STRING_64th_BIT_MAX;
	if (tag) {
		__HeapSsoStr* str_ptr = (__HeapSsoStr*) str;
		free(str_ptr->ptr);
		return true;
	}

	return false;
}

/// @brief
/// @param str
/// @return Returns `true` if the value was heap allocated and `false` if stack allocated
bool SsoString_is_heap_allocated(const SsoString* str) {
	uint64_t tag = str->__field_3 & __SSO_STRING_64th_BIT_MAX;
	if (tag) {
		return true;
	}

	return false;
}

/// @brief
/// @param str
/// @return Returns the length of the string
uint64_t SsoString_len(const SsoString* str) {
	uint64_t tag = str->__field_3 & __SSO_STRING_64th_BIT_MAX;
	if (tag) {
		__HeapSsoStr* str_ptr = (__HeapSsoStr*) str;
		return str_ptr->length & (~__SSO_STRING_64th_BIT_MAX);
	}
	__StackSsoStr* str_ptr = (__StackSsoStr*) str;
	return (uint64_t) strlen((char*) str_ptr->chars);
}

/// @brief Performs a deep clone of the string (if heap allocated, the characters will be reallocated and copied)
/// @param str
/// @return
SsoString SsoString_clone(const SsoString* str) {
	uint64_t tag = str->__field_3 & __SSO_STRING_64th_BIT_MAX;
	if (tag) {
		__HeapSsoStr* str_ptr = (__HeapSsoStr*) str;
		uint64_t capacity = str_ptr->capacity;
		uint64_t length = str_ptr->length & (~__SSO_STRING_64th_BIT_MAX);

		SsoString new_str;
		__HeapSsoStr* new_str_ptr = (__HeapSsoStr*) &new_str;
		new_str_ptr->ptr = malloc(capacity);
		memcpy(new_str_ptr->ptr, str_ptr->ptr, length + 1);
	}
	return (*str);
}

/// @brief Adds on to the end of the string. May raise an allocation faulure
/// @param str
/// @param c_str
void SsoString_push_cstr(SsoString* str, char* c_str) {
    uint64_t curr_len = SsoString_len(str);
    uint64_t append_len = strlen(c_str);
    uint64_t new_len = curr_len + append_len;

    if (SsoString_is_heap_allocated(str)) {
        __HeapSsoStr* heap_str = (__HeapSsoStr*) str;
        if (new_len + 1 > heap_str->capacity) {
            uint64_t new_capacity = (uint64_t)((new_len + 1) * 1.5);
            uint8_t* new_ptr = realloc(heap_str->ptr, new_capacity);
            if (!new_ptr) {
                perror("Failed to reallocate memory in SsoString_push_cstr");
                exit(1);
            }
            heap_str->ptr = new_ptr;
            heap_str->capacity = new_capacity;
        }
        memcpy(heap_str->ptr + curr_len, c_str, append_len);
        heap_str->ptr[new_len] = '\0';
        heap_str->length = new_len | __SSO_STRING_64th_BIT_MAX;
    } else {
        __StackSsoStr* stack_str = (__StackSsoStr*) str;
        if (new_len <= 22) {
            memcpy(stack_str->chars + curr_len, c_str, append_len);
            stack_str->chars[new_len] = '\0';
        } else {
            uint64_t new_capacity = (uint64_t)((new_len + 1) * 1.5);
            uint8_t* heap_ptr = malloc(new_capacity);
            if (!heap_ptr) {
                perror("Failed to allocate memory in SsoString_push_cstr");
                exit(1);
            }
            memcpy(heap_ptr, stack_str->chars, curr_len);
            memcpy(heap_ptr + curr_len, c_str, append_len);
            heap_ptr[new_len] = '\0';

            __HeapSsoStr* heap_str = (__HeapSsoStr*) str;
            heap_str->ptr = heap_ptr;
            heap_str->capacity = new_capacity;
            heap_str->length = new_len | __SSO_STRING_64th_BIT_MAX;
        }
    }
}

/// @brief The index returned will be index of the first character of c_str
/// @param str
/// @param c_str
/// @return Returns the index of the 1st occurance of c_str in str. Returns -1 if not found
int64_t SsoString_find(const SsoString* str, const char* c_str)  {
    const char* haystack = SsoString_as_cstr(str);
    const char* found = strstr(haystack, c_str);
    if (found == NULL) {
        return -1;
    }
    return (int64_t)(found - haystack);
}

/// @brief This will start searching from the back. The index returned will be index of the first character of c_str.
/// @param str
/// @param c_str
/// @return Returns the index of the 1st occurance of c_str in str. Returns -1 if not found.
int64_t SsoString_rfind(const SsoString* str, const char* c_str) {
    const char* haystack = SsoString_as_cstr(str);
    uint64_t hay_len = SsoString_len(str);
    size_t needle_len = strlen(c_str);

	if (needle_len == 0) {
        return hay_len;
    } else if (needle_len > hay_len) {
        return -1;
    }

    for (int64_t i = hay_len - needle_len; i >= 0; i--) {
        if (strncmp(haystack + i, c_str, needle_len) == 0) {
            return i;
        }
    }
    return -1;
}

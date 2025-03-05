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

/// @brief Removes all white space characters from the start and end of the string. 
/// @param str 
void SsoString_trim(SsoString* str) {
    // Get C string representation and length
    char* c_str = SsoString_as_cstr(str);
    uint64_t len = SsoString_len(str);
    
    // If empty string, nothing to do
    if (len == 0) return;
    
    // Find first non-whitespace character
    uint64_t start = 0;
    while (start < len && isspace((unsigned char)c_str[start])) {
        start++;
    }
    
    // If string is all whitespace, make it empty
    if (start == len) {
        if (SsoString_is_heap_allocated(str)) {
            __HeapSsoStr* heap_str = (__HeapSsoStr*)str;
            heap_str->ptr[0] = '\0';
            heap_str->length = 0 | __SSO_STRING_64th_BIT_MAX;
        } else {
            __StackSsoStr* stack_str = (__StackSsoStr*)str;
            stack_str->chars[0] = '\0';
        }
        return;
    }
    
    // Find last non-whitespace character
    int64_t end = len - 1;
    while (end >= 0 && isspace((unsigned char)c_str[end])) {
        end--;
    }
    
    // Calculate new length
    uint64_t new_len = end - start + 1;
    
    // If no trimming needed, return
    if (start == 0 && (uint64_t)end == len - 1) {
        return;
    }
    
    // Update the string based on storage type
    if (SsoString_is_heap_allocated(str)) {
        __HeapSsoStr* heap_str = (__HeapSsoStr*)str;
        
        // Move characters if needed
        if (start > 0) {
            memmove(heap_str->ptr, heap_str->ptr + start, new_len);
        }
        
        // Add null terminator
        heap_str->ptr[new_len] = '\0';
        
        // Update length while preserving heap flag
        heap_str->length = new_len | __SSO_STRING_64th_BIT_MAX;
    } else {
        __StackSsoStr* stack_str = (__StackSsoStr*)str;
        
        // Move characters if needed
        if (start > 0) {
            memmove(stack_str->chars, stack_str->chars + start, new_len);
        }
        
        // Add null terminator
        stack_str->chars[new_len] = '\0';
    }
}

/// @brief 
/// @param str The String we want to split
/// @param output_buffer The buffer we want to place our strings into. 
/// @param buffer_len The length of the output buffer. If 0, SsoString_split will reassign `output_buffer` using malloc() 
/// @return 0 on success, 1 if the output buffer is not large enough
int32_t SsoString_split(const SsoString* str, SsoString* output_buffer, uint64_t buffer_len) {
    // Get C string representation
    const char* c_str = SsoString_as_cstr(str);
    uint64_t str_len = SsoString_len(str);
    
    // Count the number of words (splitting on whitespace)
    uint64_t word_count = 0;
    bool in_word = false;
    
    for (uint64_t i = 0; i < str_len; i++) {
        if (isspace((unsigned char)c_str[i])) {
            if (in_word) {
                in_word = false;
            }
        } else {
            if (!in_word) {
                word_count++;
                in_word = true;
            }
        }
    }
    
    // If buffer_len is 0, just return the count of words
    // This allows the caller to determine the needed buffer size first
    if (buffer_len == 0) {
        return word_count;
    }
    
    // Check if buffer is large enough
    if (buffer_len < word_count) {
        return 1; // Buffer too small
    }
    
    // Split the string and fill the buffer
    uint64_t output_index = 0;
    uint64_t word_start = 0;
    bool in_word_splitting = false;
    
    for (uint64_t i = 0; i <= str_len; i++) {
        if (i == str_len || isspace((unsigned char)c_str[i])) {
            if (in_word_splitting) {
                // Extract the word
                uint64_t word_len = i - word_start;
                
                // Create a temporary buffer for the word
                char* temp = (char*)malloc(word_len + 1);
                if (!temp) {
                    perror("Failed to allocate memory in SsoString_split");
                    exit(1);
                }
                
                // Copy the word to the temporary buffer
                memcpy(temp, &c_str[word_start], word_len);
                temp[word_len] = '\0'; // Ensure null termination
                
                // Convert to SsoString and store in output buffer
                output_buffer[output_index] = SsoString_from_cstr(temp);
                output_index++;
                
                // Free the temporary buffer
                free(temp);
                
                in_word_splitting = false;
            }
        } else {
            if (!in_word_splitting) {
                word_start = i;
                in_word_splitting = true;
            }
        }
    }
    
    return 0; // Success
}
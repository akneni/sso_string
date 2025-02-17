#include <stdio.h>
#include "../include/sso_string.h"

int main() {
    // Test 1: Create a short (stack-allocated) string
    SsoString s1 = SsoString_from_cstr("Hello");
    printf("Test 1:\n");
    printf("s1: `%s`, Length: %lu, Heap allocated: %d\n",
           SsoString_as_cstr(&s1),
           SsoString_len(&s1),
           SsoString_is_heap_allocated(&s1));
    
    // Test 2: Push additional text that still fits in the stack buffer
    SsoString_push_cstr(&s1, ", World!");
    printf("\nTest 2 (push staying on stack):\n");
    printf("s1: `%s`, Length: %lu, Heap allocated: %d\n",
           SsoString_as_cstr(&s1),
           SsoString_len(&s1),
           SsoString_is_heap_allocated(&s1));

    // Test 3: Push additional text that forces conversion from stack to heap
    SsoString_push_cstr(&s1, " -- And here comes a longer string that forces a heap allocation.");
    printf("\nTest 3 (push forcing heap allocation):\n");
    printf("s1: `%s`, Length: %lu, Heap allocated: %d\n",
           SsoString_as_cstr(&s1),
           SsoString_len(&s1),
           SsoString_is_heap_allocated(&s1));

    // Test 4: Create a heap-allocated string directly
    SsoString s2 = SsoString_from_cstr("0123456789 0123456789 0123456789 0123456789");
    printf("\nTest 4:\n");
    printf("s2: `%s`, Length: %lu, Heap allocated: %d\n",
           SsoString_as_cstr(&s2),
           SsoString_len(&s2),
           SsoString_is_heap_allocated(&s2));
    
    // Push additional text to the heap string
    SsoString_push_cstr(&s2, " APPENDED");
    printf("After push, s2: `%s`, Length: %lu, Heap allocated: %d\n",
           SsoString_as_cstr(&s2),
           SsoString_len(&s2),
           SsoString_is_heap_allocated(&s2));

    // Test 5: Compare two equal strings and two different strings
    SsoString s3 = SsoString_from_cstr("Test string");
    SsoString s4 = SsoString_from_cstr("Test string");
    SsoString s5 = SsoString_from_cstr("Different string");
    printf("\nTest 5 (comparison):\n");
    printf("s3: `%s`\ns4: `%s`\n  cmp: %d, equals: %d\n",
           SsoString_as_cstr(&s3),
           SsoString_as_cstr(&s4),
           SsoString_cmp(&s3, &s4),
           SsoString_equals(&s3, &s4));
    printf("s3: `%s`\ns5: `%s`\n  cmp: %d, equals: %d\n",
           SsoString_as_cstr(&s3),
           SsoString_as_cstr(&s5),
           SsoString_cmp(&s3, &s5),
           SsoString_equals(&s3, &s5));

    // Test 6: Clone a string and verify equality with the original
    SsoString s6 = SsoString_clone(&s3);
    printf("\nTest 6 (clone):\n");
    printf("s3: `%s`\ns6 (clone of s3): `%s`\n  Length of clone: %lu, equals original: %d\n",
           SsoString_as_cstr(&s3),
           SsoString_as_cstr(&s6),
           SsoString_len(&s6),
           SsoString_equals(&s3, &s6));

    // Test 7: SsoString_find
    SsoString s_find = SsoString_from_cstr("The quick brown fox jumps over the lazy dog");
    int64_t index = SsoString_find(&s_find, "brown");
    printf("\nTest 7 (SsoString_find):\n");
    printf("Searching for \"brown\" in \"%s\": Index = %ld (expected 10)\n",
           SsoString_as_cstr(&s_find), index);
    
    index = SsoString_find(&s_find, "lazy");
    printf("Searching for \"lazy\" in \"%s\": Index = %ld (expected 35)\n",
           SsoString_as_cstr(&s_find), index);
    
    index = SsoString_find(&s_find, "cat");
    printf("Searching for \"cat\" in \"%s\": Index = %ld (expected -1)\n",
           SsoString_as_cstr(&s_find), index);
    
    // Test 8: SsoString_rfind
    SsoString s_rfind = SsoString_from_cstr("abcabcabc");
    index = SsoString_rfind(&s_rfind, "abc");
    printf("\nTest 8 (SsoString_rfind):\n");
    printf("Reverse searching for \"abc\" in \"%s\": Index = %ld (expected 6)\n",
           SsoString_as_cstr(&s_rfind), index);
    
    index = SsoString_rfind(&s_rfind, "");
    printf("Reverse searching for an empty string in \"%s\": Index = %ld (expected 9)\n",
           SsoString_as_cstr(&s_rfind), index);
    
    index = SsoString_rfind(&s_rfind, "zzz");
    printf("Reverse searching for \"zzz\" in \"%s\": Index = %ld (expected -1)\n",
           SsoString_as_cstr(&s_rfind), index);
    
    // Free any heap-allocated memory.
    SsoString_free(&s1);
    SsoString_free(&s2);
    SsoString_free(&s3);
    SsoString_free(&s4);
    SsoString_free(&s5);
    SsoString_free(&s6);
    SsoString_free(&s_find);
    SsoString_free(&s_rfind);

    return 0;
}

#include <stdio.h>
#include "../include/sso_string.h"


void test_SsoString_trim() {
       // Test 9: SsoString_trim
       printf("\nTest 9 (SsoString_trim):\n");

       // Test 9.1: Leading whitespace
       SsoString s_trim1 = SsoString_from_cstr("   Leading whitespace");
       printf("Before trim: `%s`, Length: %lu\n", 
              SsoString_as_cstr(&s_trim1), 
              SsoString_len(&s_trim1));
       SsoString_trim(&s_trim1);
       printf("After trim:  `%s`, Length: %lu\n", 
              SsoString_as_cstr(&s_trim1), 
              SsoString_len(&s_trim1));

       // Test 9.2: Trailing whitespace
       SsoString s_trim2 = SsoString_from_cstr("Trailing whitespace   \t\n");
       printf("\nBefore trim: `%s`, Length: %lu\n", 
              SsoString_as_cstr(&s_trim2), 
              SsoString_len(&s_trim2));
       SsoString_trim(&s_trim2);
       printf("After trim:  `%s`, Length: %lu\n", 
              SsoString_as_cstr(&s_trim2), 
              SsoString_len(&s_trim2));

       // Test 9.3: Both leading and trailing whitespace
       SsoString s_trim3 = SsoString_from_cstr("  \t Both leading and trailing whitespace \n ");
       printf("\nBefore trim: `%s`, Length: %lu\n", 
              SsoString_as_cstr(&s_trim3), 
              SsoString_len(&s_trim3));
       SsoString_trim(&s_trim3);
       printf("After trim:  `%s`, Length: %lu\n", 
              SsoString_as_cstr(&s_trim3), 
              SsoString_len(&s_trim3));

       // Test 9.4: No whitespace to trim
       SsoString s_trim4 = SsoString_from_cstr("NoWhitespace");
       printf("\nBefore trim: `%s`, Length: %lu\n", 
              SsoString_as_cstr(&s_trim4), 
              SsoString_len(&s_trim4));
       SsoString_trim(&s_trim4);
       printf("After trim:  `%s`, Length: %lu\n", 
              SsoString_as_cstr(&s_trim4), 
              SsoString_len(&s_trim4));

       // Test 9.5: All whitespace
       SsoString s_trim5 = SsoString_from_cstr("   \t\n  ");
       printf("\nBefore trim: `%s`, Length: %lu\n", 
              SsoString_as_cstr(&s_trim5), 
              SsoString_len(&s_trim5));
       SsoString_trim(&s_trim5);
       printf("After trim:  `%s`, Length: %lu\n", 
              SsoString_as_cstr(&s_trim5), 
              SsoString_len(&s_trim5));

       // Test 9.6: Heap-allocated string with whitespace
       SsoString s_trim6 = SsoString_from_cstr("    This is a long string that will be heap allocated with whitespace on both ends    ");
       printf("\nBefore trim: `%s`, Length: %lu, Heap allocated: %d\n", 
              SsoString_as_cstr(&s_trim6), 
              SsoString_len(&s_trim6),
              SsoString_is_heap_allocated(&s_trim6));
       SsoString_trim(&s_trim6);
       printf("After trim:  `%s`, Length: %lu, Heap allocated: %d\n", 
              SsoString_as_cstr(&s_trim6), 
              SsoString_len(&s_trim6),
              SsoString_is_heap_allocated(&s_trim6));



       // Free trim strings
       SsoString_free(&s_trim1);
       SsoString_free(&s_trim2);
       SsoString_free(&s_trim3);
       SsoString_free(&s_trim4);
       SsoString_free(&s_trim5);
       SsoString_free(&s_trim6);
}

void test_SsoString_split() {
       printf("\nTest 10 (SsoString_split):\n");
   
       // Test 10.1: Basic splitting by space
       SsoString s_split1 = SsoString_from_cstr("This is a test");
       uint64_t buffer_len = 0;
       
       // First call with buffer_len=0 to get the count
       int32_t result = SsoString_split(&s_split1, " ", NULL, &buffer_len);
       printf("String: \"%s\" split by space has %lu parts\n", 
              SsoString_as_cstr(&s_split1), buffer_len);
   
       // Now allocate and split
       SsoString* split_buffer1 = (SsoString*)malloc(buffer_len * sizeof(SsoString));
       result = SsoString_split(&s_split1, " ", split_buffer1, &buffer_len);
       printf("Split result: %d (expected 0)\n", result);
   
       // Print each part
       printf("Parts:\n");
       for (uint64_t i = 0; i < buffer_len; i++) {
           printf("  Part %lu: \"%s\", Length: %lu\n", 
                  i, 
                  SsoString_as_cstr(&split_buffer1[i]), 
                  SsoString_len(&split_buffer1[i]));
       }
   
       // Free the split strings
       for (uint64_t i = 0; i < buffer_len; i++) {
           SsoString_free(&split_buffer1[i]);
       }
       free(split_buffer1);
   
       // Test 10.2: Splitting by a multi-character delimiter
       SsoString s_split2 = SsoString_from_cstr("apple,banana,orange,grape");
       buffer_len = 0;
       
       result = SsoString_split(&s_split2, ",", NULL, &buffer_len);
       printf("\nString: \"%s\" split by comma has %lu parts\n", 
              SsoString_as_cstr(&s_split2), buffer_len);
   
       SsoString* split_buffer2 = (SsoString*)malloc(buffer_len * sizeof(SsoString));
       result = SsoString_split(&s_split2, ",", split_buffer2, &buffer_len);
       printf("Split result: %d (expected 0)\n", result);
   
       printf("Parts:\n");
       for (uint64_t i = 0; i < buffer_len; i++) {
           printf("  Part %lu: \"%s\", Length: %lu\n", 
                  i, 
                  SsoString_as_cstr(&split_buffer2[i]), 
                  SsoString_len(&split_buffer2[i]));
       }
   
       // Free the split strings
       for (uint64_t i = 0; i < buffer_len; i++) {
           SsoString_free(&split_buffer2[i]);
       }
       free(split_buffer2);
   
       // Test 10.3: Splitting with delimiter at beginning and end
       SsoString s_split3 = SsoString_from_cstr("|first|second|third|");
       buffer_len = 0;
       
       result = SsoString_split(&s_split3, "|", NULL, &buffer_len);
       printf("\nString: \"%s\" split by \"|\" has %lu parts\n", 
              SsoString_as_cstr(&s_split3), buffer_len);
   
       SsoString* split_buffer3 = (SsoString*)malloc(buffer_len * sizeof(SsoString));
       result = SsoString_split(&s_split3, "|", split_buffer3, &buffer_len);
       
       printf("Parts:\n");
       for (uint64_t i = 0; i < buffer_len; i++) {
           printf("  Part %lu: \"%s\", Length: %lu\n", 
                  i, 
                  SsoString_as_cstr(&split_buffer3[i]), 
                  SsoString_len(&split_buffer3[i]));
       }
   
       // Free the split strings
       for (uint64_t i = 0; i < buffer_len; i++) {
           SsoString_free(&split_buffer3[i]);
       }
       free(split_buffer3);
   
       // Test 10.4: Empty delimiter (splits by character)
       SsoString s_split4 = SsoString_from_cstr("abc");
       buffer_len = 0;
       
       result = SsoString_split(&s_split4, "", NULL, &buffer_len);
       printf("\nString: \"%s\" split by empty delimiter has %lu parts\n", 
              SsoString_as_cstr(&s_split4), buffer_len);
   
       SsoString* split_buffer4 = (SsoString*)malloc(buffer_len * sizeof(SsoString));
       result = SsoString_split(&s_split4, "", split_buffer4, &buffer_len);
       
       printf("Parts:\n");
       for (uint64_t i = 0; i < buffer_len; i++) {
           printf("  Part %lu: \"%s\", Length: %lu\n", 
                  i, 
                  SsoString_as_cstr(&split_buffer4[i]), 
                  SsoString_len(&split_buffer4[i]));
       }
   
       // Free the split strings
       for (uint64_t i = 0; i < buffer_len; i++) {
           SsoString_free(&split_buffer4[i]);
       }
       free(split_buffer4);
   
       // Test 10.5: Empty string
       SsoString s_split5 = SsoString_from_cstr("");
       buffer_len = 0;
       
       result = SsoString_split(&s_split5, ",", NULL, &buffer_len);
       printf("\nEmpty string split by comma has %lu parts\n", buffer_len);
   
       // Test 10.6: Insufficient buffer
       SsoString s_split6 = SsoString_from_cstr("one:two:three:four");
       buffer_len = 2; // Too small for 4 parts
       
       SsoString* split_buffer6 = (SsoString*)malloc(buffer_len * sizeof(SsoString));
       result = SsoString_split(&s_split6, ":", split_buffer6, &buffer_len);
       printf("\nSplit with insufficient buffer result: %d (expected 1)\n", result);
       printf("Required buffer size: %lu\n", buffer_len);
       free(split_buffer6);
   
       // Test 10.7: Delimiter not found
       SsoString s_split7 = SsoString_from_cstr("No delimiter here");
       buffer_len = 0;
       
       result = SsoString_split(&s_split7, "XYZ", NULL, &buffer_len);
       printf("\nString without delimiter has %lu parts\n", buffer_len);
   
       // Free all strings
       SsoString_free(&s_split1);
       SsoString_free(&s_split2);
       SsoString_free(&s_split3);
       SsoString_free(&s_split4);
       SsoString_free(&s_split5);
       SsoString_free(&s_split6);
       SsoString_free(&s_split7);
   }

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


    test_SsoString_trim();
    test_SsoString_split();

    return 0;
}

/* Bubble sort algorithm. 
 * 
 * Compile with GCC (or MinGW):
 * gcc -std=c99 BubbleSort.c -o BubbleSort
 * 
 * For Microsoft Visual Studio Community Edition
 * execute with: CTRL+F5
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Pointer to function Bubble Sort for test. */
void (*bubbleSortfPtr)(void *, size_t, size_t,
                int (*)(const void *, const void *));
typedef void (*bSortfPtr)(void *, size_t, size_t,
                int (*)(const void *, const void *));

void bubbleSortM(void *a, size_t count, size_t size,
                int (*comp)(const void *, const void *) )
{
    size_t i, j;
    const size_t j_temp = count - 1;
    void *current;
    void *prev;
    void *buffer = malloc(count*size);
    memset(buffer, 0, count*size);
    for(i = 0; i < count; ++i) {
        for(j = j_temp; j > i; --j) {
            current = a + j * size;
            prev = current - size;
            if(comp(prev, current) > 0) {
                memcpy(buffer, prev, size);
                memcpy(prev, current, size);
                memcpy(current, buffer, size);
            }
        }
    }
    free(buffer);
}

void optimizedBubbleSortM(void *a, size_t count, size_t size,
                int (*comp)(const void *, const void *) )
{
    /*Optimized version based on: https://neerc.ifmo.ru/wiki/index.php?title=Сортировка_пузырьком */
    size_t i = 0;
    size_t j = 0;
    size_t temp = 0;
    char t = 1;
    void *current;
    void *next;
    void *buffer = malloc(count*size);
    memset(buffer, 0, count*size);
    while(t) {
        t = 0;
        temp = count - i - 1;
        for(j = 0; j < temp; ++j) {
            current = a + j * size;
            next = current + size;
            if(comp(current, next) > 0) {
                //swap(a[j], a[j + 1]);
                memcpy(buffer, next, size);
                memcpy(next, current, size);
                memcpy(current, buffer, size);
                t = 1;
            }
        }
        i += 1;
    }
    free(buffer);
}

/* Version with XOR swap algorithm. */
void swap_xor(void *a, void *b, size_t size)
{
    char *x;
    char *y;
    /* XOR swap algorithm */
    for(size_t i = 0; i < size; ++i) {
        x = a + i;
        y = b + i;
        *x = *x ^ *y;
        *y = *y ^ *x;
        *x = *x ^ *y;
    }
}

void bubbleSortX(void *a, size_t count, size_t size,
                int (*comp)(const void *, const void *) )
{
    size_t i, j;
    const size_t j_temp = count - 1;
    for(i = 0; i < count; ++i) {
        for(j = j_temp; j > i; --j) {
            void *current = a + j * size;
            void *prev = current - size;
            if(comp(prev, current) > 0) {
                swap_xor(prev, current, size);
            }
        }
    }
}

void optimizedBubbleSortX(void *a, size_t count, size_t size,
                int (*comp)(const void *, const void *) )
{
    /*Optimized version based on: https://neerc.ifmo.ru/wiki/index.php?title=Сортировка_пузырьком */
    size_t i = 0;
    size_t j = 0;
    size_t temp = 0;
    char t = 1;
    void *current;
    void *next;
    while(t) {
        t = 0;
        temp = count - i - 1;
        for(j = 0; j < temp; ++j) {
            current = a + j * size;
            next = current + size;
            if(comp(current, next) > 0) {
                swap_xor(current, next, size);
                t = 1;
            }
        }
        i += 1;
    }
}

void optimizedBubbleSortX2(void *a, size_t count, size_t size,
                int (*comp)(const void *, const void *) )
{
    /*More optimized version based on: https://neerc.ifmo.ru/wiki/index.php?title=Сортировка_пузырьком */
    size_t i = 0;
    char t = 1;
    void *current = a;
    void *next;
    void *end;
    while(t) {
        t = 0;
        end = a + (count - i - 1) * size;
        for(current = a; current < end; current += size) {
            next = current + size;
            if(comp(current, next) > 0) {
                swap_xor(current, next, size);
                t = 1;
            }
        }
        i += 1;
    }
}

int comparator(const void* a, const void* b)
{
    const int arg1 = *(const int*)a;
    const int arg2 = *(const int*)b;
	return arg1 - arg2;
    return 0;
}

/* ----------------------------------------
Test */
int greaterInt(const int *arr, size_t count)
{
    const size_t right = count - 1;
    for(size_t i = 0; i < right; ++i) {
        if(arr[i] > arr[i+1])
            return -1;
    }
    return 1;
}

void printInt(const int *arr, size_t count)
{
    for(size_t i = 0; i < count; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void testArr(int *arr, size_t count, bSortfPtr func)
{
    func(arr, count, sizeof(int), comparator);
    if(greaterInt(arr, count) < 0) {
        printf("Error sorting with array:\n");
        printInt(arr, count);
    } else {
        printf("Sort ok.\n");
    }
}

/* Simple test */
void test(bSortfPtr func)
{
    const size_t sa1 = 1;
    int a1[1] = {5};
    const size_t sa2 = 2;
    int a2[2] = {5, 3};
    const size_t sa3 = 7;
    int a3[3] = {5, 3, 7};
    const size_t sa7 = 7;
    int a7[7] = {5, 3, 7, 2, 9, 1, 0};
    testArr(a1, sa1, func);
    testArr(a2, sa2, func);
    testArr(a3, sa3, func);
    testArr(a7, sa7, func);
}

int main()
{
	printf("Bubble sort algorithm.\n\n");
    printf("Test bubbleSortM function:\n");
    test(&bubbleSortM);
    printf("Test optimizedBubbleSortM function:\n");
    test(&optimizedBubbleSortM);
    printf("Test optimizedBubbleSortM function:\n");
    test(&optimizedBubbleSortM);
    printf("Test bubbleSortX function:\n");
    test(&bubbleSortX);
    printf("Test optimizedBubbleSortX function:\n");
    test(&optimizedBubbleSortX);
    printf("Test optimizedBubbleSortX2 function:\n");
    test(&optimizedBubbleSortX2);
    return 0;
}

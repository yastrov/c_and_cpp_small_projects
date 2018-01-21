/* String sort algorithms. 
 * 
 * Compile with GCC (or MinGW):
 * gcc -std=c99 string_sort.c -o string_sort
 * 
 * For Microsoft Visual Studio Community Edition
 * execute with: CTRL+F5
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static
int comparator(const char *a, const char *b)
{
    #ifdef DEBUG
    printf("CMP ADDR %p %p\n", a, b);
    #endif // DEBUG
    return strcmp(a, b);
}

void str_naive_sort(char **array, size_t count,
                    int (*comp)(const char *, const char *)
                    )
{
    char *addrI = NULL;
    char *addrJ = NULL;
    char *tmp;
    for(size_t i = 0; i < count; ++i) {
        for(size_t j = i+1; j < count; ++j) {
            addrI = *(array + i);
            addrJ = *(array + j);
            #ifdef DEBUG
            printf("%p %p\n", addrI, addrJ);
            #endif // DEBUG
            if(comp(addrI, addrJ) > 0) {
                tmp = addrI;            /* *(array + i) */
                array[i] = addrJ;       /* *(array + j) */
                array[j] = tmp;
            }
        }
    }
}

void str_bubble_sort(char **array, size_t count,
                     int (*comp)(const char *, const char *) )
{
    size_t i, j;
    char *tmp;
    const size_t j_temp = count - 1;
    for(i = 0; i < count; ++i) {
        for(j = j_temp; j > i; --j) {
            if(comp(array[j-1], array[j]) > 0) {
                tmp = array[j];
                array[j] = array[j-1];
                array[j-1] = tmp;
            }
        }
    }
}

void str_optimized_bubble_sort(char **array, size_t count,
                                int (*comp)(const char *, const char *) )
{
    /* More optimized version based on article: https://neerc.ifmo.ru/wiki/index.php?title=Сортировка_пузырьком */
    size_t i = 0;
    char t = 1;
    size_t current = 0;
    size_t next;
    size_t end;
    char *tmp;
    while(t) {
        t = 0;
        end = count - i - 1;
        for(current = 0; current < end; ++current) {
            next = current + 1;
            if(comp(array[current], array[next]) > 0) {
                tmp = array[current];
                array[current] = array[next];
                array[next] = tmp;
                t = 1;
            }
        }
        i += 1;
    }
}

/* wchar_t version */
void wstr_naive_sort(wchar_t **array, size_t count,
                    int (*comp)(const wchar_t *, const wchar_t *)
                    )
{
    wchar_t *addrI = NULL;
    wchar_t *addrJ = NULL;
    wchar_t *tmp;
    for(size_t i = 0; i < count; ++i) {
        for(size_t j = i+1; j < count; ++j) {
            addrI = *(array + i);
            addrJ = *(array + j);
            #ifdef DEBUG
            printf("%p %p\n", addrI, addrJ);
            #endif // DEBUG
            if(comp(addrI, addrJ) > 0) {
                tmp = addrI;            /* *(array + i) */
                array[i] = addrJ;       /* *(array + j) */
                array[j] = tmp;
            }
        }
    }
}

void wstr_bubble_sort(wchar_t **array, size_t count,
                      int (*comp)(const wchar_t *, const wchar_t *) )
{
    size_t i, j;
    wchar_t *tmp;
    const size_t j_temp = count - 1;
    for(i = 0; i < count; ++i) {
        for(j = j_temp; j > i; --j) {
            if(comp(array[j-1], array[j]) > 0) {
                tmp = array[j];
                array[j] = array[j-1];
                array[j-1] = tmp;
            }
        }
    }
}

void wstr_optimized_bubble_sort(wchar_t **array, size_t count,
                                int (*comp)(const wchar_t *, const wchar_t *) )
{
    /* Optimized version based on article: https://neerc.ifmo.ru/wiki/index.php?title=Сортировка_пузырьком */
    size_t i = 0;
    wchar_t t = 1;
    size_t current = 0;
    size_t next;
    size_t end;
    wchar_t *tmp;
    while(t) {
        t = 0;
        end = count - i - 1;
        for(current = 0; current < end; ++current) {
            next = current + 1;
            if(comp(array[current], array[next]) > 0) {
                tmp = array[current];
                array[current] = array[next];
                array[next] = tmp;
                t = 1;
            }
        }
        i += 1;
    }
}

#define STR_CNT 3

int main()
{
    char *arr[STR_CNT] = {"B\0", "A\0", "C\0"};
    char *a;
    for(size_t i = 0; i<STR_CNT; ++i) {
        a = arr[i];
        printf("ADDR %p STR %s\n", a, a);
    }

    str_optimized_bubble_sort(arr, STR_CNT, comparator);

    for(size_t i = 0; i<STR_CNT; ++i) {
        printf("%s\n", arr[i]);
    }
    return 0;
}

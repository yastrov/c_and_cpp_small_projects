/* Stack via array, universal version, v1. 
 * 
 * Compile with GCC (or MinGW):
 * gcc -std=c99 stack_via_array_universal_v1.c -o stack_via_array_universal_v1
 * 
 * For Microsoft Visual Studio Community Edition execute with: CTRL+F5
 * 
 * Author: Yuriy Astrov
 * License: MIT License
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ELEMENTS 2

void init_stack(void *stack, size_t *count, size_t size, size_t max_count)
{
	if(stack == NULL || count == NULL) return;
	if(*count == 0) return;
	memset(stack, 0, max_count*size);
	*count = 0;
}

void push(void *stack, size_t *count, size_t size, const void *data)
{
	if(stack == NULL || count == NULL) return;
	if(data == NULL) return;
	memcpy(stack + (*count)*size, data, size);
	(*count) += 1;
}

int push_safe(void *stack, size_t *count, size_t size, const void *data, size_t max_count)
{
	if(stack == NULL || count == NULL) return -1;
	if(*count > max_count-1) return -1;
	if(data == NULL) return -1;
	memcpy(stack + (*count)*size, data, size);
	(*count) += 1;
	return 0;
}

void pop(void *stack, size_t *count, size_t size, void *data)
{
	if(data == NULL) return;
	memset(data, 0, size);
	if(stack == NULL || count == NULL) return;
	if(*count == 0) return;
	memcpy(data, stack + (*count-1)*size, size);
	*count -= 1;
}

int pop_safe(void *stack, size_t *count, size_t size, void *data, size_t max_count)
{
	if(data == NULL) return -1;
	memset(data, 0, size);
	if(stack == NULL || count == NULL) return -1;
	if(*count == 0) return -1;
	if(*count > max_count) return -1;
	memcpy(data, stack + (*count-1)*size, size);
	*count -= 1;
	return 0;
}


/* Function return raw pointer to top element. */
void *pop_r_unsafe(void *stack, size_t *count, size_t size)
{
	if(stack == NULL || count == NULL) return NULL;
	if(*count == 0) return NULL;
	size_t c = *count - 1;
	*count -= 1;
	return stack + c*size;
}

/* Function return raw pointer to top element. */
void *pop_r_s_unsafe(void *stack, size_t *count, size_t size, size_t max_count)
{
	if(stack == NULL || count == NULL) return NULL;
	if(*count == 0) return NULL;
	if(*count > max_count) return NULL;
	size_t c = *count - 1;
	*count -= 1;
	return stack + c*size;
}

void peek(const void *stack, const size_t *count, size_t size, void *data)
{
	if(stack == NULL || count == NULL) return;
	if(*count == 0) return;
	if(data == NULL) return;
	memset(data, 0, size);
	memcpy(data, stack + (*count - 1)*size, size);
}

/* Function return raw pointer to top element. */
void *peek_r_unsafe(void *stack, const size_t *count, size_t size)
{
	if(stack == NULL || count == NULL) return NULL;
	if(*count == 0) return NULL;
	return stack + (*count -1)*size;
}

void peek_safe(const void *stack, const size_t *count, size_t size, void *data, size_t max_count)
{
	if(data == NULL) return;
	memset(data, 0, size);
	if(stack == NULL || count == NULL) return;
	if(*count == 0) return;
	if(*count > max_count) return;
	memcpy(data, stack + (*count - 1)*size, size);
}

/* Function return raw pointer to top element. */
void *peek_r_s_unsafe(void *stack, const size_t *count, size_t size, size_t max_count)
{
	if(stack == NULL || count == NULL) return NULL;
	if(*count == 0) return NULL;
	if(*count > max_count) return NULL;
	return stack + (*count -1)*size;
}

void print_stack_debug(void *stack, const size_t *count)
{
	printf("STACK INFO START\n");
	/* Print the address pointed to by stack. */
	printf("Address pointed to by stackinfo:\t%p\n", stack);
	/* print the address of stack itself. */
	printf("Address of stackinfo:\t%p\n", &stack);
	printf("COUNT:\t%d\n", *count);
	printf("VALUES: ");
	int *arr = (int*)stack;
	for(size_t i=0; i < *count; ++i) {
		printf("%d ", arr[i]);
	}
	printf("\n");
	
	printf("STACK INFO END\n");
}

int main()
{
	printf("Stack via array, universal version v1.\n\n");
	int data[MAX_ELEMENTS];
	size_t count = 0;
	init_stack(data, &count, sizeof(int), MAX_ELEMENTS);
	int val = 1;
	push(data, &count, sizeof(int), &val);
	int retVal = 0;
	peek_safe(data, &count, sizeof(int), &retVal, MAX_ELEMENTS);
	printf("Val %d\n", retVal);
	
	printf("\n");
	print_stack_debug(data, &count);
    return 0;
}

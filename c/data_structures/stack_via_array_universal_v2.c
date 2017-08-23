/* Stack via array, universal version, v1. 
 * 
 * Compile with GCC (or MinGW):
 * gcc -std=c99 stack_via_array_universal_v2.c -o stack_via_array_universal_v2
 * 
 * For Microsoft Visual Studio Community Edition execute with: CTRL+F5
 * 
 * Author: Yuriy Astrov
 * License: MIT License
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ELEMENTS 20

struct STACKINFO {
	void *stack;
	size_t count;
	size_t size;
	size_t max_count;
	void *current;
};

typedef struct STACKINFO STACK;

enum STACK_RESULT {
	STACK_OK = 0,
	STACK_ERR = 1
};

typedef enum STACK_RESULT STACK_RESULT_E;

void init_stack(STACK *stackinfo, void *stack, size_t size, size_t max_count)
{
	memset(stackinfo, 0, sizeof(STACK));
	if(stackinfo == NULL) return;
	if(stack == NULL) return;
	if(size == 0 || max_count == 0) return;
	memset(stack, 0, max_count*size);
	stackinfo->stack = stack;
	stackinfo->current = NULL;
	/* stackinfo->current = stack-size; */
	stackinfo->size = size;
	stackinfo->max_count = max_count;
	stackinfo->count = 0;
}

int init_stack_safe(STACK *stackinfo, void *stack, size_t size, size_t max_count)
{
	if(stackinfo == NULL) return STACK_ERR;
	if(stack == NULL) return STACK_ERR;
	if(size == 0 || max_count) return STACK_ERR;
	memset(stack, 0, max_count*size);
	stackinfo->stack = stack;
	stackinfo->current = NULL;
	stackinfo->size = size;
	stackinfo->max_count = max_count;
	stackinfo->count = 0;
	return STACK_OK;
}

void push(STACK *stackinfo, const void *data)
{
	if(stackinfo->count >= stackinfo->max_count) return;
	if(data == NULL) return;
	if(stackinfo->current == NULL) {
		stackinfo->current = stackinfo->stack;
	} else {
		stackinfo->current = stackinfo->current + stackinfo->size;
	}
	memcpy(stackinfo->current, data, stackinfo->size);
	stackinfo->count += 1;
}

int push_safe(STACK *stackinfo, const void *data)
{
	if(stackinfo->count >= stackinfo->max_count) return STACK_ERR;
	if(data == NULL) return STACK_ERR;
	if(stackinfo->current == NULL) {
		stackinfo->current = stackinfo->stack;
	} else {
		stackinfo->current = stackinfo->current + stackinfo->size;
	}
	memcpy(stackinfo->current, data, stackinfo->size);
	stackinfo->count += 1;
	return STACK_OK;
}

int pop_safe(STACK *stackinfo, void *data)
{
	if(data == NULL) return STACK_ERR;
	memset(data, 0, stackinfo->size);
	if(stackinfo->count == 0 || stackinfo->count > stackinfo->max_count)
		return STACK_ERR;
	memcpy(data, stackinfo->current, stackinfo->size);
	if(stackinfo->current > stackinfo->stack) {
		stackinfo->current = stackinfo->current - stackinfo->size;
	} else {
		stackinfo->current = NULL;
	}
	stackinfo->count -= 1;
	return STACK_OK;
}

/* Function return raw pointer to top element. */
void *pop_unsafe(STACK *stackinfo)
{
	if(stackinfo->count == 0 || stackinfo->count > stackinfo->max_count)
		return NULL;
	void *result = stackinfo->current;
	if(stackinfo->current > stackinfo->stack) {
		stackinfo->current = stackinfo->current - stackinfo->size;
	} else {
		stackinfo->current = NULL;
	}
	stackinfo->count -= 1;
	return result;
}

void peek(const STACK *stackinfo, void *data)
{
	if(data == NULL) return;
	if(stackinfo->count == 0 || stackinfo->count > stackinfo->max_count)
		return;
	memset(data, 0, stackinfo->size);
	memcpy(data, stackinfo->current, stackinfo->size);
}

int peek_safe(const STACK *stackinfo, void *data)
{
	if(data == NULL) return STACK_ERR;
	memset(data, 0, stackinfo->size);
	if(stackinfo->count == 0 || stackinfo->count > stackinfo->max_count)
		return STACK_ERR;
	memcpy(data, stackinfo->current, stackinfo->size);
	return STACK_OK;
}

/* Function return raw pointer to top element. */
void *peek_unsafe(const STACK *stackinfo)
{
	if(stackinfo->count == 0 || stackinfo->count > stackinfo->max_count)
		return NULL;
	return stackinfo->current;
}

void print_stack_debug(const STACK *stackinfo)
{
	printf("STACK INFO START\n");
	/*Print the address pointed to by stackinfo. */
	printf("Address pointed to by stackinfo:\t%p\n", stackinfo);
	/*print the address of stackinfo itself. */
	printf("Address of stackinfo:\t%p\n", &stackinfo );
	
	/*Print the address pointed to by stackinfo->stack. */
	printf("Address pointed to by stackinfo->stack:\t%p\n", stackinfo->stack);
	/*print the address of stackinfo->stack itself. */
	printf("Address of stackinfo->stack:\t%p\n", &stackinfo->stack);
	
	/*Print the address pointed to by stackinfo->current. */
	printf("Address pointed to by stackinfo->current:\t%p\n", stackinfo->current);
	/*print the address of stackinfo->current itself. */
	printf("Address of stackinfo->current:\t%p\n", &stackinfo->current);
	
	printf("COUNT:\t%d\n", stackinfo->count);
	printf("MAX COUNT:\t%d\n", stackinfo->max_count);
	int retVal;
	const int result = peek_safe(stackinfo, &retVal);
	if(result == STACK_OK) { 
		printf("PEEK: %d\n", retVal);
	}
	printf("VALUES: ");
	int *arr = (int*)stackinfo->stack;
	for(size_t i=0; i < stackinfo->count; ++i) {
		printf("%d ", arr[i]);
	}
	printf("\n");
	
	printf("STACK INFO END\n");
}

int main()
{
	printf("Stack via array, universal version v2.\n\n");
	int data[MAX_ELEMENTS];
	STACK stack;
	init_stack(&stack, data, sizeof(int), MAX_ELEMENTS);
	int val = 1;
	push(&stack, &val);
	val = 2;
	push(&stack, &val);
	val = 3;
	push(&stack, &val);
	int retVal;
	peek_safe(&stack, &retVal); 
	printf("PEEK: %d\n", retVal);
	
	print_stack_debug(&stack);
    return 0;
}

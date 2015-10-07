/*
MergeSort
copy from:
https://bitbucket.org/yrain/c-mergesort
Last updated: 2014-03-02
(c) by Yuri Astrov
*/
#include <stdio.h>
#include <stdlib.h>

#define MAX_RECURSION_LIMIT 1000
#define RESERVE_SORT error_sort //When recursion deph limit

#if defined(_WIN32) || defined(__WIN32__ )
    #define SSIZET_FMT "%u"
#elif defined(_WIN64) || defined(__WIN64__)
    #define SSIZET_FMT "%Iu"
//#elif defined(__linux) || defined(linux)
//    #define SSIZET_FMT "%u"
#else
    #define SSIZET_FMT "%u"
#endif

void merge_sort(int arr[], size_t low, size_t mid, size_t high, int buffer[]);
void partition(int arr[], size_t low, size_t high, int buffer[]);
void main_merge_sort(int arr[], size_t low, size_t high);
void error_sort(int arr[], size_t low, size_t high){
    fprintf(stderr, "Exception: Maximal recursion depth!");
    free(arr);
    exit(1);
}

int main(){
    size_t i, n;
	int *a;

    printf("Enter the total number of elements: ");
    scanf(SSIZET_FMT, &n);
	a = (int*)calloc(n, sizeof(int));
	if (a == NULL){
         fprintf(stderr, "MemoryException: Can't allocate " SSIZET_FMT " byte of memory.", n*sizeof(int));
         exit(1);
	}
    printf("Enter the elements which to be sort:\n");
    for(i=0; i<n; ++i){
         scanf("%d", &a[i]);
    }

    main_merge_sort(a, 0, n-1);

    printf("After merge sorting, elements are:\n");
    for(i=0; i < n; ++i){
         printf("%d ", a[i]);
    }
    printf("\n");
	free(a);
    return 0;
}

/*
Main MergeSort function.
arr - array
low - index of first element for sort in array
high - index of last element for sort in array
*/
void main_merge_sort(int arr[], size_t low, size_t high)
{
	int *buffer = (int*)calloc(high+1, sizeof(int));
	if (buffer == NULL){
		fprintf(stderr, "MemoryException: Can't allocate " SSIZET_FMT " byte of memory.", (high+1)*sizeof(int));
        free(arr);
		exit(1);
	}
	partition(arr, low, high, buffer);
	if(buffer) free(buffer);
}

/*
Partition function for MergeSort algorythm.
arr - array
low - index of first element for sort in array
high - index of last element for sort in array
buffer - temporary array, sizeof(arr) must be equal for sizeof(buffer)
*/
void partition(int arr[], size_t low, size_t high, int buffer[]){
    size_t mid;
	static size_t rec = 0;
    if(low < high){
         mid = low + (high-low)/2; // Naive realization
		++rec;
 		 if(rec == MAX_RECURSION_LIMIT){
			 RESERVE_SORT(arr, low, high);
			 free(buffer);
			 return;
		 }
         partition(arr, low, mid, buffer);
         partition(arr, mid+1, high, buffer);
		--rec;
         merge_sort(arr, low, mid, high, buffer);
    }
}

/*
Merge sort function for MergeSort algorythm.
arr - array
low - index of first element for sort in array
high - index of last element for sort in array
buffer - temporary array, sizeof(arr) must be equal for sizeof(buffer)
*/
void merge_sort(int arr[], size_t low, size_t mid, size_t high, int buffer[]){
	size_t i = 0;               /*Index for array*/
	size_t a_f_index = low;     /*Index in arr before middle*/
	size_t a_s_index = mid+1;   /*Index in arr after middle*/
	size_t b_index = low;       /*Index in buffer*/
	while((a_f_index <= mid) && (a_s_index <= high)){
		if(arr[a_f_index] < arr[a_s_index]) {
			buffer[b_index] = arr[a_f_index];
			++a_f_index;
		}
		else {
			buffer[b_index] = arr[a_s_index];
			++a_s_index;
		}
		b_index++;
	}
	if(a_f_index > mid) {
		for(i=a_s_index; i <= high; ++i) {
             buffer[b_index] = arr[i];
             ++b_index;
         }
	}
	else {
		for(i=a_f_index; i <= mid; ++i) {
            buffer[b_index] = arr[i];
            ++b_index;
         }
	}
	for(i=low; i <= high; ++i)
		arr[i] = buffer[i];
}

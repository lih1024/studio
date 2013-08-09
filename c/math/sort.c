#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void rand_array(int *array, int len, int min, int max);
void print_array(int *array, int len);
//sort
void select_sort(int *array, int len);
void bubble_sort(int *array, int len);
void insert_sort(int *array, int len);
void shell_sort(int *array, int len);
void merge_sort(int *array, int len);
void quick_sort(int *array, int len);

int main(int argc, char* argv[])
{
  int len = 10;
  int* array = (int*)malloc(sizeof(int)*len);
  memset(array, 0, sizeof(int)*len);
  srand((int)time(0));

  //select sort
  rand_array(array, len, 0, 100);
  printf("select sort before:\n");
  print_array(array, len);
  select_sort(array, len);
  printf("select sort after:\n");
  print_array(array, len);

  //bubble sort
  rand_array(array, len, 0, 100);
  printf("bubble sort before:\n");
  print_array(array, len);
  bubble_sort(array, len);
  printf("bubble sort after:\n");
  print_array(array, len);

  //insert sort
  rand_array(array, len, 0, 100);
  printf("insert sort before:\n");
  print_array(array, len);
  insert_sort(array, len);
  printf("insert sort after:\n");
  print_array(array, len);

  //shell sort
  rand_array(array, len, 0, 100);
  printf("shell sort before:\n");
  print_array(array, len);
  shell_sort(array, len);
  printf("shell sort after:\n");
  print_array(array, len);

  //merge sort
  rand_array(array, len, 0, 100);
  printf("merge sort before:\n");
  print_array(array, len);
  merge_sort(array, len);
  printf("merge sort after:\n");
  print_array(array, len);

  //quick sort
  rand_array(array, len, 0, 100);
  printf("quick sort before:\n");
  print_array(array, len);
  quick_sort(array, len);
  printf("quick sort after:\n");
  print_array(array, len);

  free(array);
  return 0;
}

void rand_array(int *array, int len, int min, int max)
{
  if(min >= max)
    return;
  int range = max - min + 1;
  int i;
  for(i=0; i < len; i++) {
    array[i] = rand() % range + min;
  }
}

void print_array(int *array, int len)
{
  if(array == NULL)
    return ;
  int i;
  for(i=0; i < len; i++) {
    printf("%d, ", array[i]);
  }
  printf("\n");
}

void swap(int *a, int *b)
{
#if 0
  int tmp = *a;
  *a = *b;
  *b = tmp;
#else
  *a = *a ^ *b;
  *b = *a ^ *b;
  *a = *a ^ *b;
#endif
}

void select_sort(int *array, int len)
{
  int i, j, k, tmp;
  for(i = 0; i < len - 1; i++) {
    k = i;
    for(j = i + 1; j < len; j++) {
      if(array[j] < array[k]) {
	k = j;
      }
    }
    if(i != k) {
      swap(&(array[i]), &(array[k]));
    }
  }
}

void bubble_sort(int *array, int len)
{
  int i, j;
  for(i = 0; i < len - 1; i++) {
    for(j = 0; j < len - 1 - i; j++) {
      if(array[j] > array[j+1]) {
	swap(&(array[j]), &(array[j+1]));
      }
    }
  }
}

void insert_sort(int *array, int len)
{
  int i, j, tmp;
  for(i = 1; i < len; i++) {
    tmp = array[i];
    for(j = i - 1; j >= 0; j--) {
      if(array[j] <= tmp)
	break;
      array[j+1] = array[j];
    }
    array[++j] = tmp;
  }
}

void shell_sort(int *array, int len)
{
  int i, j, t, s;
  s = len / 2;
  while(s > 0) {
    for(i = s; i < len; i++) {
      t = array[i];
      for(j = i - s; j >= 0; j -= s) {
	if(array[j] <= t)
	  break;
	array[j + s] = array[j];
      }
      array[j + s] = t;
    }
    s /= 2;
  }
}

void merge_sort_merge(int *array, int start, int middle, int end)
{
  int left_len = middle - start + 1,
    right_len = end - middle;
  int *left = (int*)malloc(sizeof(int)*left_len),
    *right = (int*)malloc(sizeof(int)*right_len);
  int l, r;
  l = 0;
  while(l < left_len) {
    left[l] = array[start + l];
    l++;
  }
  r = 0;
  while(r < right_len) {
    right[r] = array[middle + r + 1];
    r++;
  }
  l = 0;
  r = 0;
  while(l < left_len && r < right_len) {
    if(left[l] <= right[r]) {
      array[start++] = left[l++];
    } else {
      array[start++] = right[r++];
    }
  }
  while(l < left_len)
    array[start++] = left[l++];
  while(r < right_len)
    array[start++] = right[r++];
  free(left);
  free(right);
}

void merge_sort_entry(int *array, int start, int end)
{
  if(end > start) {
    int middle = (start + end) / 2;
    merge_sort_entry(array, start, middle);
    merge_sort_entry(array, middle + 1, end);
    merge_sort_merge(array, start, middle, end);
  }
}

void merge_sort(int *array, int len)
{
  merge_sort_entry(array, 0, len - 1);
}

void quick_sort_entry(int *array, int start, int end)
{
#if 0
  if(end > start) {
    int pivot = array[start];
    int i = start, j = end;
    while(i < j) {
      while(i < j) {
	if(array[j] < pivot) {
	  swap(&(array[i]), &(array[j]));
	  i++;
	  break;
	}
	j--;
      }
      while(i < j) {
	if(array[i] > pivot) {
	  swap(&(array[i]), &(array[j]));
	  j--;
	  break;
	}
	i++;
      }
    }
    quick_sort_entry(array, start, i);
    quick_sort_entry(array, j + 1, end);
  }
#else
  if(end > start) {
    int p = (start + end) / 2;
    int i = start, j = end;
    int key = array[p];
    while(i < j) {
      while(i < p) {
	if(array[i] > key) {
	  array[p] = array[i];
	  p = i;
	  break;
	}
	i++;
      }
      while(j > p) {
	if(array[j] < key) {
	  array[p] = array[j];
	  p = j;
	  break;
	}
	j--;
      }
    }
    array[p] = key;
    quick_sort_entry(array, start, p - 1);
    quick_sort_entry(array, p + 1, end);
  }
#endif
}

void quick_sort(int *array, int len)
{
  quick_sort_entry(array, 0, len - 1);
}


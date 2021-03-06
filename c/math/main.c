#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ds.h"

void print_list(list_t *list)
{
  list_node_t *curr = list->head;
  int *ip;
  while(curr != NULL) {
    ip = (int*)curr->data;
    if(ip != NULL) {
      printf("%d, ", *ip);
    } else {
      printf("NULL, ");
    }
    curr = curr->next;
  }
  printf("\n");
}

void print_stack(stack *s)
{
  printf("stack(%d):\n", stack_size(s));
  int *dp = NULL;
  while(stack_size(s) > 0) {
    stack_pop(s, (void**)&dp);
    if(dp != NULL)
      printf("%d, ", *dp);
    else
      printf("NULL, ");
  }
  printf("\n");
}

void test_stack()
{
  stack s;
  stack_init(&s, NULL);
  print_stack(&s);
  int a = 1, b = 2, c = 3;
  stack_push(&s, &a);
  stack_push(&s, &b);
  stack_push(&s, &c);
  print_stack(&s);
  int *dp = NULL;
  stack_pop(&s, NULL);
  print_stack(&s);
  //  printf("%d\n", *dp);
}

void print_queue(queue *q)
{
  printf("queue(%d):\n", queue_size(q));
  while(queue_size(q) > 0) {
    int *dp = NULL;
    queue_dequeue(q, (void**)&dp);
    printf("%d, ", *dp);
  }
  printf("\n");
}

void test_queue()
{
  queue q;
  queue_init(&q, NULL);
  print_queue(&q);

  int a = 1, b = 2, c = 3;
  queue_enqueue(&q, &a);
  queue_enqueue(&q, &b);
  queue_enqueue(&q, &c);
  print_queue(&q);

  queue_destroy(&q);
}

int ht_hash_callback(void *data)
{
  int *ip = (int*)data;
  
  int h = *ip;
  h = h & 0x7fffffff;
  printf("%x\n", h);
  return h;
}

int ht_match_callback(void *dpa, void *dpb)
{
  int* dpi = (int*)dpa;
  int* dpj = (int*)dpb;
  if(*dpi == *dpj) {
    return 0;
  }
  return -1;
}

void ht_destroy_callback(void *data)
{
  int *dp = (int*)data;
  free(dp);
}

void print_hash_table(hash_table_t *table)
{
  int i;
  printf("table:\n");
  for(i = 0; i < table->buckets; i++) {
    printf("bucket %d: ", i);
    print_list(&((table->table)[i]));
  }
}

void test_table()
{
  hash_table_t *table = (hash_table_t*)malloc(sizeof(hash_table_t));
  hash_table_init(table, 10, ht_hash_callback, ht_match_callback, ht_destroy_callback);
  int i, *a;
  for(i = 0; i < 10; i++) {
    int *ip = (int*)malloc(sizeof(int));
    *ip = i+1;
    if(i == 5)
      a = ip;
    hash_table_insert(table, (void*)ip);
  }
  print_hash_table(table);

  hash_table_remove(table, (void*)(a));
  print_hash_table(table);

  hash_table_destroy(table);
  free(table);
}

void print_tab(int n)
{
  int i;
  for(i = 0; i < n; i++) {
    printf("\t");
  }
}

void print_tree_node_inorder(bitree_node_t *node, int nt)
{
  if(node == NULL)
    return ;
  print_tab(nt);
  int *dp = (int*)(node->data);
  if(dp != NULL) {
    printf("%d\n", *dp);
  } else {
    printf("NULL\n");
  }
  if(node->left != NULL)
    print_tree_node_inorder(node->left, nt+1);
  if(node->right != NULL)
    print_tree_node_inorder(node->right, nt+1);
}

void print_tree(bitree_t *tree)
{
  print_tree_node_inorder(tree->root, 0);
}

int bt_compare_callback(void *key1, void *key2)
{
  return 0;
}

void bt_destroy_callback(void *data)
{
  //  int *dp = (int*)data;
  //  free(dp);
}

void test_tree()
{
  bitree_t *tree = (bitree_t*)malloc(sizeof(bitree_t));
  bitree_init(tree, bt_compare_callback, bt_destroy_callback);
  int a = 1, b = 2, c = 3, d = 4, e = 5;
  bitree_ins_left(tree, NULL, &a);
  bitree_node_t *root = bitree_root(tree);
  bitree_ins_left(tree, root, &b);
  bitree_ins_right(tree, root, &c);
  bitree_node_t *left = root->left;
  bitree_ins_left(tree, left, &d);
  bitree_node_t *right = root->right;
  bitree_ins_right(tree, right, &e);
  print_tree(tree);
  bitree_destroy(tree);
}

void print_bistree_node_inorder(bitree_node_t *node, int nt)
{
  if(node == NULL)
    return ;
  print_tab(nt);
  int *dp = (int*)(((avl_node_t*)(node->data))->data);
  if(dp != NULL) {
    printf("%d\n", *dp);
  } else {
    printf("NULL\n");
  }
  if(node->left != NULL)
    print_bistree_node_inorder(node->left, nt+1);
  if(node->right != NULL)
    print_bistree_node_inorder(node->right, nt+1);
}

void print_bistree(bitree_t *tree)
{
  printf("==========tree================\n");
  print_bistree_node_inorder(tree->root, 0);
}

int bist_compare_callback(void *key1, void *key2)
{
  int *kp1 = (int*)key1;
  int *kp2 = (int*)key2;
  return (*kp1 - *kp2);
}

void bist_destroy_callback(void *data)
{
  int *dp = (int*)data;
  free(dp);
}

void test_bistree()
{
  bistree_t *tree = (bistree_t*)malloc(sizeof(bistree_t));
  bistree_init(tree, bist_compare_callback, bist_destroy_callback);
  int i;
  for(i=10; i > 0; i--) {
    int *ip = (int*)malloc(sizeof(int));
    *ip = i;
    bistree_insert(tree, ip);
    print_bistree(tree);
  }

  int a = 1;
  printf("find %d: %d\n", a, bistree_lookup(tree, &a));

  a = 11;
  printf("find %d: %d\n", a, bistree_lookup(tree, &a));

  /*
  int *ap = (int*)malloc(sizeof(int));
  *ap = 1;
  bistree_insert(tree, ap);
  print_bistree(tree);
  int *bp = (int*)malloc(sizeof(int));
  *bp = 2;
  bistree_insert(tree, bp);
  print_bistree(tree);
  int *cp = (int*)malloc(sizeof(int));
  *cp = 3;
  bistree_insert(tree, cp);
  print_bistree(tree);
  int *dp = (int*)malloc(sizeof(int));
  *dp = 4;
  bistree_insert(tree, dp);
  print_bistree(tree);
  */
  bistree_destroy(tree);
}

int heap_compare_callback(void *k1, void *k2)
{
  int *ip1 = (int*)k1;
  int *ip2 = (int*)k2;
  return (*ip1 - *ip2);
}

void print_heap(heap_t *heap)
{
  int size = heap_size(heap);
  printf("heap(%d):\n", size);
  int i;
  int *ip;
  for(i = 0; i < size; i++) {
    ip = (int*)((heap->tree)[i]);
    printf("%d, ", *ip);
  }
  printf("\n");
}

void test_heap()
{
  heap_t *heap = (heap_t*)malloc(sizeof(heap_t));
  heap_init(heap, heap_compare_callback, NULL);
  srand((int)time(0));
  int i, len = 10;
  int *array = (int*)malloc(sizeof(int)*len);
  printf("random array:\n");
  for(i = 0; i < len; i++) {
    array[i] = rand() % 20;
    printf("%d, ", array[i]);
    heap_insert(heap, (void*)(&(array[i])));
  }
  printf("\n");

  print_heap(heap);

  heap_destroy(heap);
}

int main(int argc, char* argv[])
{
  /*
  test_stack();

  test_queue();

  test_table();
 
  test_tree();

  test_bistree();

  test_heap();
 */

  return 0;
}

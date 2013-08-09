#include "ds.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//========================================list==========================================
int list_init(list_t *list, void (*destroy)(void *))
{
  if(list == NULL)
    return -1;
  list->size = 0;
  list->destroy = destroy;
  list->head = NULL;
  list->tail = NULL;
}

int list_ins_next(list_t *list, list_node_t *node, void *data)
{
  if(list == NULL)
    return -1;
  list_node_t *new_node = (list_node_t*)malloc(sizeof(list_node_t));
  if(new_node == NULL)
    return -1;
  new_node->data = data;
  if(node == NULL) {//insert to head
    new_node->next = list->head;
    list->head = new_node;
  } else {
    new_node->next = node->next;
    node->next = new_node;
  }
  if(new_node->next == NULL) {
    list->tail = new_node;
  }
  list->size += 1;
  return 0;
}

int list_rem_next(list_t *list, list_node_t *node, void **data)
{
  if(list == NULL)
    return -1;
  if(node == NULL) {//remove head
    if(list->head != NULL) {
      list_node_t *head = list->head;
      if(data != NULL)
	*data = head->data;
      list->head = head->next;
      if(list->head == NULL) {
	list->tail = NULL;
      }
      list->size -= 1;
      return 0;
    }
  } else {
    list_node_t *next_node = node->next;
    if(next_node != NULL) {
      if(data != NULL)
	*data = next_node->data;
      node->next = next_node->next;
      free(next_node);
      if(node->next == NULL) {
	list->tail = NULL;
      }
      list->size -= 1;
      return 0;
    }
  }
  return -1;
}

int list_head(list_t *list, void **data)
{
  if(list == NULL || data == NULL)
    return -1;
  list_node_t *head = list->head;
  if(head == NULL)
    return -1;
  *data = head->data;
  return 0;
}

int list_tail(list_t *list, void **data)
{
  if(list == NULL || data == NULL)
    return -1;
  list_node_t *tail = list->tail;
  if(tail == NULL)
    return -1;
  *data = tail->data;
  return 0;
}

int list_destroy(list_t *list)
{
  if(list == NULL)
    return -1;
  list_node_t *node = list->head, *tmp;
  while(node != NULL) {
    if(list->destroy != NULL) {
      (list->destroy)(node->data);
    }
    tmp = node->next;
    free(node);
    node = tmp;
  }
  return 0;
}

//==========================================hash table====================================
int hash_table_init(hash_table_t *table,
		    int buckets,
		    int (*hash)(void*),
		    int (*match)(void*, void*),
		    void (*destroy)(void*))
{
  table->buckets = buckets;
  table->size = 0;
  table->table = (list_t*)malloc(sizeof(list_t)*buckets);
  int i;
  for(i = 0; i < buckets; i++)
    list_init(&((table->table)[i]), destroy);
  table->hash = hash;
  table->match = match;
}

int hash_table_insert(hash_table_t *table, void *data)
{
  if(0 == hash_table_lookup(table, data))
    return -1;
  int index = (table->hash)(data) % table->buckets;
  int ret = list_ins_next(&((table->table)[index]), NULL, data);
  if(ret == 0) {
    (table->size)++;
  }
  return ret;
}

int hash_table_remove(hash_table_t *table, void *data)
{
  int index = (table->hash)(data) % table->buckets;
  list_node_t *prev = NULL, *current = ((table->table)[index]).head;
  while(current != NULL) {
    if((table->match)(current->data, data) == 0) {
      list_rem_next(&((table->table)[index]), prev, NULL);
      (table->size)--;
      return 0;
    }
    prev = current;
    current = current->next;
  }
  return -1;
}

int hash_table_lookup(hash_table_t *table, void *data)
{
  int index = (table->hash)(data) % table->buckets;
  list_node_t *prev = NULL, *current = ((table->table)[index]).head;
  while(current != NULL) {
    if((table->match)(current->data, data) == 0) {
      return 0;
    }
    prev = current;
    current = current->next;
  }
  return -1;
}

int hash_table_destroy(hash_table_t *table)
{
  int i;
  for(i = 0; i < table->buckets; i++) {
    list_destroy(&((table->table)[i]));
  }
}

//===================================bitree====================================
void bitree_init(bitree_t *tree, 
		int (*compare)(void*, void*),
		void (*destroy)(void*))
{
  tree->size = 0;
  tree->root = NULL;
  tree->compare = compare;
  tree->destroy = destroy;
}

int bitree_ins_left(bitree_t *tree, bitree_node_t *node, void *data)
{
  if(node == NULL) {//as root
    if(tree->root != NULL)
      return -1;
    bitree_node_t *new_node = (bitree_node_t*)malloc(sizeof(bitree_node_t));
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    tree->root = new_node;
  } else {
    if(node->left != NULL)
      return -1;
    bitree_node_t *new_node = (bitree_node_t*)malloc(sizeof(bitree_node_t));
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    node->left = new_node;
  }
  return 0;
}

int bitree_ins_right(bitree_t *tree, bitree_node_t *node, void *data)
{
  if(node == NULL) {//as root
    if(tree->root != NULL)
      return -1;
    bitree_node_t *new_node = (bitree_node_t*)malloc(sizeof(bitree_node_t));
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    tree->root = new_node;
  } else {
    if(node->right != NULL)
      return -1;
    bitree_node_t *new_node = (bitree_node_t*)malloc(sizeof(bitree_node_t));
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    node->right = new_node;
  }
  return 0;
}

void bitree_destroy_node(bitree_t *tree, bitree_node_t *node)
{
  if(node != NULL) {
    if(node->left != NULL)
      bitree_destroy_node(tree, node->left);
    if(node->right != NULL)
      bitree_destroy_node(tree, node->right);
    if(node->data != NULL && tree->destroy != NULL)
      (tree->destroy)(node->data);
    free(node);
  }
}

void bitree_rem_left(bitree_t *tree, bitree_node_t *node)
{
  if(node == NULL) {
    bitree_destroy_node(tree, tree->root);
  } else {
    bitree_destroy_node(tree, node->left);
  }
}

void bitree_rem_right(bitree_t *tree, bitree_node_t *node)
{
  if(node == NULL) {
    bitree_destroy_node(tree, tree->root);
  } else {
    bitree_destroy_node(tree, node->right);
  }
}

void bitree_merge(bitree_t *merge, bitree_t *left, bitree_t *right, void *data)
{
  merge->size = left->size + right->size;
  merge->root = (bitree_node_t*)malloc(sizeof(bitree_node_t));
  (merge->root)->data = data;
  (merge->root)->left = left->root;
  (merge->root)->right = right->root;
  merge->compare = left->compare;
  merge->destroy = left->destroy;
  free(left);
  free(right);
}

void bitree_destroy(bitree_t *tree)
{
  bitree_destroy_node(tree, tree->root);
}

//==============================binary search tree============================
typedef avl_node {
  void *data;
  int factor;
} avl_node_t;

enum AVL_TYPE {
  AVL_BALANCE = 0,
  AVL_LEFT_HEAVY = 1,
  AVL_RIGHT_HEAVY = -1
};

void rotate_ll(bitree_node_t *grandpa)
int bistree_balance_insert(bitree_t *tree, bitree_node_t *grandpa, bitree_node_t *parent, avl_node_t *ins_node)
{
  int comp = (tree->compare)(ins_node->data, parent->data);
  if(comp == 0) {
    return -1;
  } else if(comp == 1) {
    if(bitree_is_eob(parent->right)) {
      if(grandpa == NULL) {//balance = 0
	parent->right = ins_node;
	avl_node_t *an = (avl_node_t*)bitree(data);
	an->factor += 1;
	return 0;
      } else {
	
      }
    }
  } else if(comp == -1) {
    
  } else {
    return -1;
  }
}

int bistree_insert(bistree_t *tree, void *data)
{
  if(data == NULL)
    return -1;
  avl_node_t *node = (avl_node_t*)malloc(sizeof(avl_node_t));
  node->data = data;
  node->factor = 0;
  bitree_node_t *root = tree->root;
  if(root == NULL) {
    bitree_node_t *root_node = (bitree_node_t*)malloc(sizeof(bitree_node_t));
    root_node->data = (void*)node;
    tree->root = root_node;
    node = NULL;
  } else {
    int ret = bistree_balance_insert(tree, NULL, root, node);
    if(ret == 0)
      node = NULL;
  }
  if(node != NULL) {
    free(node);
    return -1;
  }
  return 0;
}

int bistree_remove(bistree_t *tree, void *data)
{

}

int bistree_lookup(bistree_t *tree, void *data)
{

}


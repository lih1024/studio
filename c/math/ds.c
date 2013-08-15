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

enum AVL_TYPE {
  AVL_BALANCE = 0,
  AVL_LEFT_HEAVY = 1,
  AVL_RIGHT_HEAVY = -1
};

avl_node_t* avl_node_create(void *data)
{
  avl_node_t *node_avl = (avl_node_t*)malloc(sizeof(avl_node_t));
  node_avl->data = data;
  node_avl->factor = AVL_BALANCE;
  return node_avl;
}

bitree_node_t* bitree_node_create(void *data)
{
  bitree_node_t *node = (bitree_node_t*)malloc(sizeof(bitree_node_t));
  node->data = (void*)avl_node_create(data);
  node->left = NULL;
  node->right = NULL;
}

void avl_rotate(bitree_node_t **root)
{
  int factor = ((avl_node_t*)((*root)->data))->factor;
  if(factor > AVL_LEFT_HEAVY) {
    bitree_node_t *left = (*root)->left;
    if(left != NULL) {
      factor = ((avl_node_t*)(left->data))->factor;
      if(factor == AVL_LEFT_HEAVY) {//LL rotate
	(*root)->left = left->right;
	((avl_node_t*)((*root)->data))->factor = AVL_BALANCE;
	left->right = *root;
	((avl_node_t*)(left->data))->factor = AVL_BALANCE;
	*root = left;
      } else if(factor == AVL_RIGHT_HEAVY) {//LR rotate
	bitree_node_t *leftright = left->right;
	factor = ((avl_node_t*)(leftright->data))->factor;
	if(factor == AVL_BALANCE) {
	  (*root)->left = leftright->right;
	  ((avl_node_t*)((*root)->data))->factor = AVL_BALANCE;
	  left->right = leftright->left;
	  ((avl_node_t*)(left->data))->factor = AVL_BALANCE;
	  leftright->left = left;
	  leftright->right = *root;
	  ((avl_node_t*)(leftright->data))->factor = AVL_BALANCE;
	  *root = leftright;
	} else if(factor == AVL_LEFT_HEAVY) {
	  (*root)->left = leftright->right;
	  ((avl_node_t*)((*root)->data))->factor = AVL_RIGHT_HEAVY;
	  left->right = leftright->left;
	  ((avl_node_t*)(left->data))->factor = AVL_BALANCE;
	  leftright->left = left;
	  leftright->right = *root;
	  ((avl_node_t*)(leftright->data))->factor = AVL_BALANCE;
	  *root = leftright;
	} else if(factor == AVL_RIGHT_HEAVY) {
	  (*root)->left = leftright->right;
	  ((avl_node_t*)((*root)->data))->factor = AVL_BALANCE;
	  left->right = leftright->left;
	  ((avl_node_t*)(left->data))->factor = AVL_LEFT_HEAVY;
	  leftright->left = left;
	  leftright->right = *root;
	  ((avl_node_t*)(leftright->data))->factor = AVL_BALANCE;
	  *root = leftright;
	}
      }
    }
  } else if(factor < AVL_RIGHT_HEAVY) {
    bitree_node_t *right = (*root)->right;
    if(right != NULL) {
      factor = ((avl_node_t*)(right->data))->factor;
      if(factor == AVL_RIGHT_HEAVY) {//RR rotate
	(*root)->right = right->left;
	((avl_node_t*)((*root)->data))->factor = AVL_BALANCE;
	right->left = *root;
	((avl_node_t*)(right->data))->factor = AVL_BALANCE;
	*root = right;
      } else if(factor == AVL_LEFT_HEAVY) {//RL rotate
	bitree_node_t *rightleft = right->left;
	factor = ((avl_node_t*)(rightleft->data))->factor;
	if(factor == AVL_BALANCE) {
	  (*root)->right = rightleft->left;
	  ((avl_node_t*)((*root)->data))->factor = AVL_BALANCE;
	  right->left = rightleft->right;
	  ((avl_node_t*)(right->data))->factor = AVL_BALANCE;
	  rightleft->right = right;
	  rightleft->left = *root;
	  ((avl_node_t*)(rightleft->data))->factor = AVL_BALANCE;
	  *root = rightleft;
	} else if(factor == AVL_LEFT_HEAVY) {
	  (*root)->right = rightleft->left;
	  ((avl_node_t*)((*root)->data))->factor = AVL_BALANCE;
	  right->left = rightleft->right;
	  ((avl_node_t*)(right->data))->factor = AVL_RIGHT_HEAVY;
	  rightleft->right = right;
	  rightleft->left = *root;
	  ((avl_node_t*)(rightleft->data))->factor = AVL_BALANCE;
	  *root = rightleft;
	} else if(factor == AVL_RIGHT_HEAVY) {
	  (*root)->right = rightleft->left;
	  ((avl_node_t*)((*root)->data))->factor = AVL_LEFT_HEAVY;
	  right->left = rightleft->right;
	  ((avl_node_t*)(right->data))->factor = AVL_BALANCE;
	  rightleft->right = right;
	  rightleft->left = *root;
	  ((avl_node_t*)(rightleft->data))->factor = AVL_BALANCE;
	  *root = rightleft;
	}
      }
    }
  }
}

int bistree_balance_insert(bitree_t *tree, bitree_node_t **parent, void *data, int *balance)
{
  int comp = (tree->compare)(data, (*parent)->data);
  if(comp == 0) {
    return -1;
  } else if(comp > 0) {
    if((*parent)->right == NULL) {
      (*parent)->right = bitree_node_create(data);
      ((avl_node_t*)((*parent)->data))->factor += AVL_RIGHT_HEAVY;
      if(((avl_node_t*)((*parent)->data))->factor == AVL_RIGHT_HEAVY)
	*balance = AVL_RIGHT_HEAVY;
      else
	*balance = AVL_BALANCE;
      return 0;
    } else {
      int ret = bistree_balance_insert(tree, &((*parent)->right), data, balance);
      if(ret == 0 && *balance != AVL_BALANCE) {
	((avl_node_t*)((*parent)->data))->factor += AVL_RIGHT_HEAVY;
	if(((avl_node_t*)((*parent)->data))->factor < AVL_RIGHT_HEAVY) {
	  avl_rotate(parent);
	  *balance = AVL_BALANCE;
	} else if(((avl_node_t*)((*parent)->data))->factor == AVL_RIGHT_HEAVY) {
	  *balance = AVL_RIGHT_HEAVY;
	} else if(((avl_node_t*)((*parent)->data))->factor == AVL_BALANCE) {
	  *balance = AVL_BALANCE;
	}
      }
      return ret;
    }
  } else {
    if((*parent)->left == NULL) {
      (*parent)->left = bitree_node_create(data);
      ((avl_node_t*)((*parent)->data))->factor += AVL_LEFT_HEAVY;
      if(((avl_node_t*)((*parent)->data))->factor == AVL_LEFT_HEAVY)
	*balance = AVL_LEFT_HEAVY;
      else
	*balance = AVL_BALANCE;
      return 0;
    } else {
      int ret = bistree_balance_insert(tree, &((*parent)->left), data, balance);
      if(ret == 0 && *balance != AVL_BALANCE) {
	((avl_node_t*)((*parent)->data))->factor += AVL_LEFT_HEAVY;
	if(((avl_node_t*)((*parent)->data))->factor > AVL_LEFT_HEAVY) {
	  avl_rotate(parent);
	  *balance = AVL_BALANCE;
	} else if(((avl_node_t*)((*parent)->data))->factor == AVL_LEFT_HEAVY) {
	  *balance = AVL_LEFT_HEAVY;
	} else if(((avl_node_t*)((*parent)->data))->factor > AVL_BALANCE) {
	  *balance = AVL_BALANCE;
	}
      }
      return ret;
    }
  }
}

int bistree_insert(bistree_t *tree, void *data)
{
  if(data == NULL)
    return -1;
  if(tree->root == NULL) {
    tree->root = bitree_node_create(data);
    return 0;
  } else {
    int balance;
    int ret = bistree_balance_insert(tree, &(tree->root), data, &balance);
    if(ret == 0 && balance != AVL_BALANCE) {
      /*
      ((avl_node_t*)(tree->root->data))->factor += AVL_LEFT_HEAVY;
      if(((avl_node_t*)(tree->root->data))->factor > AVL_LEFT_HEAVY ||
	 ((avl_node_t*)(tree->root->data))->factor < AVL_RIGHT_HEAVY) {
	avl_rotate(&(tree->root));
      }
      */
    }
  }
}

int bistree_remove(bistree_t *tree, void *data)
{

}

int bistree_lookup(bistree_t *tree, void *data)
{
  bitree_node_t *node = tree->root;
  int compval;
  while(node != NULL) {
    compval = tree->compare(data, ((avl_node_t*)(node->data))->data);
    if(compval == 0) {
      return 0;
    } else if(compval > 0) {
      node = node->right;
    } else {
      node = node->left;
    }
  }
  return -1;
}


#ifndef DS_H
#define DS_H

//#ifdef __cplusplus
//extern "C" {
//#endif

//=============================single-linked list========================================//
typedef struct list_node {
  void *data;
  struct list_node *next;
} list_node_t;

typedef struct list {
  int size;
  void (*destroy)(void *);
  list_node_t *head, *tail;
} list_t;

int list_init(list_t *list, void (*destroy)(void *));
int list_ins_next(list_t *list, list_node_t *node, void *data);
int list_rem_next(list_t *list, list_node_t *node, void **data);
int list_head(list_t *list, void **data);
int list_tail(list_t *list, void **data);
int list_destroy(list_t *list);
#define list_size(l) ((l)->size)

//========================================stack==============================================
typedef list_t stack;
#define stack_init(s, destroy) (list_init((s), (destroy)))
#define stack_push(s, dp) (list_ins_next((s), NULL, (dp)))
#define stack_pop(s, dpp) (list_rem_next((s), NULL, (dpp)))
#define stack_peep(s, dpp) (list_head((s), (dpp)))
#define stack_size(s) (list_size((s)))
#define stack_destroy(s) (list_destroy((s)))

//=======================================queue==============================================
typedef list_t queue;
#define queue_init(q, destroy) (list_init((q), (destroy)))
#define queue_enqueue(q, dp) (list_ins_next((q), (q)->tail, (dp)))
#define queue_dequeue(q, dpp) (list_rem_next((q), NULL, (dpp)))
#define queue_peek(q, dpp) (list_head((q), (dpp)))
#define queue_size(q) (list_size((q)))
#define queue_destroy(q) (list_destroy((q)))

//=====================================hash table===========================================
typedef struct hash_table {
  int (*hash)(void*);
  int (*match)(void*, void*);
  int buckets;
  int size;
  list_t *table;
} hash_table_t;

int hash_table_init(hash_table_t *table,
		    int buckets,
		    int (*hash)(void*),
		    int (*match)(void*, void*),
		    void (*destroy)(void*));
int hash_table_insert(hash_table_t *table, void *data);
int hash_table_remove(hash_table_t *table, void *data);
int hash_table_lookup(hash_table_t *table, void *data);
#define hash_table_size(table) ((table)->size)
int hash_table_destroy(hash_table_t *table);

//===================================binary tree============================================
typedef struct bitree_node {
  void *data;
  struct bitree_node *left, *right;
} bitree_node_t;

typedef struct bitree {
  int (*compare)(void *key1, void *key2);
  void (*destroy)(void*);
  int size;
  bitree_node_t *root;
} bitree_t;

void bitree_init(bitree_t *tree,
		int (*compare)(void*, void*),
		void (*destroy)(void*));
int bitree_ins_left(bitree_t *tree, bitree_node_t *node, void *data);
int bitree_ins_right(bitree_t *tree, bitree_node_t *node, void *data);
void bitree_rem_left(bitree_t *tree, bitree_node_t *node);
void bitree_rem_right(bitree_t *tree, bitree_node_t *node);
void bitree_merge(bitree_t *merge, bitree_t *left, bitree_t *right, void *data);
#define bitree_root(tree) ((tree)->root)
#define bitree_left(node) ((node)->left)
#define bitree_right(node) ((node)->right)
#define bitree_data(node) ((node)->data)
#define bitree_size(tree) ((tree)->size)
#define bitree_is_eob(node) ((node) == NULL)
#define bitree_is_leaf(node) (((node->left) == NULL) && ((node->right) == NULL))
void bitree_destroy(bitree_t *tree);

//==============================binary search tree============================
typedef bitree_t bistree_t;
#define bistree_init(tree, compapre, destroy) (bitree_init(tree, compare, destroy))
int bistree_insert(bistree_t *tree, void *data);
int bistree_remove(bistree_t *tree, void *data);
int bistree_lookup(bistree_t *tree, void *data);
#define bistree_destroy(tree) (bitree_destroy(tree))

//#ifdef __cplusplus
//}
//#endif

#endif

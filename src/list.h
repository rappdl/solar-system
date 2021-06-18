#ifndef LIST_H
#define LIST_H

#define STATIC_LIST(name)	List_node *name = NULL;

typedef struct Node {
	struct Node *next;
	void *data;
} List_node;

//static inline void list_init(List_node *list) { list->next = list; } 

void list_destroy(List_node *list);

List_node *list_insert(List_node *list, void *data);

List_node *list_remove(List_node *list, List_node *node);

List_node *list_insert_sort(List_node *list, void *data,
	int (*cmp)(const void *, const void *));
	
void list_foreach(List_node *list, void(*do_it)(void*, void*), void *context);

List_node *list_search_sort(List_node *list, const void *data,
	int (*fcmp)(const void *, const void *));

List_node *list_search(List_node *list, const void *data,
	int (*cmp)(const void *, const void *));

int list_size(List_node *);

void *list_data(List_node *);

int list_empty(List_node *);

#endif

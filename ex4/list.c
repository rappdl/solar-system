#include <stdlib.h>
#include "list.h"

void list_destroy(List_node *list) {
	for (List_node *next, *p = list; p != NULL; p = next) {
		next = p->next;
		free(p);
	}
}

List_node *list_insert_sort(List_node *list, void *data,
	int (*fcmp)(const void *, const void *)) {
	List_node *node = malloc(sizeof (*node));
	if (NULL == node)
		return NULL;
	node->data = data;
	node->next = NULL;
	if (list == NULL)						/*	Primeiro elemento */
		;
	else if (fcmp(list->data, data) <= 0) {	/* Inserção antes do primeiro elemento */
		node->next = list;
	}
	else {
		List_node *prev = list;		/* Primeiro elemento já verificado */
		List_node *p;
		for (p = prev->next; p != NULL && fcmp(p->data, data) < 0; prev = p, p = p->next)
			;
		node->next = p;
		prev->next = node;
		return list;
	}
	return node;
}

List_node *list_insert(List_node *list, void *data) {
	List_node *node = malloc(sizeof (*node));
	if (NULL == node)
		return 0;
	node->data = data;
	node->next = list;
	return node;
}

List_node *list_remove(List_node *list, List_node *node) {
	if (node == NULL || list == NULL)
		return list;
	if (list == node) {
		list = node->next;
		free(node);			/* Remover o primeiro elemento */
	}
	else {
		List_node *prev = list;
		for (List_node *p = prev->next; p != NULL; prev = p, p = p->next)
			if (p == node) {
				prev->next = node->next;
				free(node);
			}
	}
	return list;
}

void list_foreach(List_node *list, void(*do_it)(void *, void *), void *context) {
	for (List_node * p = list; p != NULL; p = p->next)
		do_it(p->data, context);
}

List_node *list_search_sort(List_node *list, const void *data,
							int (*fcmp)(const void *, const void *)) {
	for (List_node *p = list; p != NULL; p = p->next) {
		int tmp = fcmp(p->data, data);
		if (tmp == 0)
			return p;
		else if (tmp > 0)
			return NULL;
	}
	return NULL;
}


List_node *list_search(List_node *list, const void *data,
	int (*fcmp)(const void *, const void *)) {
	for (List_node *p = list; p != NULL; p = p->next)
		if (fcmp(p->data, data) == 0)
			return p;
	return NULL;
}

int list_size(List_node *list) {
	int n = 0;
	for (List_node *p = list; p != NULL; p = p->next, ++n)
		;
	return n;
}

void *list_data(List_node *node) {
	return node->data;
}

int list_empty(List_node *list) {
	return NULL == list;
}

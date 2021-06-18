/*	Instituto Superior de Engenharia de Lisboa
	Ezequiel Conde, 2010

	As listas de colisão têm sentinela na própria tabela
*/
#ifndef HTABLE_H
#define HTABLE_H

#include <stdlib.h>
#include "list.h"


typedef struct Htable {
	List_node **base;
	size_t size;
	int (*hash_function)(const void *);
	int (*key_function)(const void *, const void *);
} Htable;

#define STATIC_HTABLE(name, size, hash_function) \
	static List_node *name##links[size]; \
	static Htable name = { name##links, size, hash_function }

void htable_init(Htable *, size_t,
				int (*hash_function)(const void *),
				int (*key_function)(const void *, const void *));

Htable *htable_create(size_t,
				int (*hash_function)(const void *),
				int (*key_function)(const void *, const void *));

void htable_destroy(Htable *);


int htable_insert(Htable *table, void *data);

void htable_remove(Htable *htable, const void *key);
	
void *htable_search(Htable * htable, const void *key,
	int (*cmp)(const void *, const void *));

void *htable_lookup(Htable *htable, const void *key);

void htable_foreach(Htable *, void (*do_it)(void*, void*), void *context);

size_t htable_size(Htable *table);

int htable_collisions(Htable *table);

#endif

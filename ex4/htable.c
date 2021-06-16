/*	Instituto Superior de Engenharia de Lisboa
	Ezequiel Conde, 2010
*/
#include "htable.h"

void htable_init(Htable * htable, size_t size,
				int (*hf)(const void *),
				int (*kf)(const void *, const void *)) {
	htable->hash_function = hf;
	htable->key_function = kf;
	htable->size = size;
	for (size_t i = 0; i < size; ++i)
		htable->base[i] = NULL;
}

Htable *htable_create(size_t size,
				int (*hf)(const void *),
				int (*kf)(const void *, const void *)) {
	Htable *htable = malloc(sizeof(Htable));
	if (NULL == htable)
		return NULL;
	htable->base = malloc(sizeof *htable->base * size);
	if (NULL == htable->base){
		free(htable);
		return NULL;
	}
	htable_init(htable, size, hf, kf);
	return htable;
}

void htable_destroy(Htable *htable) {
	for (size_t i = 0; i < htable->size; ++i)
		if (htable->base[i] != NULL)
			list_destroy(htable->base[i]);
	free(htable->base);
	free(htable);
}

int htable_insert(Htable *htable, void *data) {
	int index = htable->hash_function(data) % htable->size;
	htable->base[index] = list_insert(htable->base[index], data);
	return htable->base[index] != NULL;
}

void htable_remove(Htable *htable, const void *key) {
	int index = htable->hash_function(key) % htable->size;
	if (NULL == htable->base[index])
		return;
	List_node *node = list_search(htable->base[index],
								key, htable->key_function);
	if (NULL != node)
		htable->base[index] = list_remove(htable->base[index], node);
}

void *htable_lookup(Htable *htable, const void *key) {
	int index = htable->hash_function(key) % htable->size;
	if (NULL == htable->base[index])
		return NULL;
	List_node *node = list_search(htable->base[index],
									key, htable->key_function);
	if (node != NULL)
		return list_data(node);
	return NULL;
}

void *htable_search(Htable *htable, const void *key,
	int (*cmp)(const void *, const void *)) {
	int index = htable->hash_function(key) % htable->size;
	if (NULL == htable->base[index])
		return NULL;
	List_node *node = list_search(htable->base[index], key, cmp);
	if (node != NULL)
		return list_data(node);
	return NULL;
}

void htable_foreach(Htable *htable, void (*do_it)(void*, void*), void *context) { //, void*), void *context)
	for (size_t i = 0; i < htable->size; ++i)
		if (htable->base[i] != NULL)
			list_foreach(htable->base[i], do_it, context);
}

size_t htable_size(Htable *htable) {
	int i, counter = 0;
	for (i = 0; i < htable->size; ++i)
		if (NULL != htable->base[i])
			counter += list_size(htable->base[i]);
	return counter;
}

int htable_collisions(Htable * htable) {
	int i, max_collisions = 0;
	for (i = 0; i < htable->size; ++i) {
		if (NULL != htable->base[i]) {
			int collisions = list_size(htable->base[i]);
			if (collisions > max_collisions)
				max_collisions = collisions;
		}
	}
	return max_collisions - 1; 
}

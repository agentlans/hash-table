/*
    Copyright 2021 Alan Tseng

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdlib.h>

#include "vector.h"
#include "hashtable.h"

unsigned int_hash(hasht_key k) {
	int* p = (int*) k;
	return abs(*p);
}

int int_equal(hasht_key k1, hasht_key k2) {
	return *((int*) k1) == *((int*) k2);
}

hasht* hasht_new(struct hashtConfig* config, 
	unsigned (*hash)(hasht_key k), int (*equal)(hasht_key k1, hasht_key k2)) {

	hasht* h = malloc(sizeof(hasht));
	if (!h) return NULL;
	hasht_bucket* buckets = malloc(config->num_buckets * sizeof(hasht_bucket));
	if (!buckets) {
		free(h);
		return NULL;
	}
	/* Initialize each bucket */
	size_t init_cap = config->init_capacity;
	size_t delta_cap = config->delta_capacity;
	size_t num_buck = config->num_buckets;
	for (int i = 0; i < num_buck; ++i) {
		vect* keys = vect_new(init_cap, delta_cap);
		vect* values = vect_new(init_cap, delta_cap);
		if (!keys || !values) {
			hasht_free(h);
			return NULL;
		}
		buckets[i] = (hasht_bucket) {keys, values};
	}
	*h = (hasht) {buckets, config->num_buckets, hash, equal};
	return h;
}

void hasht_free(hasht* h) {
	size_t n = h->num_buckets;
	for (int i = 0; i < n; ++i) {
		hasht_bucket* b = h->buckets + i;
		vect_free(b->keys);
		vect_free(b->values);
	}
	free(h->buckets);
	free(h);
}

unsigned find_bucket(hasht* h, hasht_key k) {
	return h->hash(k) % h->num_buckets;
}

/* Find the index of the key in the bucket, or -1 if not found */
int find_index(hasht_bucket* b, hasht_key k, int (*equal)(hasht_key k1, hasht_key k2)) {
	int index = -1;
	int i = 0;
	hasht_key* p_end = b->keys->data + b->keys->size;
	for (hasht_key* p = b->keys->data; p < p_end; ++p) {
		if (!*p) continue; // Ignore NULL keys
		if (equal(*p, k)) {
			index = i;
			break;
		}
		i++;
	}
	return index;
}

int hasht_remove(hasht* h, hasht_key k) {
	hasht_bucket* b = h->buckets + find_bucket(h,k);
	int ind = find_index(b, k, h->equal);
	if (ind != -1) {
		b->keys->data[ind] = NULL;
		b->values->data[ind] = NULL;
		return 0;
	} else {
		// Nothing to erase
		return 1;
	}
}

int hasht_set(hasht* h, hasht_key k, hasht_value v) {
	hasht_bucket* b = h->buckets + find_bucket(h,k);
	int ind = find_index(b, k, h->equal);
	if (ind != -1) {
		vect_set(b->values, ind, v);
		return 0;
	} else {
		int err1 = vect_push_back(b->keys, k);
		int err2 = vect_push_back(b->values, v);
		return err1 || err2;
	}
}

hasht_value hasht_find(hasht* h, hasht_key k, hasht_value default_val) {
	hasht_bucket* b = h->buckets + find_bucket(h,k);
	int index = find_index(b, k, h->equal);
	if (index == -1) {
		return default_val;
	} else {
		return b->values->data[index];
	}
}

/* Moves the entries in the bucket so that there are no NULL places */
void compact_bucket(hasht_bucket* bucket) {
	hasht_key* i = bucket->keys->data;
	hasht_key* end = bucket->keys->data + bucket->keys->size;
	hasht_value* iv = bucket->values->data;

	/* Keep track of number of elements */
	int num_elem = 0;
	while (i < end) {
		while (i < end && *i) {
			i++;
			iv++;
			/* Guaranteed to count number of non-NULL elements 
			because i goes through each element exactly once. */
			num_elem++;
		}
		if (i == end) break; /* No NULLs in array */

		/* Start after i */
		hasht_key* j = i+1;
		hasht_value* jv = iv+1;
		while (j < end && !(*j)) {
			j++;
			jv++;
		}
		if (j == end) break; /* No non-NULLs after i. */

		/* Now we're sure that i is the first NULL,
			j is first non-NULL after i, 
			and i and j are in range */

		/* Move the keys from j to i. */
		*i = *j;
		*j = NULL;
		/* Move the values too */
		*iv = *jv;
		*jv = NULL;

		/* Repeat with next i until reached the end... */
	}
	/* When all entries are moved, shrink the bucket */
	// but don't shrink if size is 0
	vect_compact(bucket->keys, 0);
	vect_compact(bucket->values, 0);
}

void hasht_compact(hasht* h) {
	size_t n = h->num_buckets;
	for (int i = 0; i < n; ++i) {
		compact_bucket(h->buckets + i);
	}
}


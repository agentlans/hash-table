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

#ifndef _HASHT
#define _HASHT

#include "vector.h"

typedef void* hasht_key;
typedef void* hasht_value;

typedef struct hasht_bucket {
	vect* keys;
	vect* values;
} hasht_bucket;

typedef struct hasht {
	hasht_bucket* buckets;
	size_t num_buckets;
	unsigned (*hash)(hasht_key k);
	int (*equal)(hasht_key k1, hasht_key k2);
} hasht;

struct hashtConfig {
	size_t num_buckets;
	size_t init_capacity;
	size_t delta_capacity;
};

hasht* hasht_new(
	struct hashtConfig* config, 
	unsigned (*hash)(hasht_key k), 
	int (*equal)(hasht_key k1, hasht_key k2));

void hasht_free(hasht* h);

int hasht_set(hasht* h, hasht_key k, hasht_value v);
int hasht_remove(hasht* h, hasht_key k);
hasht_value hasht_find(hasht* h, hasht_key k, hasht_value default_val);

void hasht_compact(hasht* h);

#endif

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
#include <string.h>

#include "vector.h"

vect* vect_new(size_t capacity, size_t delta_capacity) {
	vect* v = malloc(sizeof(vect));
	if (!v) return NULL;
	v->data = malloc(capacity * sizeof(void*));
	if (!v->data) {
		free(v);
		return NULL;
	}
	v->size = 0;
	v->capacity = capacity;
	v->delta_capacity = delta_capacity;
	return v;
}

void vect_free(vect* v) {
	if (v) free(v->data);
	free(v);
}

void** vect_get(vect* v, int index) {
	return v->data + index;
}

void vect_set(vect* v, int index, void* x) {
	v->data[index] = x;
}

int vect_compact(vect* v, int free_if_zero) {
	// Call realloc when size is 0? Will free pointer.
	if (!free_if_zero && v->size == 0) return 0;
	void** p = realloc(v->data, v->size * sizeof(void*));
	if (p) {
		v->data = p;
		v->capacity = v->size;
		return 0;
	} else {
		return 1;
	}
}

int vect_push_back(vect* v, void* x) {
	size_t ind = v->size;
	if (ind == v->capacity) {
		size_t new_cap = v->capacity + v->delta_capacity;
		void** p = realloc(v->data, new_cap * sizeof(void*));
		if (p) {
			v->data = p;
			v->capacity = new_cap;
		} else {
			return 1;
		}
	}
	vect_set(v, ind, x);
	v->size++;
	return 0;
}


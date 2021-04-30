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

#ifndef _VECTOR
#define _VECTOR

typedef struct vect {	
	void** data;
	size_t size;
	size_t capacity;
	size_t delta_capacity;
} vect;

vect* vect_new(size_t capacity, size_t delta_capacity);
void vect_free(vect* v);
void** vect_get(vect* v, int index);
void vect_set(vect* v, int index, void* x);
int vect_push_back(vect* v, void* x);
int vect_compact(vect* v, int free_if_zero);

#endif

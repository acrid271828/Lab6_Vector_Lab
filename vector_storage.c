/**************
* vector_storage.c
* Alex Grant
* 10/4/2026
*
* Stores an array of vectors and interfaces methods to modify and store array
*
* This file never exposes the Vect struct externaly: callers exclusively use names and doubles
*
* to compile: $ make
**************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "vector_storage.h"

static int vectors_size = 0;
static int vectors_length = 0;
static Vect* vectors;
static Vect tmp_vector = {
    .x = 0,
    .y = 0,
    .z = 0
};


/**
 * Private method for getting a vector from the array
 * @param name the vector name to search for
 * @return If the named vector exists: that vector
 * 
 * If the named vector does not exist: a new vector with name = NULL
 */
static Vect get(char* name){
    for(size_t i = 0; i < vectors_size; i++){
        if (!strcmp(name, vectors[i].name)){
            return vectors[i];
        }
    }
    return null_vector();
}

/**
 * Private method for realloc'ing the vectors array
 * @return if succesfull: 0
 * 
 * if unsuccessful: ERR_ALLOCATION
 */
static int reallocate(){
    Vect* old_ptr = vectors;
    vectors = realloc(vectors, vectors_length * sizeof(Vect) * 2);
    if (vectors == NULL){
        vectors = old_ptr;
        return ERR_ALLOCATION;
    }
    vectors_length *= 2;
    return 0;
}

int init_storage(){
    if (vectors != NULL){
        return 1;
    }
    vectors = malloc(VECTOR_ARRAY_INITIAL_LENGTH * sizeof(Vect));
    if (vectors == NULL){
        return ERR_ALLOCATION;
    }
    vectors_length = VECTOR_ARRAY_INITIAL_LENGTH;
    return 0;
}

int fill_string_with_tmp(char* output){
    if (vectors == NULL) return ERR_ARRAY_UNINITIALIZED;
    if (tmp_vector.name == NULL) return ERR_TMP_VECTOR_NOT_SET;
    
    sprintf(output, "%s = %.02f, %.02f, %.02f", tmp_vector.name, tmp_vector.x, tmp_vector.y, tmp_vector.z);
    return 0;
}

int fill_string_from_name(char* output, char* name){
    if (vectors == NULL) return ERR_ARRAY_UNINITIALIZED;
    if (name == NULL) return ERR_NAME_NULL;
    
    Vect v = get(name);
    if (v.name == NULL){
        return ERR_VECTOR_MISSING;
    }
    sprintf(output, "%s = %.02f, %.02f, %.02f", v.name, v.x, v.y, v.z);
    return 0;
}

int print_all(){
    if (vectors == NULL) return ERR_ARRAY_UNINITIALIZED;

    for(size_t i = 0; i < vectors_size; i++){
        printf("%s = %.02f, %.02f, %.02f\n", vectors[i].name, vectors[i].x, vectors[i].y, vectors[i].z);
    }
    return 0;
}

int get_idx(char* name){
    if (vectors == NULL) return ERR_ARRAY_UNINITIALIZED;
    if (name == NULL) return ERR_NAME_NULL;

    for(size_t i = 0; i < vectors_size; i++){
        if (!strcmp(name, vectors[i].name)){
            return i;
        }
    }
    return ERR_VECTOR_MISSING;
}

int assign(char* name, double x, double y, double z){
    if (vectors == NULL) return ERR_ARRAY_UNINITIALIZED;
    if (name == NULL) return ERR_NAME_NULL;
    
    // reassign if exists
    for(size_t i = 0; i < vectors_size; i++){
        if (!strcmp(vectors[i].name, name)){
            vectors[i].x = x;
            vectors[i].y = y;
            vectors[i].z = z;
            return 0;
        }
    }

    // add if not exists
    if (vectors_size >= vectors_length){
        if (reallocate()){
            return ERR_ALLOCATION;
        }
    }

    Vect vector = {
        .name = malloc(strlen(name) + 1),
        .x = x,
        .y = y,
        .z = z
    };
    if (vector.name == NULL){
        return ERR_ALLOCATION;
    }
    strcpy(vector.name, name);
    vectors[vectors_size++] = vector;
    return 0;
}

int clear(){
    if (vectors == NULL) return ERR_ARRAY_UNINITIALIZED;

    for(size_t i = 0; i < vectors_size; i++){
        free(vectors[i].name);
    }
    free(vectors);
    vectors = NULL;
    if (tmp_vector.name != NULL){
        free(tmp_vector.name);
        tmp_vector = null_vector();
    }
    vectors_size = 0;
    return 0;
}

int copy_tmp(char* name){
    if (vectors == NULL) return ERR_ARRAY_UNINITIALIZED;

    Vect v = get(name);
    if (v.name == NULL){
        return ERR_VECTOR_MISSING;
    }
    tmp_vector = v;
    tmp_vector.name = strdup("ans");
    return 0;
}

int add_tmp(char* name1, char* name2){
    if (vectors == NULL) return ERR_ARRAY_UNINITIALIZED;

    Vect v1 = get(name1);
    Vect v2 = get(name2);
    if (v1.name == NULL || v2.name == NULL){
        return ERR_VECTOR_MISSING;
    }
    tmp_vector = add(v1, v2);
    tmp_vector.name = strdup("ans");
    return 0;
}

int subtract_tmp(char* name1, char* name2){
    if (vectors == NULL) return ERR_ARRAY_UNINITIALIZED;

    Vect v1 = get(name1);
    Vect v2 = get(name2);
    if (v1.name == NULL || v2.name == NULL){
        return ERR_VECTOR_MISSING;
    }
    tmp_vector = subtract(v1, v2);
    tmp_vector.name = strdup("ans");
    return 0;
}

int scale_tmp(char* name, double scalar){
    if (vectors == NULL) return ERR_ARRAY_UNINITIALIZED;

    Vect v = get(name);
    if (v.name == NULL){
        return ERR_VECTOR_MISSING;
    }
    tmp_vector = scale(v, scalar);
    tmp_vector.name = strdup("ans");
    return 0;
}

int assign_tmp(char* name){
    if (vectors == NULL) return ERR_ARRAY_UNINITIALIZED;
    if (tmp_vector.name == NULL) return ERR_TMP_VECTOR_NOT_SET;
    if (name == NULL) return ERR_NAME_NULL;

    int code = assign(name, tmp_vector.x, tmp_vector.y, tmp_vector.z);
    if (!code){
        free(tmp_vector.name);
        tmp_vector = null_vector();
    }

    return code;
}
/**************
* vector_storage.c
* Alex Grant
* 10/4/2026
*
* Stores an array of vectors and interfaces methods to modify and store array
*
* This file never exposes the Vect struct externaly: callers exclusively use names and doubles
*
* Uses a vector name == NULL to denote that a vector is unset/the array location is empty
* , so NULL can never be set as a valid vector's name
*
* to compile: $ make
**************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "vector_storage.h"

static Vect vectors[VECTOR_ARRAY_LENGTH] = {0};
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
    for(size_t i = 0; i < VECTOR_ARRAY_LENGTH && vectors[i].name != NULL; i++){
        if (!strcmp(name, vectors[i].name)){
            return vectors[i];
        }
    }
    return null_vector();
}

int fill_string_with_tmp(char* output){
    if (tmp_vector.name == NULL){
        return ERR_TMP_VECTOR_NOT_SET;
    }
    sprintf(output, "%s = %.02f, %.02f, %.02f", tmp_vector.name, tmp_vector.x, tmp_vector.y, tmp_vector.z);
    return 0;
}

int fill_string_from_name(char* output, char* name){
    if (name == NULL){
        return ERR_NAME_NULL;
    }
    Vect v = get(name);
    if (v.name == NULL){
        return ERR_VECTOR_MISSING;
    }
    sprintf(output, "%s = %.02f, %.02f, %.02f", v.name, v.x, v.y, v.z);
    return 0;
}

int fill_string_from_index(char* output, int index){
    if (index < 0 || index > VECTOR_ARRAY_LENGTH){
        return ERR_INDEX_OUT_OF_BOUNDS;
    }
    if (vectors[index].name == NULL){
        strcpy(output, "<empty>");
    } else {
        sprintf(output, "%s = %.02f, %.02f, %.02f", vectors[index].name, vectors[index].x, vectors[index].y, vectors[index].z);
    }
    return 0;
    
}

int get_idx(char* name){
    if (name == NULL){
        return ERR_NAME_NULL;
    }
    for(size_t i = 0; i < VECTOR_ARRAY_LENGTH; i++){
        if (!strcmp(name, vectors[i].name)){
            return i;
        }
    }
    return ERR_VECTOR_MISSING;
}

int assign(char* name, double x, double y, double z){
    if (name == NULL){
        return ERR_NAME_NULL;
    }
    for(size_t i = 0; i < VECTOR_ARRAY_LENGTH; i++){
        if (vectors[i].name == NULL){
            vectors[i].name = malloc(strlen(name));
            strcpy(vectors[i].name, name);
        }
        if (vectors[i].name == NULL || !strcmp(vectors[i].name, name)){
            vectors[i].x = x;
            vectors[i].y = y;
            vectors[i].z = z;
            return 0;
        }
    }
    return ERR_STORAGE_FULL;
}

void clear(){
    for(size_t i = 0; i < VECTOR_ARRAY_LENGTH; i++){
        free(vectors[i].name);
        vectors[i].name = NULL;
    }
}

int copy_tmp(char* name){
    Vect v = get(name);
    if (v.name == NULL){
        return ERR_VECTOR_MISSING;
    }
    tmp_vector = v;
    tmp_vector.name = strdup("ans");
    return 0;
}

int add_tmp(char* name1, char* name2){
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
    Vect v = get(name);
    if (v.name == NULL){
        return ERR_VECTOR_MISSING;
    }
    tmp_vector = scale(v, scalar);
    tmp_vector.name = strdup("ans");
    return 0;
}

int assign_tmp(char* name){
    if (tmp_vector.name == NULL){
        return ERR_TMP_VECTOR_NOT_SET;
    }
    if (name == NULL){
        return ERR_NAME_NULL;
    }
    for(size_t i = 0; i < VECTOR_ARRAY_LENGTH; i++){
        // if name does not exist, overwrite first null element (add to array)
        // if name does exists, overwrite that field
        if (vectors[i].name == NULL || !strcmp(name, vectors[i].name)){
            vectors[i] = tmp_vector;
            vectors[i].name = malloc(strlen(name));
            strcpy(vectors[i].name, name);
            free(tmp_vector.name);
            tmp_vector = null_vector();
            return 0;
        }
    }
    return ERR_STORAGE_FULL;
}
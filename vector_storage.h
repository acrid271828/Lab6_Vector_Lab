/**************
* vector_storage.h
* Alex Grant
* 10/20/2026
*
* Stores an array of vectors and interfaces methods to modify and store array
*
* This file never exposes the Vect struct externaly: callers exclusively use names and doubles
*
* to compile: $ make
**************/
#ifndef VECTOR_STORAGE_H
#define VECTOR_STORAGE_H
#define ERR_VECTOR_MISSING -1
#define ERR_NAME_NULL -2
#define ERR_STORAGE_FULL -3
#define ERR_TMP_VECTOR_NOT_SET -4
#define ERR_ALLOCATION -5
#define ERR_ARRAY_UNINITIALIZED -6

/**
 * Initializes the storage array. This MUST be called before any other function
 * 
 * @return if successful: 0
 * 
 * if array already exists (good): 1
 * 
 * if fails: ERR_ALLOCATION
 */
int init_storage();

/**
 * Formats a string representation of only vector magnitudes, "x, y, z"
 * @param output the string to store the output in
 * @return if the vector exists: 0
 * 
 * if the temp vector was not set: ERR_TMP_VECTOR_NOT_SET
 * 
 * if init_storage() was not completed succesfully: ERR_ARRAY_UNINITIALIZED
 */
int fill_string_with_tmp(char* output);

/**
 * Formats a string representation of a vector, "name = x, y, z"
 * @param output the string to store the output in
 * @param name the vector name to search for
 * @return if the vector exists: 0
 * 
 * if the vector does not exist: ERR_VECTOR_MISSING
 * 
 * if the input name is NULL: ERR_NAME_NULL
 * 
 * if init_storage() was not completed succesfully: ERR_ARRAY_UNINITIALIZED
 */
int fill_string_from_name(char* output, char* name);

/**
 * Prints all vectors in storage
 * 
 * @return if successful: 0
 * 
 * if init_storage() was not completed succesfully: ERR_ARRAY_UNINITIALIZED
 */
int print_all();

/**
 * Gets the index of a vector in the storage array
 * @param name the vector name to search for
 * @return if the vector exists: the index
 * 
 * if the vector does not exists: ERR_VECTOR_MISSING
 * 
 * if the input name is NULL: ERR_NAME_NULL
 * 
 * if init_storage() was not completed succesfully: ERR_ARRAY_UNINITIALIZED
 */
int get_idx(char* name);

/**
 * Creates a new vector and stores it in the array. 
 * @param name the name to store as. If a vector of the same name already exists, it will be overwritten
 * @param x the vector's x component
 * @param y the vector's y component
 * @param z the vector's z component
 * @return if successful: 0
 * 
 * if the array is full: ERR_STORAGE_FULL
 * 
 * if the input name is NULL: ERR_NAME_NULL
 * 
 * if init_storage() was not completed succesfully: ERR_ARRAY_UNINITIALIZED
 */
int assign(char* name, double x, double y, double z);

/**
 * Clears the entire vector array memory. After this, init_storage() must be used to continue using the storage array
 * @return if successful: 0
 * 
 * if init_storage() was not completed succesfully: ERR_ARRAY_UNINITIALIZED
 */
int clear();

/**
 * Creates a copy of a stored vector and stores the copy as the temp vector
 * @param name the name of the vector to copy
 * @return if successful: 0
 * 
 * if the named vector does not exists: ERR_VECTOR_MISSING
 * 
 * if init_storage() was not completed succesfully: ERR_ARRAY_UNINITIALIZED
 */
int copy_tmp(char* name);

/**
 * Adds 2 stored vectors and stores the resultant vector as the temp vector
 * @param name1 the name of the first vector
 * @param name2 the name of the second vector
 * @return if successful: 0
 * 
 * if either named vector does not exists: ERR_VECTOR_MISSING
 * 
 * if init_storage() was not completed succesfully: ERR_ARRAY_UNINITIALIZED
 */
int add_tmp(char* name1, char* name2);

/**
 * Subtracts 2 stored vectors and stores the resultant vector as the temp vector
 * @param name1 the name of the first (positive) vector
 * @param name2 the name of the second (negative) vector
 * @return if successful: 0
 * 
 * if either named vector does not exists: ERR_VECTOR_MISSING
 * 
 * if init_storage() was not completed succesfully: ERR_ARRAY_UNINITIALIZED
 */
int subtract_tmp(char* name1, char* name2);

/**
 * Scales a stored vector and stores the resultant vector as the temp vector
 * @param name the name of the initial vector
 * @param scalar the scalar to scale by
 * @return if successful: 0
 * 
 * if the named vector does not exists: ERR_VECTOR_MISSING
 * 
 * if init_storage() was not completed succesfully: ERR_ARRAY_UNINITIALIZED
 */
int scale_tmp(char* name, double scalar);

/**
 * Stores the temp vector in the array, then clears the temp vector
 * @param name the name to store as. If a vector of the same name already exists, it will be overwritten
 * @return if successful: 0
 * 
 * if the input name is NULL: ERR_NAME_NULL
 * 
 * if the array is full: ERR_STORAGE_FULL
 * 
 * if the temp vector doesn't have a value: ERR_TMP_VECTOR_NOT_SET
 * 
 * if init_storage() was not completed succesfully: ERR_ARRAY_UNINITIALIZED
 */
int assign_tmp(char* name);

/**
 * Writes all vectors to an opened file
 * @return 0
 */
int save_all(FILE* file);

#endif
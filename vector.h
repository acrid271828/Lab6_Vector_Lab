/**************
* vector.h
* Alex Grant
* 9/29/2026
* Provides simple vector math functions
* to compile: $ make
**************/
#ifndef VECTOR_H
#define VECTOR_H

typedef struct{
    char* name;
    double x;
    double y;
    double z;
} Vect;

/**
 * Shorthand to create a null vector 
 * @return a vector with a NULL name and (0,0,0) magnitudes
 */
Vect null_vector();

/**
* Adds two vectors
* @return the resultant vector
*/
Vect add(Vect a, Vect b);

/**
* Subtracts two vectors
* @return the resultant vector
*/
Vect subtract(Vect a, Vect b);

/**
* Multiplies a vector by a scalar
* @return the resultant vector
*/
Vect scale(Vect a, double scalar);


#endif
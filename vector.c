/**************
* vector.c
* Alex Grant
* 9/29/2026
* Provides simple vector math functions
* to compile: $ make
**************/
#include "vector.h"

Vect null_vector(){
    Vect v = {0};
    return v;
}

Vect add(Vect a, Vect b){
    Vect c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    c.w = a.w + b.w;
    return c;
}

Vect subtract(Vect a, Vect b){
    Vect c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    c.w = a.w + b.w;
    return c;
}


Vect scale(Vect a, double scalar){
    Vect c;
    c.x = a.x * scalar;
    c.y = a.y * scalar;
    c.z = a.z * scalar;
    c.w = a.w * scalar;
    return c;
}
/**************
* minimat_ui.c
* Alex Grant
* 9/29/2026
* Controls the UI and input parsing for vector math
* to compile: $ make
**************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "minimat_ui.h"
#include "vector_storage.h"
#define MAX_TOKENS 5

static char vector[100];
static char* tokens[MAX_TOKENS];

void launch_minimat(){
    char input[50];
    do {
        // tokenize input
        printf("evil minimat> ");
        fgets(input, 50, stdin);
        input[strcspn(input,"\n")] = '\0';
        consume_input(input);
        
    } while(strcmp(input, "quit"));
}

void consume_input(char* input){
    char* token = strtok(input, " ");
    tokens[0] = token;
    for(int i = 1; i < MAX_TOKENS && token != NULL; i++){
        token = strtok(NULL, " ");
        tokens[i] = token;
    }

    // check for excess input
    if (token != NULL){
        char* remainder = token;
        remainder += strlen(token) + 1;
        if (*remainder != '\0'){
            printf("Too many tokens. The following input will be discarded: %s\n", remainder);
        }
    }

    if (require_tokens(1)) 
        return;

    // commands
    if (!strcmp(tokens[0], "quit")) 
        return;
    if (!strcmp(tokens[0], "clear")) {
        clear();
        printf("Array clear!\n");
        return;
    }
    if (!strcmp(tokens[0], "list")) {
        for (int i = 0; i < VECTOR_ARRAY_LENGTH; i++){
            fill_string_from_index(vector, i);
            printf("%d: %s\n", i, vector);
        }
        return;
    }

    // print vector
    if (tokens[1] == NULL){
        if (!handle(fill_string_from_name(vector, tokens[0]))){
            printf("%s\n", vector);
        }
        return;
    }

    // assign vector
    if (!strcmp(tokens[1], "=")){
        if (require_tokens(5))
            return; 

        if (!strcmp(tokens[3], "+") || !strcmp(tokens[3], "-") || !strcmp(tokens[3], "*")){
            if (handle_name(tokens[0]))
                return;

            if (calc(tokens[2],tokens[3],tokens[4]))
                return;

            if (handle(assign_tmp(tokens[0])))
                return;

            fill_string_from_name(vector, tokens[0]);
            printf("I did the calculation, buuut I'm not gonna tell you the result (because I'm evil)\n");        
        } else {
            double magnitudes[3];
            int error = 0;
            for(int i = 0; i < 3; i++){
                double mag[2];
                ascii_to_float(mag, tokens[i + 2]);
                if(mag[0] >= 0){
                    magnitudes[i] = mag[1];
                } else {
                    printf("Expected float at position %d (found \'%s\')\n", i + 2, tokens[i + 2]);
                    error = 1;
                }
            }
            if (!error && !handle(assign(tokens[0], magnitudes[0], magnitudes[1], magnitudes[2]))){
                fill_string_from_name(vector, tokens[0]);
                printf("%s\n", vector);
            }
        }
    } else if (!strcmp(tokens[1], "+") || !strcmp(tokens[1], "-") || !strcmp(tokens[1], "*")){
        if (require_tokens(3))
            return; 

        if (calc(tokens[0],tokens[1],tokens[2])) 
            return;

        fill_string_with_tmp(vector);
        printf("I did the calculation, buuut I'm not gonna tell you the result (because I'm evil)\n");  
    } else {
        printf("Illegal token: 2nd position must be an operator (=, +, -, *) (found \'%s\')\n", tokens[1]);
    }
}

int require_tokens(int c){
    int tokenc = 0;
    for(; tokenc < MAX_TOKENS && tokens[tokenc] != NULL; tokenc++) {
        // do nothing; only increment tokenc
    }
    if (c > tokenc){
        printf("Expected %d total tokens separated by a space (found %d)\n", c, tokenc);
        return -1;
    }
    return 0;
}

int calc(char* a, char* op, char* b){
    if (!strcmp(op,"+") || !strcmp(op,"-")){
        if(handle_name(a) || handle_name(b)) 
            return -1;

        if(handle(!strcmp(op,"+") ? add_tmp(a,b) : subtract_tmp(a,b))) 
            return -1;

        return 0;
    } else if (!strcmp(op,"*")){
        double scalar;
        char* vect;
        double resultA[2];
        ascii_to_float(resultA, a);
        double resultB[2];
        ascii_to_float(resultB, b);
        if (resultA[0] != 0.0 && resultB[0] == 0.0){
            // a * 2
            vect = a;
            scalar = resultB[1];
        } else if (resultA[0] == 0.0 && resultB[0] != 0.0){
            // 2 * a
            vect = b;
            scalar = resultA[1];
        } else if (resultA[0] != 0.0 && resultB[0] != 0.0){
            // a * b
            printf("Expected one float scalar for '*' (found 2 names: \'%s\' and \'%s\')\n", a, b);
            return -1;
        } else {
            // 2 * 3
            printf("Expected one vector name for '*' (found 2 float scalars: \'%f\' and \'%f\')\n", resultA[1], resultB[1]);
            return -1;
        }

        if(handle(scale_tmp(vect,scalar)))
            return -1;

        return 0;
    }
    return -1;
}

void ascii_to_float(double results[2], char* a){
    results[1] = atof(a);
    if (results[1] == 0.0){
        // check actual 0 vs error
        if (!(*a == '0' || 

            ((*a == '-' || *a == '+') 
                && *(a+1) == '0'))){
        
            results[0] = -1;
        } 
    } else {
            results[0] = 0;
    }
}

int handle_name(char* name){
    double results[2];
    ascii_to_float(results, name);
    if (results[0] >= 0){
        printf("Vector names cannot start with a number (found \'%s\')\n", name);
        return -1;
    }
    return 0;
}

int handle(int status_code){
    if (!status_code){
        return 0;
    }
    switch(status_code){
        case ERR_NAME_NULL:
            printf("Vector names cannot be NULL\n");
            break;
        case ERR_STORAGE_FULL:
            printf("No space for new vector (use 'clear' to empty the storage)\n");
            break;
        case ERR_VECTOR_MISSING:
            printf("A vector with that name does not exists (check with 'list')\n");
            break;
        case ERR_TMP_VECTOR_NOT_SET:
        case ERR_INDEX_OUT_OF_BOUNDS:
            printf("Internal error: code %d\n", status_code);
            break;
        default:
            printf("Unknown error: code %d\n", status_code);
            break;
    }
    return -1;
}
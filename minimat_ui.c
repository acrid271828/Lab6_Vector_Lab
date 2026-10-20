/**************
* minimat_ui.c
* Alex Grant
* 10/20/2026
* Controls the UI and input parsing for vector math
* to compile: $ make
**************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "minimat_ui.h"
#include "vector_storage.h"
#define MAX_TOKENS 5
#define MAX_CSV_TOKENS 4

volatile sig_atomic_t cancel = 0;

void handle_sigint(int sig){
    cancel = 1;
}

static char vector[100];
static char* tokens[MAX_TOKENS];

/**
 * Prints an error message for the corresponding status code and collapses all error codes to -1
 * @param status_code the return code of a vector_storage.c method to check
 * @return the status_code
 */
static int handle(int status_code){
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
        case ERR_ALLOCATION:
            printf("Memory allocation error. Please try again\n");
            break;
        case ERR_ARRAY_UNINITIALIZED:
            printf("Memory state error. Attempting a fix...\n");
            if (init_storage() >= 0){
                printf("Success! Please retry your latest operation\n");
            } else {
                printf("Memory allocation error. Please try again\n");
            }
            break;
        case ERR_TMP_VECTOR_NOT_SET:
            printf("Internal error: code %d\n", status_code);
            break;
        default:
            printf("Unknown error: code %d\n", status_code);
            break;
    }
    return status_code;
}

/**
 * Wraps atof() with error detection.
 * @param results the array to store the following results in:
 * 
 * [0] = isSuccessful ? 0.00 : -1.00
 * 
 * [1] = atof(a);
 * 
 * @param a the ascii to parse
 */
static void ascii_to_float(double results[2], char* a){
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

/**
 * Checks if a vector name is invalid, and prints a message for the user if so
 * @param name the name to parse
 * @return isValidName ? 0 : -1
 */
static int handle_name(char* name){
    double results[2];
    ascii_to_float(results, name);
    if (results[0] >= 0){
        printf("Vector names cannot start with a number (found \'%s\')\n", name);
        return -1;
    }
    return 0;
}

/**
 * Performs a calculation of a and b with op operator. Result is stored in vector_storage temp vector
 * @param a the first operand
 * @param op the operator. MUST BE "+", "-", or "*"
 * @param b the second operand
 * @return isSuccessful ? 0 : -1
 */
static int calc(char* a, char* op, char* b){
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

/**
 * Validates that there are at least c tokens
 * @return isValid ? 0 : -1
 */
static int require_tokens(int c){
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

/**
 * Opens a file pointer
 * @return isSuccessful ? 1 : 0
 */
static int handle_open(FILE** file_ptr, char* filename, char* mode){
    *file_ptr = fopen(filename, mode);
    if (*file_ptr == NULL){
        printf("Could not find file.\n");
        return -1;
    }
    return 0;
}

/**
 * Parses all lines of a file as inputs
 */
static int load_csv(char* filename){
    FILE* file_ptr;

    if (handle_open(&file_ptr, filename, "r"))
        return -1;


    char input[50];
    int i = 0;
    while(fgets(input, 50, file_ptr) != NULL){
        printf("%d: ", ++i);

        input[strcspn(input,"\n")] = '\0';
        char* token = strtok(input, ",");
        char* csv_tokens[MAX_CSV_TOKENS] = {0};
        csv_tokens[0] = token;
        int has_spaces = 0;
        for(int i = 1; i < MAX_CSV_TOKENS && token != NULL; i++){
            token = strtok(NULL, ",");
            csv_tokens[i] = token;
            if (token != NULL && strcspn(token, " ") < strlen(token)){
                has_spaces = 1;
            }
        }

        if (has_spaces){
            printf("Values cannot contain spaces\n");
        } else if (csv_tokens[MAX_CSV_TOKENS - 1] != NULL){
            char assignment_str[60];
            sprintf(assignment_str, "%s = %s %s %s%c", 
                csv_tokens[0], csv_tokens[1], csv_tokens[2], csv_tokens[3], '\0');
            consume_input(assignment_str);
        } else {
            printf("Less than 4 values\n");
        }
    }

    fclose(file_ptr);
    return 0;
}

/**
 * Saves all vectors in storage to a file
 */
static int save_csv(char* filename){
    FILE* file_ptr;

    if (handle_open(&file_ptr, filename, "w"))
        return -1;

    save_all(file_ptr);
    fclose(file_ptr);
    return 0;
}

void launch_minimat(){
    init_storage();
    char input[50];

    struct sigaction act;
    act.sa_handler = handle_sigint;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction(SIGINT, &act, NULL) < 0){
        printf("WARNING: signal error. Exiting with Ctrl+C (instead of the quit command) may cause a memory leak\n");
    }

    printf("minimat> ");
    fgets(input, 50, stdin);
    input[strcspn(input,"\n")] = '\0';
    while(strcmp(input, "quit") && !cancel){
        consume_input(input);
        printf("minimat> ");
        fgets(input, 50, stdin);
        input[strcspn(input,"\n")] = '\0';
    }

    printf("\nClearing data...\n");
    clear();
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
    if (!strcmp(tokens[0], "clear")) {
        clear();
        printf("Array clear!\n");
        handle(init_storage());
        return;
    }
    if (!strcmp(tokens[0], "list")) {
        print_all();
        return;
    }
    if (!strcmp(tokens[0], "load")){
        if (require_tokens(2)){
            printf("usage: load <filename>\n");
            return;
        }
        load_csv(tokens[1]);
        return;
    }
    if (!strcmp(tokens[0], "save")){
        if (require_tokens(2)){
            printf("usage: save <filename>\n");
            return;
        }
        if (!save_csv(tokens[1])){
            printf("Done!\n");
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

        if (handle_name(tokens[0]))
            return;

        if (!strcmp(tokens[3], "+") || !strcmp(tokens[3], "-") || !strcmp(tokens[3], "*")){
            if (calc(tokens[2],tokens[3],tokens[4]))
                return;

            if (handle(assign_tmp(tokens[0])))
                return;

            fill_string_from_name(vector, tokens[0]);
            printf("%s\n", vector);
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
        printf("%s\n", vector);
    } else {
        printf("Illegal token: 2nd position must be an operator (=, +, -, *) (found \'%s\')\n", tokens[1]);
    }
}
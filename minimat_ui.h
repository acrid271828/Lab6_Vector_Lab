/**************
* minimat_ui.h
* Alex Grant
* 9/29/2026
* Controls the UI and input parsing for vector math
* to compile: $ make
**************/

/**
 * Runs the minimat program, for doing vector math, in an infinite loop until the user quits
 */
void launch_minimat();

/**
 * Fully processes one user input to the program
 */
void consume_input(char* input);

/**
 * Validates that there are at least c tokens
 * @return isValid ? 0 : -1
 */
int require_tokens(int c);


/**
 * Performs a calculation of a and b with op operator. Result is stored in vector_storage temp vector
 * @param a the first operand
 * @param op the operator. MUST BE "+", "-", or "*"
 * @param b the second operand
 * @return isSuccessful ? 0 : -1
 */
int calc(char* a, char* op, char* b);

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
void ascii_to_float(double results[2], char* a);

/**
 * Checks if a vector name is invalid, and prints a message for the user if so
 * @param name the name to parse
 * @return isValidName ? 0 : -1
 */
int handle_name(char* name);

/**
 * Prints an error message for the corresponding status code and collapses all error codes to -1
 * @param status_code the return code of a vector_storage.c method to check
 * @return 0 if the status_code is 0
 * 
 * -1 if the status_code is not 0
 */
int handle(int status_code);

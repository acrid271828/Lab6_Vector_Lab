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
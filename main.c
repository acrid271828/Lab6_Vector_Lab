/**************
* main.c
* Alex Grant
* 10/20/2026
* Boots the minimat user interface for vector math
* to compile: $ make
**************/
#include <stdio.h>
#include <string.h>
#include "minimat_ui.h"

int main(int argc, char* argv[]){
	for(size_t i = 1; i < argc; i++){
		if (!strcmp(argv[i], "-h")){
			printf("Commands:\n");
			printf("%-5s : exit the program\n", "quit");
			printf("%-5s : lists all stored vectors\n", "list");
			printf("%-5s : deletes all stored vectors\n", "clear");
			printf("%-15s : loads all vectors from a file in csv format\n", "load <filename>");
			printf("%-15s : loads all vectors from a file in csv format\n", "save <filename>");
			printf("\nOperations:\n");
			printf("%-20s | varA = VALx, VALy, VALz\n", "Initial Assignment");
			printf("%-20s | varC = varA + varB\n", "Addition & Assign");
			printf("%-20s | varC = varA - varB\n", "Subtraction & Assign");
			printf("%-20s | varB = varA * VALs | varB = VALs * varA\n", "Scalar & Assign");
			printf("%-20s | varA + varB\n", "Addition");
			printf("%-20s | varA - varB\n", "Subtraction");
			printf("%-20s | varA * VALs | VALs * varA\n", "Scalar");

		}
	}
	launch_minimat();

	return 0;
}
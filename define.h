/***********************************************************************
 *    Contains all the function and variable declarations
 * ********************************************************************/
#ifndef DEFINE_H 
#define DEFINE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "structs.h"
#include "parameter.h"

/***********************************************************************
 *              defined in main.c
 * ********************************************************************/
extern FILE *circuit_fd, *fault_fd;
extern const char* circuit_name;
extern const char* fault_name;


/***********************************************************************
*                     defined in common.c                              *
***********************************************************************/
extern const char* program_name;

/* error message for a failed call operation */
extern void system_error (const char* operation);

/* Print the error message and end the program */
extern void error (const char* cause, const char* message);

/* Like malloc, except aborts the program if allocation fails */
extern void* xmalloc (size_t size);

/***********************************************************************
*                    defined in read_circuit.c                         *
***********************************************************************/
extern GATEPTR begnet;

extern HASHPTR symbol_tbl[HASHSIZE]; //an array of struct hash pointers
// struct hash *symbol_tbl[HASHSIZE];  its the same

extern int read_circuit (FILE* , const char*);
extern void InitHash (HASHPTR*, int);
extern char getsymbol (FILE*, char*);

#endif 

/***********************************************************************
 *    Contains all the function and variable declarations
 * ********************************************************************/
#ifndef DEFINE_H 
#define DEFINE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "structs.h"
#include "parameter.h"

#define         AND             0
#define         NAND            1
#define         OR              2
#define         NOR             3
#define         PI              4
#define         NOT             5
#define         PO              6


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
extern void system_error (const char*);

/* Print the error message and end the program */
extern void error (const char*, const char*);

/* Like malloc, except aborts the program if allocation fails */
extern void* xmalloc (size_t);

/***********************************************************************
*                    defined in read_circuit.c                         *
***********************************************************************/
extern GATEPTR begnet;
extern HASHPTR symbol_tbl[HASHSIZE]; //an array of struct hash pointers
// struct hash *symbol_tbl[HASHSIZE];  its the same

extern int read_circuit (FILE* , const char*);
extern int gatetype(char*);

/***********************************************************************
 *                   defined in hashes.c                               *
 * ********************************************************************/
extern void InitHash (HASHPTR*, int);
extern HASHPTR Find_and_Insert_Hash (HASHPTR*, int, char*, int);
extern HASHPTR FindHash (HASHPTR*, int, char*, int);
extern HASHPTR InsertHash (HASHPTR*, int, char*, int);
extern HASHPTR hashalloc ();
extern int keyvalue (char*);
extern char* astrcpy(char*, char*);
extern char getsymbol (FILE*, char*);

#endif 

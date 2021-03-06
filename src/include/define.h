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


/***********************************************************************
 *              defined in main.c
 * ********************************************************************/
extern FILE *circuit_fd, *fault_fd, *vectors_fd,*test_fd;
extern const char* circuit_name;
extern const char* fault_name;
extern const char* vectors_name;
extern int nodummy;
extern int* LUT;
extern char test_name[100];


extern void option_set (int argc, char* const argv[]);
extern void handle_files (const char*,const char*);


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
extern void xfree (void*);

/***********************************************************************
*                    defined in read_circuit.c                         *
***********************************************************************/
extern GATEPTR *net;
extern GATEPTR begnet;
extern int *primaryin,*primaryout;
extern int nog,nopi,nopo;
extern HASHPTR symbol_tbl[HASHSIZE]; //an array of struct hash pointers
// struct hash *symbol_tbl[HASHSIZE];  its the same

extern int read_circuit (FILE*);
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

/***********************************************************************
 *                     defined in structures.c						   *
 * ********************************************************************/
 extern int add_PO ();
 extern int maxlevel;
 extern STACKPTR event_list;
 extern STACKTYPE stack1, stack2;
 extern int *levels;  //number of gates in each leve
 extern int compute_level ();
 extern void allocate_stacks();
 extern void allocate_event_list();
 extern void levelize();
 extern void place_PO ();

/***********************************************************************
 * 						defined in read_vectors.c					   *
 * ********************************************************************/
extern char getvector (FILE* , char*);
extern int read_vectors (FILE *,const char*);
extern void allocate_and_init ();
extern int find_offset (GATEPTR);
extern char **test_set;
extern int patterns;  //number of vectors for simulation

/***********************************************************************
* 						defined in logic_sim.c						   *
*********************************************************************/
 extern int* create_lut (int *);
 extern int gate_eval (THREADTYPE );
 extern void logic_sim ();
 extern void print_logic_sim();

/**********************************************************************
* 					defined in fault_sim.c                         *
* *********************************************************************/
extern FAULTPTR fault_list;
extern int total_faults;
extern void create_fault_list ();
extern void print_fault_list ();
extern void allocate_and_init_faults ();
extern void fault_sim ();
extern void set_injection_bits(FAULTYPE );
extern void print_fault_sim (FILE *);
int fault_eval (THREADFAULTYPE );
#endif

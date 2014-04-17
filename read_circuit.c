#include "define.h"
#include "structs.h"


HASHPTR symbol_tbl[HASHSIZE];
GATEPTR begnet;



int read_circuit (FILE *circuit_fd, const char* circuit_name)
{
    char c;
    int fn;
    char symbol[MAXSTRING];
	register HASHPTR hp;
	register GATEPTR cg;
	begnet=(GATEPTR)NULL;
	
	
    InitHash (symbol_tbl, HASHSIZE);
    while ((c = getsymbol (circuit_fd, symbol)) != EOF) {
			switch (c) {
				
				case '=' :
					hp = Find_and_Insert_Hash (symbol_tbl, HASHSIZE, symbol, 0);
					printf("in read: hp =  %s \n", symbol);
					
					if ((cg = hp->pnode) == NULL) {
						cg = (GATETYPE *)xmalloc(sizeof(GATETYPE)); 
						hp->pnode = cg;
						cg->symbol = hp;
						cg->next = begnet;
						begnet = cg;
					}
				break;
					
				case '(' :
					if ((fn = gatetype (symbol)) < 0) {
						fprintf (stderr, "Error: Gate type %s is not valid\n", symbol);
						return(-1);
					}
				break;
					
					
				
			}
	 
    }
	return 0;
}

int gatetype(char *symbol)
{
	int fn;
	
    if (strcmp(symbol, "NOT") == 0) fn = NOT;
    else if (strcmp(symbol, "AND") == 0) fn = AND;
    else if (strcmp(symbol, "NAND") == 0) fn = NAND;
    else if (strcmp(symbol, "OR")== 0) fn = OR;
    else if (strcmp(symbol, "NOR") == 0) fn = NOR;
    else if (strcmp(symbol, "INPUT") == 0) fn = PI;
    else if (strcmp(symbol, "OUTPUT") == 0) fn = PO;
    else if (strcmp(symbol, "not") == 0) fn = NOT;
    else if (strcmp(symbol, "and") == 0) fn = AND;
    else if (strcmp(symbol, "nand") == 0) fn = NAND;
    else if (strcmp(symbol, "or")== 0) fn = OR;
    else if (strcmp(symbol, "nor") == 0) fn = NOR;
    else if (strcmp(symbol, "input") == 0) fn = PI;
    else if (strcmp(symbol, "output") == 0) fn = PO;
    else fn = (-1);

    return(fn);
}

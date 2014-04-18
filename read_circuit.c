#include "define.h"
#include "structs.h"


HASHPTR symbol_tbl[HASHSIZE];
GATEPTR begnet;



int read_circuit (FILE *circuit_fd, const char* circuit_name)
{
    char c;
    int fn, nofanin, i;
    int int_nog, int_nopi, int_nopo;
    char symbol[MAXSTRING];
	register HASHPTR hp;
	register GATEPTR cg;
	GATEPTR pg;
	begnet=(GATEPTR)NULL;
	GATEPTR pfanin[MAXFIN+100];
	GATEPTR po_gates[MAXPO+100];
	
    int_nog = int_nopi = int_nopo = 0;
    nofanin=0;
    
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
				
				case ',' :             
					hp = Find_and_Insert_Hash (symbol_tbl, HASHSIZE, symbol, 0);
					if ((pg = hp->pnode) == NULL) {
						pg = (GATETYPE *)xmalloc(sizeof(GATETYPE));
						hp->pnode = pg;
						pg->symbol = hp;
						pg->index = (-1);
						pg->next = begnet;
						begnet = pg;
					}
					pfanin[nofanin++]=pg;
				break;
					
				case ')':				
					hp = Find_and_Insert_Hash (symbol_tbl, HASHSIZE, symbol, 0);
					if ((pg = hp->pnode) == NULL) {
						pg = (GATETYPE *)xmalloc(sizeof(GATETYPE));
						hp->pnode = pg;
						pg->symbol = hp; 
						pg->index = (-1);
						pg->next = begnet;
						begnet = pg;
					}		
				
				    switch(fn) {
						
						case PI:
							int_nopi++;
							pg->index = int_nog++;
							pg->ninput=0;
							pg->inlis=(GATEPTR *)NULL;
							pg->fn=PI;
							pg->noutput=0;
							pg->outlis=(GATEPTR *)NULL;
						break;
						
						case PO:
							po_gates[int_nopo++] = pg;
						break;
						
						default:
							pfanin[nofanin++] = pg;

							if (cg == NULL) {
								fprintf(stderr,"Error: Syntax error in the circuit file\n");
								return(-1);
							}
						
							cg->index = int_nog++;
							cg->fn=fn;
							if ((cg->ninput = nofanin) == 0) cg->inlis = NULL;
							else { cg->inlis = (GATEPTR *)xmalloc(cg->ninput*(sizeof(GATEPTR)));
								 //ALLOCATE(cg->inlis,GATEPTR,cg->ninput); 
							}
							for(i=0; i<nofanin; i++) cg->inlis[i]=pfanin[i];
							cg->noutput=0;
							cg->outlis=(GATEPTR *)NULL;
							
							nofanin=0;
							cg = (GATEPTR) NULL;
							break; 
					}		
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

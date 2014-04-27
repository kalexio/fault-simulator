#include "define.h"
#include "structs.h"


HASHPTR symbol_tbl[HASHSIZE];
GATEPTR begnet;
GATEPTR *net;
int *primaryin, *primaryout;
int nog, nopi, nopo;



int read_circuit (FILE *circuit_fd)
{
    char c;
    int nerrs = 0;
    int fn, nofanin, i, j;
    int int_nog, int_nopi, int_nopo;
    int net_size;
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
					//printf("in read: hp =  %s \n", symbol);
					
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
					pfanin[nofanin++] = pg;
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
							pg->ninput = 0;
							pg->inlis=(GATEPTR *)NULL;
							pg->fn = PI;
							pg->noutput = 0;
							pg->outlis = (GATEPTR *)NULL;
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
							cg->fn = fn;
							if ((cg->ninput = nofanin) == 0) cg->inlis = NULL;
							else { cg->inlis = (GATEPTR *)xmalloc(cg->ninput*(sizeof(GATEPTR)));
							}
							for (i = 0; i<nofanin; i++) cg->inlis[i] = pfanin[i];
							cg->noutput = 0;
							cg->outlis = (GATEPTR *)NULL;
							
							nofanin = 0;
							cg = (GATEPTR) NULL;
							break; 
					}		
			}
    }
    
    
    
    net_size = int_nog + int_nopo;
    //printf(" net = %d\n",net_size);
    
    net = (GATEPTR *)xmalloc(net_size*(sizeof(GATEPTR)));
    primaryin = (int *)xmalloc(int_nopi*(sizeof(int)));
    primaryout = (int *)xmalloc(int_nopo*(sizeof(int)));
    
    nog=nopi=nopo=0;
    
    for (cg = begnet; cg != NULL; cg = cg->next) {
		if (cg->index < 0) {
			fprintf(stderr,"Error: floating net %s\n",cg->symbol->symbol);
			fprintf(stderr, "Workaround. You have to take one of the two actions:\n");
			fprintf(stderr, "   1. Remove all the floating input and associated gates, or\n");
			fprintf(stderr, "   2. Make each floating input a primary output.\n");
			return(-1);
		}
		
		net[cg->index] = cg;
		nog++;

    }
   
   
	for (i = nog; i<net_size; i++) net[i] = (GATEPTR)NULL;

    if (nog != int_nog) {
		fprintf(stderr,"Error in read_circuit\n");
		return(-1);
    }
    

	for (i = 0; i<nog; i++) {
		cg = net[i];

		for (j = 0; j<cg->ninput; j++) cg->inlis[j]->noutput++;

		if (cg->fn == PI) primaryin[nopi++] = i; 
    }
    
    
    for (i = 0; i<int_nopo; i++) primaryout[nopo++] = po_gates[i]->index;
    
    
    for (i = 0; i<nog; i++) {
		cg = net[i];
		if (cg->noutput>0) {
		    cg->outlis = (GATEPTR *)xmalloc(cg->noutput*(sizeof(GATEPTR)));	 
    	    //maxfout = MAX(maxfout,cg->noutput);
    	    //cg->noutput= 0;
        }
    }


    for (i = 0; i<nog; i++) net[i]->noutput = 0;

    
	for (i = 0; i<nog; i++) {
		cg = net[i];

		for (j = 0; j<cg->ninput; j++) {
			cg->inlis[j]->outlis[(cg->inlis[j]->noutput)++] = cg;
			//pg = cg->inlis[j];
			//pg->outlis[pg->noutput++] = cg;
		}
	}
		
	
	for (i = 0; i<nog; i++) {
		cg = net[i];
		if (cg->noutput > 0) continue;
		
		for (j = 0; j<int_nopo; j++)
			if (cg == po_gates[j]) break;

		if (j == int_nopo) {
			fprintf(stderr, "Error: floating output '%s' detected!\n", cg->symbol->symbol); 
			nerrs++;
		}
	}
	 
   
	if (nerrs > 0) {
		fprintf(stderr, "Workaround. You have to take one of the two actions:\n");
		fprintf(stderr, "   1. Remove all the floating output and associated gates, or\n");
		fprintf(stderr, "   2. Make each floating output a primary output.\n");
		return(-1);
	}

	if (int_nog == nog) return(nog);
	else return(-1);
}

int gatetype (char *symbol)
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

#include "define.h"


int add_PO () {
	
	register int i, j;
	GATEPTR gut, last, *outlist;
	char name[MAXSTRING];
	
	for (i = 0; i<nopo; i++) {
		gut = net[primaryout[i]];
		
		if ((last = net[nog]) == NULL) last = (GATETYPE *)xmalloc(sizeof(GATETYPE)); 
		
		last->index = nog; 
		last->fn = PO;
		last->ninput = 1;
		last->inlis = (GATEPTR *)xmalloc(sizeof(GATEPTR));
		last->inlis[0] = gut;
		last->noutput = 0;
		last->outlis = NULL;
		
		strcpy(name,gut->symbol->symbol);
        strcat(name,"_PO");
        
        while ((last->symbol = FindHash (symbol_tbl,HASHSIZE,name,0)) != NULL) {
			strcat(name,"_PO"); 
			printf("There is already a _PO gate with the same name!! why?\n");
		}

		if ((last->symbol = InsertHash (symbol_tbl,HASHSIZE,name,0)) == NULL) {
			fprintf(stderr,"Fatal error: Error in hash table \n");
			exit(1);
		}
        else last->symbol->pnode = last;
        
		outlist=gut->outlis;
		gut->outlis = (GATEPTR *)xmalloc((gut->noutput+1)*sizeof(GATEPTR));
		for (j = 0; j<gut->noutput; j++) gut->outlis[j] = outlist[j];
        
        gut->outlis[gut->noutput]=last;
        gut->noutput+=1;
		
		primaryout[i] = nog;
        net[nog++] = last;
    }
    
    /* Memory data check */
    /*for (i = 0; i<nog; i++) {
		gut = net[i];
		printf(" einai h pulh= %s me fanin= %d kai fanout= %d fn=%d \n",gut->symbol->symbol,gut->ninput,gut->noutput,gut->fn);
		for (j = 0; j<gut->ninput; j++) {
			printf("ta fanin ths einai oi %s\n",gut->inlis[j]->symbol->symbol);	}
	    for (j = 0; j<gut->noutput; j++) {
			printf("ta fanout ths einai oi %s\n",gut->outlis[j]->symbol->symbol); }
		printf("\n");
	} */
    
	return (nopo);
	
}

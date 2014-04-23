#include "define.h"

/* macros for LIFO stack operation */
#define EMPTY (-1)
#define push(s,ele) s.list[++(s.last)]=ele
#define pop(s) s.list[(s.last)--]
#define clear(s) s.last=EMPTY
#define is_empty(s) (s.last<0)
#define delete_last(s) --(s.last)
#define delete(s,i) s.list[i] = s.list[(s.last)--]
#define copy(s1,s2,i) s2.last = s1.last;\
		      for (i =s1.last; i>=0; i--) s2.list[i]=s1.list[i]

STACKTYPE stack1, stack2;
int maxlevel;


void allocate_stacks () 
{
	stack1.list = (GATEPTR *)xmalloc(nog*sizeof(GATEPTR));
	stack2.list = (GATEPTR *)xmalloc(nog*sizeof(GATEPTR));
	clear(stack1);
	clear(stack2);	
} 


int add_PO () 
{	
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


int compute_level () 
{
	register int i, j, flag=1;
	GATEPTR cg,ng;	
	
	for (i = 0; i<nog; i++) {
		cg = net[i];
		if(cg->fn == PI) {
			cg->level = 0;
			push(stack1,net[i]);
			cg->changed = cg->ninput;
		} 
		else {
			cg->level = (-1);
			cg->changed = 0;
		} 

    }
	
	
	while (TRUE) {
		
		if (flag == 1) while (!is_empty(stack1)) {
			cg = pop(stack1);

			for (i = 0; i<cg->noutput; i++) {
				ng = cg->outlis[i];
				if (++ng->changed == ng->ninput) {
					ng->level = cg->level+1;
					push(stack2,ng);
				}
			}
			
		}
		else if (flag == 2) while(!is_empty(stack2)) {
			cg = pop(stack2);

			for (i = 0; i<cg->noutput; i++) {
				ng = cg->outlis[i];
				if (++ng->changed == ng->ninput) {
					ng->level = cg->level+1;
					push(stack1,ng);
				}
			}

		}
		
		flag = (flag == 1) ? 2 : 1;
		if (is_empty(stack1) && is_empty(stack2)) break; 
	}
	
	maxlevel = (-1);
	for (i = 0; i<nog; i++) {
		cg = net[i];
		if ( cg->level > maxlevel) maxlevel = cg->level;
	}
	
	/* Memory data check */
    for (i = 0; i<nog; i++) {
		cg = net[i];
		printf(" einai h pulh= %s me fanin= %d kai fanout= %d fn=%d level=%d \n",cg->symbol->symbol,cg->ninput,cg->noutput,cg->fn,cg->level);
		for (j = 0; j<cg->ninput; j++) {
			printf("ta fanin ths einai oi %s\n",cg->inlis[j]->symbol->symbol);	}
	    for (j = 0; j<cg->noutput; j++) {
			printf("ta fanout ths einai oi %s\n",cg->outlis[j]->symbol->symbol); }
		printf("\n");
	} 
	
	return (maxlevel+1);
	
}



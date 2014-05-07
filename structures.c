#include "define.h"

/* macros for LIFO stack operation */
#define EMPTY (-1)
#define push(s,ele) s.list[++(s.last)]=ele
#define pop(s) s.list[(s.last)--]
#define clear(s) s.last=EMPTY
#define is_empty(s) (s.last<0)



STACKTYPE stack1, stack2;
STACKPTR event_list;
int maxlevel;
int *levels;


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
        
	return (nopo);
	
}


int compute_level () 
{
	register int i, flag=1;
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

	return (maxlevel+1);
	
}
///////////////////////////////place the PO at the last level
void place_PO () 
{
	register int i;
	GATEPTR cg;	
	
	for (i = 0; i<nog; i++) {
		cg = net[i];
		if(cg->fn == PO) {
			cg->level = maxlevel-1 ;
		} 
    }
	
	
	
	
}
///////////////////////////////

void allocate_event_list()
{
    register int i;

	event_list = (STACKTYPE *)xmalloc((maxlevel)*sizeof(STACKTYPE));
	for (i = 0; i<maxlevel; i++) clear(event_list[i]);

	for (i = 0; i<nog; i++) event_list[net[i]->level].last++;
	
	for(i=0;i<maxlevel;i++) {
		event_list[i].list = (GATEPTR *)xmalloc((event_list[i].last+1)*sizeof(GATEPTR));
		clear(event_list[i]);
	}
}


void levelize()
{
	register int i, j, new=0;
	GATEPTR cg;
	
	levels = (int *)xmalloc(maxlevel*(sizeof(int)));

	for (i = 0; i<nog; i++) push(event_list[net[i]->level],net[i]);
	

	for (i =0 ; i<maxlevel; i++) {
		levels[i] = event_list[i].last+1;
		//printf("oi pules epipedou %d einai %d\n",i,event_list[i].last+1);
		//printf("oi pules epipedou %d einai %d\n",i,levels[i]);
		for (j = 0; j<=event_list[i].last; j++) {	
			cg = event_list[i].list[j];
			cg->index = new++;
		}
		//clear(event_list[i]);
	}
    
    
	for (i = 0; i<nopi; i++)	 primaryin[i] = net[primaryin[i]]->index;
	
	for (i = 0; i<nopo; i++)  primaryout[i] = net[primaryout[i]]->index;

	i = 0;
	while (i<nog) {
		if (i == net[i]->index) i++; 
		else {		
			cg = net[i];
			net[i] = net[cg->index];
			net[cg->index] = cg;
		}
	}
	
	
	
	/* Memory data check 
	for (i = 0; i<nog; i++) {
		cg = net[i];
		printf("\nseira i=%d index=%d\n",i,cg->index);
		printf(" einai h pulh= %s me fanin= %d kai fanout= %d fn=%d level=%d \n",cg->symbol->symbol,cg->ninput,cg->noutput,cg->fn,cg->level);
		for (j = 0; j<cg->ninput; j++) {
			printf("ta fanin ths einai oi %s\n",cg->inlis[j]->symbol->symbol);	}
	    for (j = 0; j<cg->noutput; j++) {
			printf("ta fanout ths einai oi %s\n",cg->outlis[j]->symbol->symbol); }
	}*/
	
	
}

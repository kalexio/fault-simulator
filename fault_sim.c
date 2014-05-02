#include "define.h"
#include "structs.h"

FAULTPTR fault_list;


void create_fault_list ()
{
	int i;
	int j = 0;
	
	fault_list = (FAULTYPE *)xmalloc(2*(nog-levels[maxlevel-1])*sizeof(FAULTYPE));
	//printf("%d",event_list[0].last+1);
	
	for (i= 0; i<2*(nog-levels[maxlevel-1]); i+=2) {
		fault_list[i].gate = net[j];
		fault_list[i].SA = 0;
		fault_list[i+1].gate = net[j];
		fault_list[i+1].SA = 1;
		j++;		
	}
	
}



void print_fault_list ()
{
	int i;
	for (i= 0; i<2*(nog-levels[maxlevel-1]); i++) 
		printf("%s / %d \n",fault_list[i].gate->symbol->symbol,fault_list[i].SA);
}

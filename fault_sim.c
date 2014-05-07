#include "define.h"
#include "structs.h"

FAULTPTR fault_list;
int total_faults;

void fault_sim () 
{
	GATEPTR cg;
	int n, i, j, k, l;
	
	if ((test_fd = fopen(test_name,"a")) == NULL){
	    fprintf(stderr,"Fatal error: %s file open error\n",test_name);
		exit(0);
	} 
	
	allocate_and_init_faults ();
	for (n = 0; n<total_faults; n++) {
		set_injection_bits(fault_list[n]);

		for (i = 0 ; i<maxlevel; i++) {
			for (j = 0; j<=event_list[i].last; j++) {	
				cg = event_list[i].list[j];
				for ( k = 0; k<patterns; k++) {
					//if (i == 0) {
						cg->result[k].output = fault_eval(cg->faultData[k]); //υπολογισμος εξοδου πυλης 
						for ( l = 0; l<cg->noutput; l++){
							cg->outlis[l]->faultData[k].input[cg->outlis[l]->faultData[k].count] = cg->result[k].output;
							cg->outlis[l]->faultData[k].count++;
						}
						//prepei na kateuthinoume ta dedomena stis outlis
					//}
					//else {
						//cg->result[k].output = gate_eval(cg->threadData[k]);
						//gate evaluation + na kateuthinoyme ta dedomena stis outlis
					//}
				}
			}
		}
		fprintf(test_fd,"==========================================================\n");	
		fprintf(test_fd,"fault at gate=%s stack-at=%d\n",fault_list[n].gate->symbol->symbol,fault_list[n].SA);
		print_fault_sim (test_fd);
		//clean the counts
		for (i = 0; i<nog; i++) {
			for ( j = 0; j<patterns; j++) 
				net[i]->faultData[j].count = 0;
		}	
		
	}
	
	fclose(test_fd);
	
}


void print_fault_sim (FILE *f)
{
	int i, j;
	
	
	fprintf(f,"\n");

	for (i = 0; i<patterns; i++) {
		for (j = 0; j<nopo; j++) {
			fprintf(f,"%d",net[primaryout[j]]->result[i].output);
		}
		fprintf(f,"\n");
	}	
}






int fault_eval (THREADFAULTYPE data)
{
	int index,out;

	index = data.offset + data.input[0]*1 + data.input[1]*2 + data.input[2]*4 + data.input[3]*8;
	out = (LUT[index] & data.m0) || data.m1;
	
	return out;
} 




void set_injection_bits(FAULTYPE fault) 
{	
	int i, j;
	
	for (i = 0; i<nog; i++) {
		if ( fault.gate->symbol->symbol == net[i]->symbol->symbol) {
			if ( fault.SA == 0) {
				for ( j = 0; j<patterns; j++) {
					net[i]->faultData[j].m0 = 0;
					net[i]->faultData[j].m1 = 0;
				}
			}
			else if (fault.SA ==1) {
				for ( j = 0; j<patterns; j++) {
					net[i]->faultData[j].m0 = 1;
					net[i]->faultData[j].m1 = 1;
				}				
			}
		}
		else {
			for ( j = 0; j<patterns; j++) {
				net[i]->faultData[j].m0 = 1;
				net[i]->faultData[j].m1 = 0;
			}		
		}
    }	
	
	
}

void allocate_and_init_faults () 
{
	int i,j;
	
	//allocates memory for the faultdata struct of each gate
	for (i = 0; i<nog; i++) {
		net[i]->faultData = (THREADFAULTYPE *)xmalloc(patterns*sizeof(THREADFAULTYPE));
		//net[i]->result = (RESULTYPE *)xmalloc(patterns*sizeof(RESULTYPE));
		for ( j = 0; j<patterns; j++) {
			if ((net[i]->fn == PI) || (net[i]->fn == PO) || (net[i]->fn == NOT))
				net[i]->faultData[j].offset = net[i]->fn;
			else
				net[i]->faultData[j].offset = find_offset (net[i]);
		}
    }
    
    
	//puts the pattern values into the thread structs
	for (i = 0; i<nog; i++) {
		if (net[i]->level == 0) {
			for ( j = 0; j<patterns; j++) {
				net[i]->faultData[j].input[0] = test_set[j][i] - '0';
				net[i]->faultData[j].input[1] = 0;
				net[i]->faultData[j].input[2] = 0;
				net[i]->faultData[j].input[3] = 0;
			}
		}
		else break;
	}
	
	
	/*Memory checks 
	printf("Gates fn  data\n");
	for (i = 0; i<nog; i++) {
		if (net[i]->level == 0) {
			printf("%s  %d ",net[i]->symbol->symbol,net[i]->fn);
			for (j = 0; j<patterns; j++) {
				printf("%d",net[i]->faultData[j].input[0]);
			}
			printf("\n");
		}
	} */
	
}


void create_fault_list ()
{
	int i;
	int j = 0;
	
	total_faults = 2*(nog-levels[maxlevel-1]);
	printf("total fault =%d\n",total_faults);
	
	fault_list = (FAULTYPE *)xmalloc(total_faults*sizeof(FAULTYPE));
	//printf("%d",event_list[0].last+1);
	
	for (i= 0; i<total_faults; i+=2) {
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
	for (i= 0; i<total_faults; i++) 
		printf("%s / %d\n",fault_list[i].gate->symbol->symbol,fault_list[i].SA);
}



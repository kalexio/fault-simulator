#include "define.h"
#include "structs.h"

#define EMPTY (-1)
#define push(s,ele) s.list[++(s.last)]=ele
#define pop(s) s.list[(s.last)--]
#define clear(s) s.last=EMPTY

void logic_sim ()
{
	GATEPTR cg;
	int i, j, k, l;
	
	for (i = 0 ; i<maxlevel; i++) {
		for (j = 0; j<=event_list[i].last; j++) {	
			cg = event_list[i].list[j];
			for ( k = 0; k<patterns; k++) {
				//if (i == 0) {
					cg->result[k].output = gate_eval(cg->threadData[k]);
					for ( l = 0; l<cg->noutput; l++){
						cg->outlis[l]->threadData[k].input[cg->outlis[l]->threadData[k].count] = cg->result[k].output;
						cg->outlis[l]->threadData[k].count++;
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
	
	
	/*Memory checks */
	printf("Gates \tfn    \tinput     \t   output\n");
	for (i = 0; i<nog; i++) {
		printf("%s %d    ",net[i]->symbol->symbol,net[i]->fn);
		for (j = 0; j<patterns; j++) {
			if (net[i]->level == 0)
				printf("%d||",net[i]->threadData[j].input[0]);
			else {
				for (k = 0; k<net[i]->ninput; k++) 
					printf("%d",net[i]->threadData[j].input[k]);
				    printf("||");
				}
		}
		printf("\t");
		for (j = 0; j<patterns; j++) {
			printf("%d||",net[i]->result[j].output);
		}		
		printf("\n");
	} 
	
	/*printf("\nGates fn data           value\n");
	for (i = 0; i<nog; i++) {
		if (net[i]->level == 1) {
			printf("%s  %d ",net[i]->symbol->symbol,net[i]->fn);
			for (j = 0; j<patterns; j++) {
				for (k = 0; k<net[i]->ninput; k++) 
					printf("%d",net[i]->threadData[j].input[k]);
				printf("||");
			}
			printf("\t");
			for (j = 0; j<patterns; j++) {
				printf("%d||",net[i]->result[j].output);
			}
			printf("\n");
		}
	} */
	
	
}


int gate_eval (THREADTYPE data)
{
	int index,out;

	index = data.offset + data.input[0]*1 + data.input[1]*2 + data.input[2]*4 + data.input[3]*8;
	out = LUT[index];
	
	return out;
} 

int* create_lut (int * LUT)
{
	LUT = xmalloc(118*sizeof(int));
	//PI
	LUT[0] = 0;
	LUT[1] = 1;
	//PO
	LUT[2] = 0;
	LUT[3] = 1;
	//2-AND
	LUT[4] = 0;
	LUT[5] = 0;
	LUT[6] = 0;
	LUT[7] = 1;
	//3-AND
	LUT[8] = 0;
	LUT[9] = 0;
	LUT[10] = 0;
	LUT[11] = 0;
	LUT[12] = 0;
	LUT[13] = 0;
	LUT[14] = 0;
	LUT[15] = 1;
	//4-AND
	LUT[16] = 0;
	LUT[17] = 0;
	LUT[18] = 0;
	LUT[19] = 0;
	LUT[20] = 0;
	LUT[21] = 0;
	LUT[22] = 0;
	LUT[23] = 0;
	LUT[24] = 0;
	LUT[25] = 0;
	LUT[26] = 0;
	LUT[27] = 0;
	LUT[28] = 0;
	LUT[29] = 0;
	LUT[30] = 0;
	LUT[31] = 1;
	//2-NAND
	LUT[32] = 1;
	LUT[33] = 1;
	LUT[34] = 1;
	LUT[35] = 0;
	//3-NAND
	LUT[36] = 1;
	LUT[37] = 1;
	LUT[38] = 1;
	LUT[39] = 1;
	LUT[40] = 1;
	LUT[41] = 1;
	LUT[42] = 1;
	LUT[43] = 0;
	//4-NAND
	LUT[44] = 1;
	LUT[45] = 1;
	LUT[46] = 1;
	LUT[47] = 1;
	LUT[48] = 1;
	LUT[49] = 1;
	LUT[50] = 1;
	LUT[51] = 1;
	LUT[52] = 1;
	LUT[53] = 1;
	LUT[54] = 1;
	LUT[55] = 1;
	LUT[56] = 1;
	LUT[57] = 1;
	LUT[58] = 1;
	LUT[59] = 0;
	//2-OR
	LUT[60] = 0;
	LUT[61] = 1;
	LUT[62] = 1;
	LUT[63] = 1;
	//3-OR
	LUT[64] = 0;
	LUT[65] = 1;
	LUT[66] = 1;
	LUT[67] = 1;
	LUT[68] = 1;
	LUT[69] = 1;
	LUT[70] = 1;
	LUT[71] = 1;
	//4-OR
	LUT[72] = 0;
	LUT[73] = 1;
	LUT[74] = 1;
	LUT[75] = 1;
	LUT[76] = 1;
	LUT[77] = 1;
	LUT[78] = 1;
	LUT[79] = 1;
	LUT[80] = 1;
	LUT[81] = 1;
	LUT[82] = 1;
	LUT[83] = 1;
	LUT[84] = 1;
	LUT[85] = 1;
	LUT[86] = 1;
	LUT[87] = 1;
	//2-NOR
	LUT[88] = 1;
	LUT[89] = 0;
	LUT[90] = 0;
	LUT[91] = 0;
	//3-NOR
	LUT[92] = 1;
	LUT[93] = 0;
	LUT[94] = 0;
	LUT[95] = 0;
	LUT[96] = 0;
	LUT[97] = 0;
	LUT[98] = 0;
	LUT[99] = 0;
	//4-NOR
	LUT[100] = 1;
	LUT[101] = 0;
	LUT[102] = 0;
	LUT[103] = 0;
	LUT[104] = 0;
	LUT[105] = 0;
	LUT[106] = 0;
	LUT[107] = 0;
	LUT[108] = 0;
	LUT[109] = 0;
	LUT[110] = 0;
	LUT[111] = 0;
	LUT[112] = 0;
	LUT[113] = 0;
	LUT[114] = 0;
	LUT[115] = 0;
	//NOT
	LUT[116] = 1;
	LUT[117] = 0;
	
	return LUT;
}

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
					cg->result[k].output = gate_eval(cg->threadData[k]); //υπολογισμος εξοδου πυλης
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


	/*Memory checks
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
	} */

}




void print_logic_sim ()
{
	int i, j;

	FILE *f;


   if ((f = fopen(test_name,"w")) == NULL){
	        fprintf(stderr,"Fatal error: %s file open error\n",test_name);
      exit(0);
   }

	fprintf(f,"\nPrinting the logic sim results\n\n");

	for (i = 0; i<patterns; i++) {
		for (j = 0; j<levels[0]; j++) {
			fprintf(f,"%d",net[primaryin[j]]->threadData[i].input[0]);
		}
		fprintf(f," --> ");
		for (j = 0; j<nopo; j++) {
			fprintf(f,"%d",net[primaryout[j]]->result[i].output);
		}
		fprintf(f,"\n");
	}
	fprintf(f,"\nEnd of logic simulation \n\n");


	fclose(f);

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
	LUT = xmalloc(182*sizeof(int));
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
	//5-AND
	LUT[32] = 0;
	LUT[33] = 0;
	LUT[34] = 0;
	LUT[35] = 0;
	LUT[36] = 0;
	LUT[37] = 0;
	LUT[38] = 0;
	LUT[39] = 0;
	LUT[40] = 0;
	LUT[41] = 0;
	LUT[42] = 0;
	LUT[43] = 0;
	LUT[44] = 0;
	LUT[45] = 0;
	LUT[46] = 0;
	LUT[47] = 0;
	LUT[48] = 0;
	LUT[49] = 0;
	LUT[50] = 0;
	LUT[51] = 0;
	LUT[52] = 0;
	LUT[53] = 0;
	LUT[54] = 0;
	LUT[55] = 0;
	LUT[56] = 0;
	LUT[57] = 0;
	LUT[58] = 0;
	LUT[59] = 0;
	LUT[60] = 0;
	LUT[61] = 0;
	LUT[62] = 0;
	LUT[63] = 1;
	//2-NAND
	LUT[64] = 1;
	LUT[65] = 1;
	LUT[66] = 1;
	LUT[67] = 0;
	//3-NAND
	LUT[68] = 1;
	LUT[69] = 1;
	LUT[70] = 1;
	LUT[71] = 1;
	LUT[72] = 1;
	LUT[73] = 1;
	LUT[74] = 1;
	LUT[75] = 0;
	//4-NAND
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
	LUT[88] = 1;
	LUT[89] = 1;
	LUT[90] = 1;
	LUT[91] = 0;
	//2-OR
	LUT[92] = 0;
	LUT[93] = 1;
	LUT[94] = 1;
	LUT[95] = 1;
	//3-OR
	LUT[96] = 0;
	LUT[97] = 1;
	LUT[98] = 1;
	LUT[99] = 1;
	LUT[100] = 1;
	LUT[101] = 1;
	LUT[102] = 1;
	LUT[103] = 1;
	//4-OR
	LUT[104] = 0;
	LUT[105] = 1;
	LUT[106] = 1;
	LUT[107] = 1;
	LUT[108] = 1;
	LUT[109] = 1;
	LUT[110] = 1;
	LUT[111] = 1;
	LUT[112] = 1;
	LUT[113] = 1;
	LUT[114] = 1;
	LUT[115] = 1;
	LUT[116] = 1;
	LUT[117] = 1;
	LUT[118] = 1;
	LUT[119] = 1;
	//5-0R
	LUT[120] = 0;
	LUT[121] = 1;
	LUT[122] = 1;
	LUT[123] = 1;
	LUT[124] = 1;
	LUT[125] = 1;
	LUT[126] = 1;
	LUT[127] = 1;
	LUT[128] = 1;
	LUT[129] = 1;
	LUT[130] = 1;
	LUT[131] = 1;
	LUT[132] = 1;
	LUT[133] = 1;
	LUT[134] = 1;
	LUT[135] = 1;
	LUT[136] = 1;
	LUT[137] = 1;
	LUT[138] = 1;
	LUT[139] = 1;
	LUT[140] = 1;
	LUT[141] = 1;
	LUT[142] = 1;
	LUT[143] = 1;
	LUT[144] = 1;
	LUT[145] = 1;
	LUT[146] = 1;
	LUT[147] = 1;
	LUT[148] = 1;
	LUT[149] = 1;
	LUT[150] = 1;
	LUT[151] = 1;
	//2-NOR
	LUT[152] = 1;
	LUT[153] = 0;
	LUT[154] = 0;
	LUT[155] = 0;
	//3-NOR
	LUT[156] = 1;
	LUT[157] = 0;
	LUT[158] = 0;
	LUT[159] = 0;
	LUT[160] = 0;
	LUT[161] = 0;
	LUT[162] = 0;
	LUT[163] = 0;
	//4-NOR
	LUT[164] = 1;
	LUT[165] = 0;
	LUT[166] = 0;
	LUT[167] = 0;
	LUT[168] = 0;
	LUT[169] = 0;
	LUT[170] = 0;
	LUT[171] = 0;
	LUT[172] = 0;
	LUT[173] = 0;
	LUT[174] = 0;
	LUT[175] = 0;
	LUT[176] = 0;
	LUT[177] = 0;
	LUT[178] = 0;
	LUT[179] = 0;
	//NOT
	LUT[180] = 1;
	LUT[181] = 0;

	return LUT;
}

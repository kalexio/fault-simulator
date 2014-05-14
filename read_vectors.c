#include "define.h"
#include "structs.h"

#define is_delimiter(c) (c == ':')
#define is_comment(c) (c=='#')

char **test_set;
int patterns;


int read_vectors (FILE *vectors_fd,const char* vectors_name) 
{
	register char c;
    char symbol[levels[0]];
    //int lines = 0;
    //char buf[MAXSTRING];
    patterns = 0;

    
    while ((c = getvector (vectors_fd, symbol)) != EOF) {
		//printf("len %d\n",strlen(symbol));
		//printf("symbol = %s\n",symbol);
		patterns++;
	}
    
    test_set = (char **)xmalloc(patterns*sizeof(char *));
    
    fclose(vectors_fd);
    vectors_fd = fopen (vectors_name, "r");
	if (vectors_fd == NULL)
		system_error ("fopen");
    
    
    //reads the file into test_set
    patterns = 0;
    //printf(" patterns %d\n",patterns);
	while ((c = getvector (vectors_fd, symbol)) != EOF) {
		//printf("here patterns %d\n",patterns);
		test_set[patterns] = xmalloc((levels[0]+1)*sizeof(char));
		strcpy(test_set[patterns],symbol);
		//printf("symbol = %s\n",test_set[patterns]);
		patterns++;
	} 
	printf("End patterns %d\n",patterns);
	allocate_and_init ();
 
	return 0;
	
}




void allocate_and_init () 
{
	int i,j;
	
	//allocates memory for the threaddata result struct of each gate
	for (i = 0; i<nog; i++) {
		net[i]->threadData = (THREADTYPE *)xmalloc(patterns*sizeof(THREADTYPE));
		net[i]->result = (RESULTYPE *)xmalloc(patterns*sizeof(RESULTYPE));
		for ( j = 0; j<patterns; j++) {
			if ((net[i]->fn == PI) || (net[i]->fn == PO) || (net[i]->fn == NOT))
				net[i]->threadData[j].offset = net[i]->fn;
			else
				net[i]->threadData[j].offset = find_offset (net[i]);
		}
    }
    
    
	//puts the pattern values into the thread structs
	for (i = 0; i<nog; i++) {
		if (net[i]->level == 0) {
			for ( j = 0; j<patterns; j++) {
				net[i]->threadData[j].input[0] = test_set[j][i] - '0';
				net[i]->threadData[j].input[1] = 0;
				net[i]->threadData[j].input[2] = 0;
				net[i]->threadData[j].input[3] = 0;
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
				printf("%d",net[i]->threadData[j].input[0]);
			}
			printf("\n");
		}
	} */
	
	
}




int find_offset (GATEPTR cg)
{
	int inputs, offset, fn;
	
	inputs = cg->ninput;
	fn = cg->fn;
	
	switch (fn) {
		case AND:
			if (inputs == 2) offset = AND2;
			else if (inputs == 3) offset = AND3;
			else if (inputs == 4) offset = AND4;
			else if (inputs == 5) offset = AND5;
			break;
		case NAND:
			if (inputs == 2) offset = NAND2;
			else if (inputs == 3) offset = NAND3;
			else if (inputs == 4) offset = NAND4;
			break;
		case OR:
			if (inputs == 2) offset = OR2;
			else if (inputs == 3) offset = OR3;
			else if (inputs == 4) offset = OR4;
			else if (inputs == 5) offset = OR5;
			break;
		case NOR:
			if (inputs == 2) offset = NOR2;
			else if (inputs == 3) offset = NOR3;
			else if (inputs == 4) offset = NOR4;
			break;
	}
	
	return (offset);
	
}




char getvector (FILE* file, char* s)
{
	register char c;
    int flag = 0;
    int comm = 0;

    while ((c = getc (file)) != EOF) {
	    if(is_comment(c)) { comm=1; continue; }
		if(comm==1) {
			if(c=='\n') comm=0;
			continue;
		}
		if (!flag) {
			if (is_delimiter(c)) flag = 1;
			continue;
		}
		else {
			if ( c != '\n') {
				*s++ = c;
			}
			else if ( c == '\n') {
				flag = 0;
				break;
			}
		}
	}
	
	*s = '\0';
    return(c);
}

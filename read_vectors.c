#include "define.h"
#include "structs.h"

#define is_delimiter(c) (c == ':')

char **test_set;

int read_vectors (FILE *vectors_fd,const char* vectors_name) 
{
	register char c;
    char symbol[levels[0]];
    int lines = 0;
    char buf[MAXSTRING];
    int patterns = 0;

     
    //finds the number of lines , that is all the vectors 
    while (fgets(buf,sizeof(buf),vectors_fd) != NULL) lines++;
    fclose(vectors_fd);
    test_set = xmalloc(lines*sizeof(char *));
    
    vectors_fd = fopen (vectors_name, "r");
	if (vectors_fd == NULL)
		system_error ("fopen");
    
    //reads the file into test_set
	while ((c = getvector (vectors_fd, symbol)) != EOF) {
		test_set[patterns] = xmalloc(levels[0]*sizeof(char));
		strcpy(test_set[patterns],symbol);
		patterns++;
	}
	
	allocate_and_init (patterns);
 
	return 0;
	
}



void allocate_and_init (int patterns) 
{
	int i,j;
	
		//allocates memory for the threaddata struct of each gate
	for (i = 0; i<nog; i++) {
		net[i]->threadData =(THREADTYPE *)xmalloc(patterns*sizeof(THREADTYPE));
		
		//allocates memory for the pointer int of each thread data struct
		if (net[i]->level == 0) {
			for ( j = 0; j<patterns; j++) {
				net[i]->threadData[j].input = xmalloc(1*sizeof(int));
				net[i]->threadData[j].offset = net[i]->fn;
			}
		} 
		else {
			for ( j = 0; j<patterns; j++) {
				net[i]->threadData[j].input = xmalloc(net[i]->ninput*sizeof(int));
				net[i]->threadData[j].offset = net[i]->fn;
			}
		}
    }
    
	//puts the pattern values into the thread structs
	for (i = 0; i<nog; i++) {
		if (net[i]->level == 0) {
			for ( j = 0; j<patterns; j++) {
				net[i]->threadData[j].input[0] = test_set[j][i] - '0';
			}
		}
		else break;
	}
	
	/*Memory checks */
	printf("Gates fn  data\n");
	for (i = 0; i<nog; i++) {
		if (net[i]->level == 0) {
			printf("%s  %d ",net[i]->symbol->symbol,net[i]->fn);
			for (j = 0; j<patterns; j++) {
				printf("%d",net[i]->threadData[j].input[0]);
			}
			printf("\n");
		}
	}
	
}



char getvector (FILE* file, char* s)
{
	register char c;
    int flag = 0;

    while ((c = getc (file)) != EOF) {
		
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

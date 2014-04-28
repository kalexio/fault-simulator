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
    int i,j;
     
    //finds the number of lines , that is all the vectors 
    while (fgets(buf,sizeof(buf),vectors_fd) != NULL) lines++;
    fclose(vectors_fd);
    test_set = xmalloc(lines*sizeof(char *));
    
    vectors_fd = fopen (vectors_name, "r");
	if (vectors_fd == NULL)
		system_error ("fopen");
    
    //reads the file into test_set
	while ((c = getvector (vectors_fd, symbol)) != EOF) {
		//printf("symbol =%s\n",symbol);
		test_set[patterns] = xmalloc(levels[0]*sizeof(char));
		strcpy(test_set[patterns],symbol);
		//printf("test   =%c\n",test_set[patterns][1]);
		patterns++;
	}
	
	//-------------------------------------------------------------------------------------
	//allocates memory for the threaddata struct of each gate
	for (i = 0; i<nog; i++) {
		net[i]->threadData =(THREADTYPE *)xmalloc(patterns*sizeof(THREADTYPE));
		
		if (net[i]->level == 0) {
			//printf("einai h %s me level=%d kai ninput=%d\n",net[i]->symbol->symbol,net[i]->level,net[i]->ninput);
			for ( j = 0; j<patterns; j++) {
				net[i]->threadData[j].input = xmalloc(1*sizeof(int));
				net[i]->threadData[j].offset = net[i]->fn;
			}
		} 
		else {
			//printf("einai h %s me level=%d kai ninput=%d\n",net[i]->symbol->symbol,net[i]->level,net[i]->ninput);
			for ( j = 0; j<patterns; j++) {
				net[i]->threadData[j].input = xmalloc(net[i]->ninput*sizeof(int));
				net[i]->threadData[j].offset = net[i]->fn;
			}
		}
    }
    
	
	for (i = 0; i<nog; i++) {
		if (net[i]->level == 0) {
			for ( j = 0; j<patterns; j++) {
				net[i]->threadData[j].input[0] = test_set[j][i] - '0';
				printf("einai h pulh %s me level=%d kai stoixeio=%d\n",net[i]->symbol->symbol,net[i]->level,net[i]->threadData[j].input[0]);
				//printf("hello =%d\n", test_set[j][i]);
			}
		}
		else break;
	} 


	return 0;
	
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



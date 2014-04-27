#include "define.h"
#include "structs.h"


#define is_comment(c) (c == '#')
#define is_white_space(c) (c == ' ' || c == '\n' || c == '\t')
#define is_delimiter(c) (c == ':')

char **test_set;

int read_vectors (FILE *vectors_fd) 
{
	register char c;
    char symbol[levels[0]];
    int i = 0;
    
    
    test_set = xmalloc(MAXSTRING*sizeof(char *));
    
	while ((c = getvector (vectors_fd, symbol)) != EOF) {
		//printf("symbol =%s\n",symbol);
		test_set[i] = xmalloc(levels[0]*sizeof(char));
		strcpy(test_set[i],symbol);
		//printf("test   =%s\n",test_set[i]);
		i++;
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



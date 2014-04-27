#include "define.h"
#include "structs.h"


#define is_comment(c) (c == '#')
#define is_white_space(c) (c == ' ' || c == '\n' || c == '\t')
#define is_delimiter(c) (c == ':')




int read_vectors (FILE *vectors_fd, const char* vectors_name) {
	
	
	
	
	
	
	
	
	




 return 0;
	
}



void getvector (FILE* file, char* s)
{
   	register char c;
    int comm = 0;

    while ((c = getc (file)) != EOF) {
		if (is_comment(c)) { 
			comm = 1; 
			continue; 
		}
		if (comm == 1) {
			if(c == '\n') comm = 0;
			continue;
		}	
        if (is_white_space(c)) continue;
        if (is_delimiter(c)) break;
        *s++ = c;
    }
    *s = '\0';
}



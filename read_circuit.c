#include <stdio.h>
#include "define.h"
#include <string.h>

HASHPTR symbol_tbl[HASHSIZE];
GATEPTR begnet = (GATEPTR)NULL;

#define is_comment(c) (c == '#')
#define is_white_space(c) (c == ' ' || c == '\n' || c == '\t')
#define is_delimiter(c) (c == '=' || c == ',' || c == '(' || c == ')')

int read_circuit (FILE *circuit_fd, const char* circuit_name)
{
	char c;
	char symbol[MAXSTRING];
    InitHash (symbol_tbl, HASHSIZE);
    while( (c = getsymbol (circuit_fd, symbol)) != EOF) {
	  
    }
  
  
	return 0;
}

void InitHash (HASHPTR symbol_tbl[], int size) 
{
	while (--size >= 0) symbol_tbl[size] = NULL;
}

char getsymbol (FILE* file, char* s)
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
    return(c);
}

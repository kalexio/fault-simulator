#include "define.h"
#include "structs.h"


HASHPTR symbol_tbl[HASHSIZE];
GATEPTR begnet = (GATEPTR)NULL;



int read_circuit (FILE *circuit_fd, const char* circuit_name)
{
    char c;
    char symbol[MAXSTRING];
	register HASHPTR hp;
	
	
    InitHash (symbol_tbl, HASHSIZE);
    while( (c = getsymbol (circuit_fd, symbol)) != EOF) {
			switch (c) {
				case '=':
					hp=Find_and_Insert_Hash(symbol_tbl,HASHSIZE,symbol,0);
					printf("in read: hp =  %s \n", symbol);
					break;
				
			}
	 
    }
	return 0;
}



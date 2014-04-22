#include "define.h"
#include "structs.h"

#define EDIGIT 4
#define BASIS ('Z'-'A'+19)
#define is_comment(c) (c == '#')
#define is_white_space(c) (c == ' ' || c == '\n' || c == '\t')
#define is_delimiter(c) (c == '=' || c == ',' || c == '(' || c == ')')

void InitHash (HASHPTR symbol_tbl[], int size) 
{
	while (--size >= 0) symbol_tbl[size] = NULL;
}

HASHPTR Find_and_Insert_Hash (HASHPTR symbol_tbl[], int size, char symbol[] , int key)
{
	HASHPTR hp;
	
    //printf("in find and insert!\n");
	if (key == 0) key = keyvalue (symbol);
	
    //printf("key = %d\n",key);
    
	if ((hp = FindHash (symbol_tbl, size, symbol, key)) == NULL) {
		//printf( "i didnt find it\n");
	    hp=InsertHash(symbol_tbl,size,symbol,key);
    }
   return(hp);	
}

HASHPTR FindHash(HASHPTR symbol_tbl[], int size, char symbol[], int key)
{
	HASHPTR hp;
	int h;
    
    //printf("in find !\n");
    if (key == 0) key = keyvalue (symbol);
    //printf("key = %d\n",key);
    h = key % size;
    hp = symbol_tbl[h];

    while (hp != NULL) {
		if (key == hp->key)
			if (strcmp (hp->symbol, symbol) == 0) {
				 //printf(" i found it\n");
				 break;
		    }
        hp=hp->next; 
    }

    return(hp);
}

HASHPTR InsertHash (HASHPTR symbol_tbl[], int size, char symbol[], int key)
{
    register HASHPTR hp;
    register int h;

    //printf("in insert !\n");
    if (key ==0) key = keyvalue (symbol);
    //printf("key = %d\n",key);
    h = key % size;

    hp = hashalloc(); 
    //printf("Created!\n");
    
	hp->key = key;
	hp->next = symbol_tbl[h];//την πρωτη φορα θα δειξει σε NULL meta ayto den allazei
	hp->symbol = astrcpy (hp->symbol, symbol);
   
	symbol_tbl[h]=hp;

	return(hp);
	
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

int keyvalue(char *s)
{
	
    register char c;
    register int i, j, val=0;
    int multi=1;

    i=0;
    while ((c = s[i]) != '\0') {
		
        if (c >= '0' && c <= '9') j = c-'0'+1;
        else if (c >= 'a' && c <= 'z') j = c-'a'+11;
        else if (c >= 'A' && c <= 'Z') j = c-'A'+11;
        else if (c == '_') j = 'Z'-'A'+12;
        else if (c == '#') j = 'Z'-'A'+13;
        else if (c == '@') j = 'Z'-'A'+14;
        else if (c == '$') j = 'Z'-'A'+15;
        else if (c == '/') j = 'Z'-'A'+16;
        else if (c == '<' || c == '>') j = 'Z'-'A'+17;
        else if (c == '[' || c == ']') j = 'Z'-'A'+18;
        else j = 'Z'-'A'+18;
      
        val+=j*multi;
        i++;
        if(i % EDIGIT == 0) multi=1; else multi*=BASIS;
   }
   return(val);
}

HASHPTR hashalloc()
{
   HASHPTR hash;

   hash = (HASHTYPE *)xmalloc (sizeof(HASHTYPE)); 
   hash->key=0;
   hash->pnode=NULL;
   hash->symbol=NULL;
   hash->next=NULL;
   
   return(hash);
}

char *astrcpy(char *d, char *s)
{
    int length;

    length=strlen(s);
    d = (char *)xmalloc((length+1)*sizeof(char));
    strcpy(d,s);
    return(d);
}

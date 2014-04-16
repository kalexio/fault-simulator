/***********************************************************************
 *     Contains the structs that are used
 * ********************************************************************/
#ifndef STRUCTS_H 
#define STRUCTS_H

typedef struct HASH {
    int key;
    struct GATE *pnode;
    struct HASH *next;
    char *symbol;
} HASHTYPE, *HASHPTR;

typedef struct GATE {
    int fn;
    short ninput;
    struct GATE **inlis;
    short noutput;
    struct GATE **outlis;
    struct HASH *symbol;
    struct GATE *next;
} GATETYPE, *GATEPTR;




#endif

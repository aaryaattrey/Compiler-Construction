/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#ifndef PARSER_H
#define PARSER_H

#include "parserDef.h"

// function for storing grammar rules in the form of linked list
void makeGrammar(FILE* fp);

//function for parse table generation
void createParseTable();

// passing token to parser
void getNextToken();

//parsing using stack
void runPDA();

//error handling
void display_error();

//hash function for non-terminals and terminals
int get_hash(const char* s);

#endif


/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/

#ifndef LEXER_H
#define LEXER_H
#include "lexerDef.h"


//function prototypes

// tk_data has to be set according to tk_name
void tokenise(enum TOKEN tk_name);

// get the next character
char getnextchar(FILE *fp,char * buff1,char * buff2);

// populate the buffer
void getnextblock(FILE* fp,char * buff);

//check for keywords,AND,OR,true,false
int check_keyword(char *lexeme);

//error handling function
void error_handle();

//copying lexeme
void copy_lexeme(char * str);

//get the hash value for a string
int get_hash_lexer(char* str);
#endif

/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#ifndef LEXERDEF_H
#define LEXERDEF_H
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

//Token names
enum TOKEN {INTEGER,REAL,BOOLEAN,OF,ARRAY,START,END,DECLARE,MODULE,DRIVER,
PROGRAM,GET_VALUE,PRINT,USE,WITH,PARAMETERS,TAKES,INPUT,RETURNS,FOR,IN,SWITCH,
CASE,BREAK,DEFAULT,WHILE,AND,OR,TRUE,FALSE,ID,NUM,RNUM,PLUS,MINUS,MUL,DIV,
LT,LE,GE,GT,EQ,NE,DEF,ENDDEF,DRIVERDEF,DRIVERENDDEF,COLON,RANGEOP,SEMICOL,
COMMA,ASSIGNOP,SQBO,SQBC,BO,BC,COMMENTMARK};

char token_strings[57][30] = {"INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER",
"PROGRAM","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE","AND","OR","TRUE","FALSE","ID","NUM","RNUM","PLUS","MINUS","MUL","DIV",
"LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL",
"COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK"};

int token_found=0;
// union
union Variant{
    char lexeme[21];
    int val;
    double realVal;
};

//struct
typedef struct token_info{
    int line_no;
    enum TOKEN tk_name;
    union Variant tk_data;
    int hasError ;
} TK_INFO;

//global declarations

//global token
TK_INFO global_token;

// twin buffers
// #define buffer_size 512
// char buff1[buffer_size];
// char buff2[buffer_size];


// When getting buffer size from input
int buffer_size=30;
char *buff1;
char *buff2;

// pointers to the buffer
int forward=0,begin=0;
int do_not_refill=0;
 /*0: both pointers in buff1
 1: begin in buff1 and forward in buff2
 2: forward in buff1 and begin in buff2
 3: both in buff2*/
int flag = 0;

// stores current_line_no
int current_line_no=1;

//global variable for state for dfa
int state = 0;

//flag to stop and start generation of tokens
int generateToken=0;
int programHasLexicalError=0;

//hashtable
struct ht_entry{
    enum TOKEN tk;
    char str[11];
};


struct ht_entry hashTable[84];

#endif
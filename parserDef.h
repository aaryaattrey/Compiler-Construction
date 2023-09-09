/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#ifndef PARSERDEF_H
#define PARSERDEF_H
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#define HASH_MOD 1619
#define NUM_RULES 137
#define NON_TERM 72
#define TOTAL_TERM 130

//for storing grammar node
struct node
{
    int isTerminal;
    struct node* forward_link;
    struct node* backward_link;
    char value[23]; // token
}; 

//stores grammar rule along with rule number
struct ruleNode
{
    struct node* gNode;
    int ruleNo;
};

//both non-terminals and terminals
//non terminals = 72
char nonTerminals[130][24] = {"startprogram","program","moduleDeclarations","moduleDeclaration","otherModules",
"driverModule","module","ret","input_plist","moreList","output_plist","moreOutput",
"dataType","type","moduleDef","statements","statement","ioStmt","print_var","whichId2",
"boolvar","whichId","index","sign","aVar","var_id_num","newArithmeticExpr","startExpr",
"newA1","newA2","newTerm","newNextTerm","u1","after_u1","var_id_num2","simpleStmt","assignmentStmt",
"whichStmt","lvalueIDStmt","lvalueARRStmt","moduleReuseStmt","optional","idList",
"moreId","actualParameter","expression","arithmeticOrBooleanExpr","ab1","anyTerm","ab2","u","after_unary",
"arithmeticExpr","a1","term","a2","nextTerm","op1","op2","bop","relationalOp",
"declareStmt","conditionalStmt","caseStmt","post","value","default","iterativeStmt",
"range_for","range","idList2","moreId2","INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER",
"PROGRAM","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE","AND","OR","TRUE","FALSE","ID","NUM","RNUM","PLUS","MINUS","MUL","DIV",
"LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL",
"COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","DOLLAR"};

char terminals[58][13] = {"INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER",
"PROGRAM","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE","AND","OR","TRUE","FALSE","ID","NUM","RNUM","PLUS","MINUS","MUL","DIV",
"LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL",
"COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","DOLLAR"};

FILE* fp;

//stores lhs of rules
struct node* grammar[NUM_RULES];
//first and follow for all rules
char* firstAndFollow[NUM_RULES][NUM_RULES];
//parseTable
struct ruleNode parseTable[HASH_MOD][HASH_MOD];
//comma
const char* comma = ",";
//errorToken
int errorToken = 0;
int programHasParsingError = 0;
char ntFirst[HASH_MOD][300];  
char ntFollow[HASH_MOD][300]; 

int isEpsilon[HASH_MOD];

int driverFlag;
int driverFlag2;
#endif
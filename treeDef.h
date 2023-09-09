/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#ifndef TREEDEF_H
#define TREEDEF_H
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include "symbolTable.h"
// error ka karan ban sakta hai
#include "lexerDef.h"

struct treeNode
{
    int line_no;
    int visited; //1 if visited, 0 if not visited
    int entry_visited; // 1 means visited
    int while_visited;//1 means visited while expr
    char value[23]; //token name
    int isTerminal;
    struct treeNode* children;
    struct treeNode* parent;
    struct treeNode* nextSibling;
    struct treeNode* prevSibling;
    struct treeNode* astnextSibling; //ast's next sibling
    struct treeNode* astprevSibling; //ast's prev sibling
    union Variant tk_data;
    int ruleNo;  
    int isChildOfAssign; // 1 if child of assign, 0 if not
    struct treeNode* inh; //inherited attribute
    struct treeNode* syn; //synthesized attribute
    struct treeNode* addr; //address attribute
    struct treeNode* next; //next node in list_type
    struct treeNode* pair; //pair node in list_type 
    struct treeNode* type_syn; //syn attribute for type
    struct treeNode* type_inh; //inh attribute for type
    char* true_label; //true label for boolean expression
    char* false_label; //false label for boolean expression
    char* constructLabel; //label for every construct
    char* for_construct_label; //label for for's condition in order to jump
    char* for_increment_label; //label for for's increment operation
    int isDefault;
    ST_ENTRY* temp_variable_entry;  
    ST_ENTRY* symbol_table_entry; 
    FN_ENTRY* function_table_entry; 
};

struct treeNode* root = NULL;
struct treeNode* currExpand = NULL;
#endif
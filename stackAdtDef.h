/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#ifndef STACKADTDEF_H
#define STACKADTDEF_H

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>


struct stack_node
{
    int isTerminal;
    char value[23];
    struct stack_node* link;
};

typedef struct stack_node stackElement;

stackElement* s_top = NULL;

#endif
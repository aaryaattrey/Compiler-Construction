/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#include "tree.h"

void initTreeNode(struct treeNode* node){
    if(node==NULL)return;
    node->visited = 0;
    node->children=NULL;
    node->parent=NULL;
    node->nextSibling=NULL;
    node->prevSibling=NULL;
    node->astnextSibling=NULL;
    node->astprevSibling=NULL;
    node->inh=NULL;
    node->syn=NULL;
    node->addr = NULL;
    node->next=NULL;
    node->pair=NULL;
    node->type_syn=NULL;
    node->type_inh=NULL;
    node->symbol_table_entry=NULL;
    node->temp_variable_entry=NULL;
    node->function_table_entry=NULL;
    node->isChildOfAssign = 0;
    node->isDefault = 0;
}


void addChild(struct treeNode *parent, struct node *child)
{
    struct treeNode *temp = malloc(sizeof(struct treeNode));
    initTreeNode(temp);
    strcpy(temp->value, child->value);
    temp->visited = 0;
    temp->addr = NULL;
    temp->next = NULL;
    temp->pair = NULL;
    temp->astnextSibling = NULL;
    temp->astprevSibling = NULL;
    temp->symbol_table_entry = NULL;
    temp->temp_variable_entry = NULL;
    temp->function_table_entry = NULL;
    temp->type_inh = NULL;
    temp->type_syn = NULL;
    // if(!child->isTerminal)
    //     strcpy(temp->tk_data.lexeme," ");
    temp->isTerminal = child->isTerminal;
    if (temp->isTerminal)
        temp->children = NULL;
    temp->parent = parent;
    temp->prevSibling = NULL;
    parent->children = temp;
    child = child->forward_link;

    while (child != NULL)
    {
        struct treeNode *temp2 = malloc(sizeof(struct treeNode));
        initTreeNode(temp2);
        strcpy(temp2->value, child->value);
        if (!strcmp(child->value, "DOLLAR"))
            temp2->children = NULL;
        temp2->isTerminal = child->isTerminal;
        if (temp2->isTerminal)
            temp2->children = NULL;
        temp2->parent = parent;
        temp2->temp_variable_entry = NULL;
        temp2->function_table_entry = NULL;
        temp2->symbol_table_entry = NULL;
        temp2->visited = 0;
        temp2->type_inh = NULL;
        temp2->type_syn = NULL;
        temp2->prevSibling = temp;
        temp->nextSibling = temp2;
        temp = temp2;
        child = child->forward_link;
    }
    temp->nextSibling = NULL;
}

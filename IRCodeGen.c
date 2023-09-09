/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#include "IRCodeGen.h"
// #include "symbolTable.c

char *newlabel()
{
    char *lab = (char *)malloc(TEMP_LEN * sizeof(char));
    sprintf(lab, "L%d", label_count);
    label_count++;
    return lab;
}

void assignlabel(struct treeNode* root)
{
    if (root->constructLabel == NULL){
        if(!strcmp(root->value,"DECLARE") || !strcmp(root->value,"GET-VALUE") || !strcmp(root->value,"PRINT") || !strcmp(root->value,"WHILE") || !strcmp(root->value,"FOR") || !strcmp(root->value,"SWITCH") || (root->parent != NULL && !strcmp(root->parent->value,"SWITCH")) ||!strcmp(root->value,"LVALUEID") || !strcmp(root->value,"LVALUEARR") || !strcmp(root->value,"MODULE-INVOKE"))
            root->constructLabel = newlabel();
    }
}

ST_ENTRY *newTemp(TYPE t)
{
    //char *temp = (char *)malloc(TEMP_LEN * sizeof(char));
    char temp[TEMP_LEN];
    sprintf(temp, "t_%d", variable_count);
    variable_count++;
    struct treeNode *temp_node = malloc(sizeof(struct treeNode));
    strcpy(temp_node->tk_data.lexeme, temp);
    temp_node->addr = NULL;
    temp_node->astnextSibling = NULL;
    temp_node->astprevSibling = NULL;
    temp_node->children = NULL;
    temp_node->function_table_entry = NULL;
    temp_node->inh = NULL;
    temp_node->next = NULL;
    temp_node->nextSibling = NULL;
    temp_node->pair = NULL;
    temp_node->parent = NULL;
    temp_node->prevSibling = NULL;
    temp_node->syn = NULL;
    temp_node->type_syn = NULL;
    temp_node->type_inh = NULL;
    ST_ENTRY* temp_entry = create_entry_and_insert(temporaries_st, temp_node, t);  
    temp_entry->offset = global_max_offset;
    char* type = t.id_type.id_dt;
    if(!strcmp(type,"INTEGER"))
    {
        global_max_offset += 2;    
    }
    else if(!strcmp(type,"REAL"))
    {
        global_max_offset += 4;
    }
    else if(!strcmp(type,"BOOLEAN")){
        global_max_offset += 1;
    }
     
    return temp_entry;
}

// CHANGE TYPE ACCORDING TO PRECEDENCE
void operator_appears(struct treeNode *root)
{
    struct treeNode *leftChild = root->children;
    if (leftChild == NULL)
        return;
    struct treeNode *rightChild = root->children->astnextSibling;
    TYPE t;
    // for left-child
    
    if (leftChild->temp_variable_entry == NULL)
    {
        
        if (!strcmp(leftChild->value, "ID"))
        {
            quadTable[count].arg1.arg.arg_var = leftChild->symbol_table_entry;
            quadTable[count].arg1.entry = 0;
            if(leftChild->symbol_table_entry->is_array == 1)
                t.id_type.id_dt = leftChild->symbol_table_entry->type.arr_type.arr_dt;
            else if(leftChild->symbol_table_entry->is_for == 1)
                t.id_type.id_dt = "INTEGER";
            else
                t = leftChild->symbol_table_entry->type;
        }
        else if (!strcmp(leftChild->value, "NUM"))
        {
            quadTable[count].arg1.arg.arg_num = leftChild->tk_data.val;
            quadTable[count].arg1.entry = 1;
            t.id_type.id_dt = "INTEGER";
        }
        else if (!strcmp(leftChild->value, "RNUM"))
        {
            quadTable[count].arg1.arg.arg_rnum = leftChild->tk_data.realVal;
            quadTable[count].arg1.entry = 2;
            t.id_type.id_dt = "REAL";
        }
        else if (!strcmp(leftChild->value, "BOOLEAN"))
        {
            quadTable[count].arg1.arg.arg_bool = leftChild->tk_data.lexeme;
            quadTable[count].arg1.entry = 3;
            t.id_type.id_dt = "BOOLEAN";
        }
    }
    else
    {
        quadTable[count].arg1.arg.arg_var = leftChild->temp_variable_entry;
        quadTable[count].arg1.entry = 0;
        t = leftChild->temp_variable_entry->type;
    }

    // for right-child
    if (rightChild->temp_variable_entry == NULL)
    {
        if (!strcmp(rightChild->value, "ID")){
            quadTable[count].arg2.arg.arg_var = rightChild->symbol_table_entry;
            quadTable[count].arg2.entry = 0;
        }
        else if (!strcmp(rightChild->value, "NUM")){
            quadTable[count].arg2.arg.arg_num = rightChild->tk_data.val;
            quadTable[count].arg2.entry = 1;
        }
        else if (!strcmp(rightChild->value, "RNUM")) // RNUM case 
        {   
            quadTable[count].arg2.arg.arg_num = rightChild->tk_data.realVal;
            quadTable[count].arg2.entry = 2;
        }
        else if (!strcmp(rightChild->value, "BOOLEAN")){
            quadTable[count].arg2.arg.arg_bool = rightChild->tk_data.lexeme;
            quadTable[count].arg2.entry = 3;
        }
    }
    else
    {
        quadTable[count].arg2.arg.arg_var = rightChild->temp_variable_entry;
        quadTable[count].arg2.entry = 0;
    }

    // set operator
    strcpy(quadTable[count].op, root->value);

    ST_ENTRY *tempVarEntry = newTemp(t);
    quadTable[count].result.arg.arg_var = tempVarEntry;
    quadTable[count].result.entry = 0;
    root->temp_variable_entry = tempVarEntry; // set temp_variable entry of node
    count++;
}

void while_expression_appears(struct treeNode *root)
{
    if (!strcmp(root->value, "AND"))
    {
        if(root->children->astnextSibling->constructLabel == NULL)
            root->children->astnextSibling->constructLabel = newlabel();
        root->children->true_label = root->children->astnextSibling->constructLabel;
        root->children->false_label = root->false_label;
        root->children->astnextSibling->true_label = root->true_label;
        root->children->astnextSibling->false_label = root->false_label;
    }
    else if (!strcmp(root->value, "OR"))
    {
        if (root->children->astnextSibling->constructLabel == NULL)
            root->children->astnextSibling->constructLabel = newlabel();
        // assignlabel(root->children->astnextSibling);
        root->children->true_label = root->true_label;
        root->children->false_label = root->children->astnextSibling->constructLabel;
        root->children->astnextSibling->true_label = root->true_label;
        root->children->astnextSibling->false_label = root->false_label;
    }
}

void while_appears(struct treeNode *root)
{
    quadTable[count].goTolabel = root->constructLabel;
    quadTable[count].instruction = "GOTO";
    quadTable[count].arg1.arg.arg_var = NULL;
    quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_var = NULL;
    quadTable[count].arg2.entry = 0;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    quadTable[count].label = NULL;
    count++;
}

void while_expression_codeGen(struct treeNode *root)
{
    if(root->while_visited == 1)
        return;
    root->while_visited = 1;
    // if (!strcmp(root->value, "AND"))
    // {
    // }
    // else if (!strcmp(root->value, "OR"))
    // {
    // }
    if (!strcmp(root->value, "BOOLEAN"))
    {
        if (!strcmp(root->tk_data.lexeme, "false"))
        {
            quadTable[count].goTolabel = root->false_label;
            quadTable[count].instruction = "GOTO";
            count++;
        }
        else
        {
            quadTable[count].goTolabel = root->true_label;
            quadTable[count].instruction = "GOTO";
            count++;
        }
    }
    else if(!strcmp(root->value,"ID")){
        quadTable[count].instruction = "IF";
        quadTable[count].arg1.arg.arg_var = root->symbol_table_entry;
        quadTable[count].arg1.entry = 0;
        quadTable[count].arg2.arg.arg_var = NULL;
        quadTable[count].arg2.entry = 0;
        quadTable[count].result.arg.arg_var = NULL;
        quadTable[count].result.entry = 0;
        quadTable[count].goTolabel = root->true_label;
        if(root->astnextSibling == NULL)
        {
            if(!strcmp(root->parent->value,"AND"))
                quadTable[count].label = root->parent->children->true_label;
            else if(!strcmp(root->parent->value,"OR"))
                quadTable[count].label = root->parent->children->false_label;
        }
        count++;
        quadTable[count].goTolabel = root->false_label;
        quadTable[count].instruction = "GOTO";
        quadTable[count].arg1.arg.arg_var = NULL;
        quadTable[count].arg1.entry = 0;
        quadTable[count].arg2.arg.arg_var = NULL;
        quadTable[count].arg2.entry = 0;
        quadTable[count].result.arg.arg_var = NULL;
        quadTable[count].result.entry = 0;
        count++;
    }
    else if (!strcmp(root->value, "ARRAY_ACCESS"))
    {
        struct treeNode* idNode = root->children; //id for array
        quadTable[count].instruction = "IF";
        quadTable[count].arg1.arg.arg_var = idNode->symbol_table_entry;
        quadTable[count].arg1.entry = 0;
        struct treeNode* rightChild = root->children->astnextSibling;
        if(rightChild->temp_variable_entry == NULL){
            if(!strcmp(rightChild->value,"NUM")){
                quadTable[count].index1.arg.arg_num = rightChild->tk_data.val;
                quadTable[count].index1.entry = 1;
            }
            else{ //ID
                quadTable[count].index1.arg.arg_var = rightChild->symbol_table_entry;
                quadTable[count].index1.entry = 0;
            }
        }
        else{//Temp
            quadTable[count].index1.arg.arg_var = rightChild->temp_variable_entry;
            quadTable[count].index1.entry = 0;
        }
        quadTable[count].arg2.arg.arg_var = NULL;
        quadTable[count].arg2.entry = 0;
        quadTable[count].result.arg.arg_var = NULL;
        quadTable[count].result.entry = 0;
        quadTable[count].goTolabel = root->true_label;
        count++;
        quadTable[count].goTolabel = root->false_label;
        quadTable[count].instruction = "GOTO";
        quadTable[count].arg1.arg.arg_var = NULL;
        quadTable[count].arg1.entry = 0;
        quadTable[count].arg2.arg.arg_var = NULL;
        quadTable[count].arg2.entry = 0;
        quadTable[count].result.arg.arg_var = NULL;
        quadTable[count].result.entry = 0;
        count++;
    }
    else
    {
        // relational Operator
        struct treeNode *leftChild = root->children;
        struct treeNode *rightChild = root->children->astnextSibling;
        if (leftChild->temp_variable_entry == NULL)
        {
            if (!strcmp(leftChild->value, "ID"))
            {
                quadTable[count].arg1.arg.arg_var = leftChild->symbol_table_entry;
                quadTable[count].arg1.entry = 0;
            }
            else if (!strcmp(leftChild->value, "NUM"))
            {
                quadTable[count].arg1.arg.arg_num = leftChild->tk_data.val;
                quadTable[count].arg1.entry = 1;
            }
            else if (!strcmp(leftChild->value, "RNUM"))
            {
                quadTable[count].arg1.arg.arg_rnum = leftChild->tk_data.realVal;
                quadTable[count].arg1.entry = 2;
            }
            else
            {
                // ARRAY CASE
                quadTable[count].arg1.arg.arg_var = leftChild->children->symbol_table_entry;
                quadTable[count].arg1.entry = 0;
                if (leftChild->children->astnextSibling->temp_variable_entry != NULL)
                {
                    quadTable[count].index1.arg.arg_var = leftChild->children->astnextSibling->temp_variable_entry;
                    quadTable[count].index1.entry = 0;
                }
                else if (!strcmp(leftChild->children->astnextSibling->value, "NUM"))
                {
                    quadTable[count].index1.arg.arg_num = leftChild->children->astnextSibling->tk_data.val;
                    quadTable[count].index1.entry = 1;
                }
                else
                { // id case
                    quadTable[count].index1.arg.arg_var = leftChild->children->astnextSibling->symbol_table_entry;
                    quadTable[count].index1.entry = 0;
                }
            }
        }
        else
        {
            quadTable[count].arg1.arg.arg_var = leftChild->temp_variable_entry;
            quadTable[count].arg1.entry = 0;
        }

        // for right-child
        if (rightChild->temp_variable_entry == NULL)
        {
            if (!strcmp(rightChild->value, "ID")){
                quadTable[count].arg2.arg.arg_var = rightChild->symbol_table_entry;
                quadTable[count].arg2.entry = 0;
            }
            else if (!strcmp(rightChild->value, "NUM")){
                quadTable[count].arg2.arg.arg_num = rightChild->tk_data.val;
                quadTable[count].arg2.entry = 1;}
            else if (!strcmp(rightChild->value, "RNUM")) // RNUM case
            {   quadTable[count].arg2.arg.arg_rnum = rightChild->tk_data.realVal;
                quadTable[count].arg2.entry = 2;}
            else if (!strcmp(rightChild->value, "BOOLEAN")){
                quadTable[count].arg2.arg.arg_bool = rightChild->tk_data.lexeme;
                quadTable[count].arg2.entry = 3;}
            else
            {
                // ARRAY CASE

                quadTable[count].arg2.arg.arg_var = rightChild->children->symbol_table_entry;
                quadTable[count].arg2.entry = 0;
                if (rightChild->children->astnextSibling->temp_variable_entry != NULL)
                {
                    quadTable[count].index2.arg.arg_var = rightChild->children->astnextSibling->temp_variable_entry;
                    quadTable[count].index2.entry = 0;
                }
                else if (!strcmp(rightChild->children->astnextSibling->value, "NUM"))
                {
                    quadTable[count].index2.arg.arg_num = rightChild->children->astnextSibling->tk_data.val;
                    quadTable[count].index2.entry = 1;
                }
                else
                { // id case
                    quadTable[count].index2.arg.arg_var = rightChild->children->astnextSibling->symbol_table_entry;
                    quadTable[count].index2.entry = 0;
                }
            }
        }
        else
        {
            quadTable[count].arg2.arg.arg_var = rightChild->temp_variable_entry;
            quadTable[count].arg2.entry = 0;
        }

        // //-------------------------------------------------------------------------------
        // // left-child
        // if (leftChild->temp_variable_entry == NULL)
        // {
        //     if (!strcmp(leftChild->value, "ID"))
        //         quadTable[count].arg1.arg_var = leftChild->symbol_table_entry;
        //     else if (!strcmp(leftChild->value, "NUM"))
        //         quadTable[count].arg1.arg_num = leftChild->tk_data.val;
        //     else if (!strcmp(leftChild->value, "RNUM"))
        //         quadTable[count].arg1.arg_num = leftChild->tk_data.realVal;
        // }
        // else
        // {
        //     quadTable[count].arg1.arg_var = leftChild->temp_variable_entry;
        // }
        // // right-child
        // if (rightChild->temp_variable_entry == NULL)
        // {
        //     if (!strcmp(rightChild->value, "ID"))
        //         quadTable[count].arg2.arg_var = rightChild->symbol_table_entry;
        //     else if (!strcmp(rightChild->value, "NUM"))
        //         quadTable[count].arg2.arg_num = rightChild->tk_data.val;
        //     else if (!strcmp(rightChild->value, "RNUM")) // RNUM case
        //         quadTable[count].arg2.arg_num = rightChild->tk_data.realVal;
        // }
        // else
        // {
        //     quadTable[count].arg2.arg_var = rightChild->temp_variable_entry;
        // }

        strcpy(quadTable[count].op, root->value);
        quadTable[count].goTolabel = root->true_label;
        quadTable[count].instruction = "IF";
        count++;
        quadTable[count].instruction = "GOTO";
        quadTable[count].goTolabel = root->false_label;
        quadTable[count].arg1.arg.arg_var = NULL;
        quadTable[count].arg1.entry = 0;
        quadTable[count].arg2.arg.arg_var = NULL;
        quadTable[count].arg2.entry = 0;
        quadTable[count].result.arg.arg_var = NULL;
        quadTable[count].result.entry = 0;
        count++;
    }
}

void for_appears_top(struct treeNode *root)
{
    struct treeNode *idNode = root->children;
    struct treeNode *rangeNode = root->children->astnextSibling;
    struct treeNode *statementsNode = root->children->astnextSibling->astnextSibling;

    // assignment instruction
    strcpy(quadTable[count].op, "=");
    quadTable[count].arg1.arg.arg_num = idNode->symbol_table_entry->type.for_type.low_range;
    quadTable[count].arg1.entry = 1;
    quadTable[count].result.arg.arg_var = idNode->symbol_table_entry;
    quadTable[count].result.entry = 0;
    count++;

    // condition check
    // label for condition check
    if (root->constructLabel == NULL)
        root->constructLabel = newlabel();
    quadTable[count].label = newlabel();
    root->for_construct_label = quadTable[count].label; //setting for's construct label
    quadTable[count].instruction = "IF";
    strcpy(quadTable[count].op, "<=");
    quadTable[count].arg1.arg.arg_var = idNode->symbol_table_entry;
    quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_num = idNode->symbol_table_entry->type.for_type.high_range;
    quadTable[count].arg2.entry = 1;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    assignlabel(statementsNode);

    if(statementsNode != NULL)
        quadTable[count].goTolabel = statementsNode->constructLabel;
    count++;

    // goto for false condition
    if (root->next == NULL && root->parent!=NULL &&  (!strcmp(root->parent->value, "MODULE_DEF") || !strcmp(root->parent->value,"PROGRAM")))
        quadTable[count].goTolabel = "return:";
    // doubtful case
    else if (root->next == NULL && root->parent!=NULL && !strcmp(root->parent->value,"WHILE"))
    {
        // if (root->parent->constructLabel == NULL)
        //     root->parent->constructLabel = newlabel();
        assignlabel(root->parent);
        quadTable[count].goTolabel = root->parent->constructLabel;
    }
    else if(root->next == NULL && root->parent!=NULL && !strcmp(root->parent->value,"FOR")){
        root->parent->for_increment_label = newlabel();
        quadTable[count].goTolabel = root->parent->for_increment_label;
    }
    // KYA YE SAHI HAI?
    else if (root->next == NULL)
    {
        struct treeNode *temp = root->parent->next;
        while (temp == NULL)
            temp = temp->parent->next;
        quadTable[count].goTolabel = temp->constructLabel;
    }
    else
    {
        // if (root->next->constructLabel == NULL)
        //     root->next->constructLabel = newlabel();
        assignlabel(root->next);
        
        quadTable[count].goTolabel = root->next->constructLabel;
    }
    quadTable[count].instruction = "GOTO";
    quadTable[count].arg1.arg.arg_var = NULL;
    quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_var = NULL;
    quadTable[count].arg2.entry = 0;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    count++;

    if(statementsNode != NULL){
        quadTable[count].label = statementsNode->constructLabel;
    }
}

void for_appears_bottom(struct treeNode *root)
{
    struct treeNode *idNode = root->children;
    // increment operation
    quadTable[count].arg1.arg.arg_var = idNode->symbol_table_entry;
    quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_var = NULL;
    quadTable[count].arg2.entry = 0;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    quadTable[count].instruction = "INCREMENT"; // increment operation
    quadTable[count].goTolabel = NULL;
    if(root->for_increment_label!=NULL)
        quadTable[count].label = root->for_increment_label;
    else
        quadTable[count].label = NULL;
    count++;

    // goto for condition check
    quadTable[count].instruction = "GOTO";
    quadTable[count].goTolabel = root->for_construct_label;
    quadTable[count].arg1.arg.arg_var = NULL;
    quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_var = NULL;
    quadTable[count].arg2.entry = 0;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    quadTable[count].label = NULL;
    count++;
}

void case_appears_top(struct treeNode *root)
{
    struct treeNode *idNode = root->parent->children;
    struct treeNode *stmtsNode = root->pair;
    // if (stmtsNode->constructLabel == NULL)
    //     stmtsNode->constructLabel = newlabel();
    assignlabel(stmtsNode);
    strcpy(quadTable[count].op, "==");
    if (root->constructLabel == NULL) // generating label for case
        root->constructLabel = newlabel();
    //assignlabel(root);
    quadTable[count].label = root->constructLabel;
    quadTable[count].arg1.arg.arg_var = idNode->symbol_table_entry;
    quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_num = root->tk_data.val;
    quadTable[count].arg2.entry = 1;
    quadTable[count].goTolabel = stmtsNode->constructLabel;
    quadTable[count].instruction = "IF";
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    count++;
    quadTable[count].instruction = "GOTO";
    //default exists
    if(root->next == NULL && root->parent->children->astnextSibling->astnextSibling!=NULL){
        assignlabel(root->parent->children->astnextSibling->astnextSibling);
        quadTable[count].goTolabel = root->parent->children->astnextSibling->astnextSibling->constructLabel;
    }
    else if (root->next == NULL && root->parent->next != NULL)
        quadTable[count].goTolabel = root->parent->next->constructLabel;
    else if (root->next == NULL && (!strcmp(root->parent->parent->value, "MODULE_DEF") || !strcmp(root->parent->parent->value,"PROGRAM")))
    {
        quadTable[count].goTolabel = "return:";     
    }
    // recursive like case, case ki nesting
    else if (root->next == NULL && strcmp(root->parent->parent->parent->value, "SWITCH"))
    {
        quadTable[count].goTolabel = root->parent->parent->constructLabel;
    }
    else if (root->next == NULL)
    {
        struct treeNode *temp = root->parent->parent;
        while (strcmp(temp->value, "MODULE_DEF") && strcmp(temp->value, "PROGRAM") && temp->next == NULL)
            temp = temp->parent;
        if (!strcmp(temp->value, "MODULE_DEF") || !strcmp(temp->value,"PROGRAM"))
            quadTable[count].goTolabel = "return:";
        else
            quadTable[count].goTolabel = temp->next->constructLabel;
    }
    else
    {
        // if (root->next->constructLabel == NULL)
        //     root->next->constructLabel = newlabel();
        assignlabel(root->next);
        quadTable[count].goTolabel = root->next->constructLabel;
    }

    quadTable[count].arg1.arg.arg_var = NULL;
     quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_var = NULL;
    quadTable[count].arg2.entry = 0;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    quadTable[count].label = NULL;
    count++;
}

void case_appears_bottom(struct treeNode *root)
{ 
    struct treeNode *switchNext = root->parent->next;
    if (switchNext == NULL && (!strcmp(root->parent->parent->value, "MODULE_DEF") || !strcmp(root->parent->parent->value,"PROGRAM")))
    {
        quadTable[count].goTolabel = "return:";
    }
    // doubtful case(agar parent case stmt hua to)
    else if (switchNext == NULL && !strcmp(root->parent->parent->value,"WHILE"))
    {
        quadTable[count].goTolabel = root->parent->parent->constructLabel;
    }
    else if(switchNext == NULL && !strcmp(root->parent->parent->value,"FOR")){
        root->parent->parent->for_increment_label = newlabel();
        quadTable[count].goTolabel = root->parent->parent->for_increment_label;
    }
    // KYA YE SAHI HAI?
    else if (switchNext == NULL)
    {
        struct treeNode *temp = root->parent->parent;
        while (strcmp(temp->value, "MODULE_DEF") && strcmp(temp->value,"PROGRAM") && temp->next == NULL)
            temp = temp->parent;
        if (!strcmp(temp->value, "MODULE_DEF") || !strcmp(temp->value,"PROGRAM"))
            quadTable[count].goTolabel = "return:";
        else
            quadTable[count].goTolabel = temp->next->constructLabel;
    }
    else
    {
        quadTable[count].goTolabel = switchNext->constructLabel;
    }
    quadTable[count].instruction = "GOTO";
    quadTable[count].arg1.arg.arg_var = NULL;
     quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_var = NULL;
    quadTable[count].arg2.entry = 0;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    quadTable[count].label = NULL;
    count++;
}

ST_ENTRY* get_offset_for_array(struct treeNode* root, int arr_index,int lowRange){
    //root is GET-VALUE,arr_index is passed as an integer here
    struct treeNode* arr_name = root->children;
    int base = arr_name->symbol_table_entry->offset;
    char* type = arr_name->symbol_table_entry->type.arr_type.arr_dt;

    //A[i] = A(base) + (i-lowRange)*width

    //i-lowRange instruction
    strcpy(quadTable[count].op,"MINUS");
    quadTable[count].arg1.arg.arg_num =  arr_index;
    quadTable[count].arg1.entry = 1;
    quadTable[count].arg2.arg.arg_num = lowRange;
    quadTable[count].arg2.entry = 1;
    TYPE t;
    t.id_type.id_dt = "INTEGER";
    ST_ENTRY *tempVarEntry2 = newTemp(t);
    quadTable[count].result.arg.arg_var = tempVarEntry2;
    quadTable[count].result.entry = 0;
    count++;

    //temp*width instruction
    strcpy(quadTable[count].op,"MUL");
    quadTable[count].arg1.arg.arg_var = tempVarEntry2;
    quadTable[count].arg1.entry = 0;
    
    if(!strcasecmp(type,"INTEGER")){
        quadTable[count].arg2.arg.arg_num = 2;
        quadTable[count].arg2.entry = 1;
    }
    else if(!strcasecmp(type,"REAL")){
        quadTable[count].arg2.arg.arg_num = 4;
        quadTable[count].arg2.entry = 1;
    }
    else
    {
        //boolean
        quadTable[count].arg2.arg.arg_num = 1;
        quadTable[count].arg2.entry = 1;
    }

    ST_ENTRY *tempVarEntry = newTemp(t);
    quadTable[count].result.arg.arg_var = tempVarEntry;
    quadTable[count].result.entry = 0;
    count++;

    //A(base) + temp
    strcpy(quadTable[count].op,"PLUS");
    quadTable[count].arg1.arg.arg_num = base;
    quadTable[count].arg1.entry = 1;
    quadTable[count].arg2.arg.arg_var = tempVarEntry;
    quadTable[count].arg2.entry = 0;
    ST_ENTRY *tempVarEntry1 = newTemp(t);
    quadTable[count].result.arg.arg_var = tempVarEntry1;
    quadTable[count].result.entry = 0;
    //IS THIS REQUIRED HERE?
    root->temp_variable_entry = tempVarEntry1; //use this temp variable in case of array access
    count++;
    return tempVarEntry1;
}

void IRcodegenerate(struct treeNode *root)
{
    if (root == NULL || root->visited == 1)
        return;
    root->visited = 1;

    if (root->constructLabel == NULL){
        if(!strcmp(root->value,"DECLARE") || !strcmp(root->value,"GET-VALUE") || !strcmp(root->value,"PRINT") || !strcmp(root->value,"WHILE") || !strcmp(root->value,"FOR") || !strcmp(root->value,"SWITCH") || (root->parent != NULL && !strcmp(root->parent->value,"SWITCH")) ||!strcmp(root->value,"LVALUEID") || !strcmp(root->value,"LVALUEARR") || !strcmp(root->value,"MODULE-INVOKE"))
            root->constructLabel = newlabel();
    }

    if(quadTable[count].label == NULL && root->constructLabel!=NULL)
        quadTable[count].label = root->constructLabel; // set label at the beginning of each construct

    //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // top down
    if (!strcmp(root->value, "WHILE"))
    {
        struct treeNode *expr = root->children;                            // expression
        struct treeNode *statementsChild = root->children->astnextSibling; // statements
        // set labels
        // CHECK KI NEXT NULL TO NI HO GAYA
        // if (statementsChild->constructLabel == NULL)
        //     statementsChild->constructLabel = newlabel();
        assignlabel(statementsChild);
        expr->true_label = statementsChild->constructLabel;
        if (root->next == NULL &&  (!strcmp(root->parent->value, "MODULE_DEF")))
        {
            expr->false_label = "return:";
        }
        /**************DOUBTFUL CASE(parent is case)**************/
        else if (root->next == NULL && !strcmp(root->parent->value,"WHILE"))
        {
            // if (root->parent->constructLabel == NULL)
            //     root->parent->constructLabel = newlabel();
            assignlabel(root->parent);
            expr->false_label = root->parent->constructLabel;
        }
        else if(root->next!=NULL && !strcmp(root->parent->value,"FOR")){
            root->parent->for_increment_label = newlabel();
            expr->false_label = root->parent->for_increment_label;
        }
        // MARK THIS FOR LATER
        else if (root->next == NULL)
        {
            struct treeNode *temp = root->parent;
            while (strcmp(temp->value, "MODULE_DEF") && strcmp(temp->value,"PROGRAM") && temp->next == NULL)
            {
                temp = temp->parent;
            }
            if (!strcmp(temp->value, "MODULE_DEF") || !strcmp(temp->value,"PROGRAM"))
                expr->false_label = "return:";
            else
                expr->false_label = temp->next->constructLabel;
        }
        else
        {
            // if (root->next->constructLabel == NULL)
            //     root->next->constructLabel = newlabel();
            assignlabel(root->next);
            expr->false_label = root->next->constructLabel;
        }
    }
    
    else if (!strcmp(root->value, "MODULE_DEF"))
    {
        // if (root->constructLabel == NULL)
        //     root->constructLabel = newlabel();
        //store label in symbol table entry
        quadTable[count].instruction = "DECLAREMOD";
        quadTable[count].arg_fn_name = root->children->function_table_entry;
        count++;
    }
    
    else if (!strcmp(root->value, "FOR"))
    {
        for_appears_top(root);
    }
    // case stmt top-down
    else if (root->parent!=NULL && !strcmp(root->parent->value, "SWITCH") && root->pair != NULL)
    {
        case_appears_top(root);
    }
    // only for AND and OR
    else if (!strcmp(root->value, "AND") || !strcmp(root->value, "OR"))
    {
        if (!root->isChildOfAssign)
            while_expression_appears(root);
    }
    
    if(root->children!=NULL){
        if (root->parent != NULL && (!strcmp(root->parent->value, "LVALUEID") || !strcmp(root->parent->value, "LVALUEARRAY") || root->parent->isChildOfAssign))
            root->isChildOfAssign = 1;
        else{
            root->isChildOfAssign = 0;
        }
    }
        
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // left-child expand
    IRcodegenerate(root->children);

    if (!strcmp(root->value, "AND") && !root->isChildOfAssign)
    {
        quadTable[count].goTolabel = root->children->true_label;
        // POINT OF DOUBT/ERROR
        //  count++;
    }
    else if (!strcmp(root->value, "OR") && !root->isChildOfAssign)
        quadTable[count].goTolabel = root->children->false_label;

    // right-child expand
    if (root->children != NULL)
    {
        struct treeNode *temp;
        temp = root->children->astnextSibling;

        while (temp != NULL)
        {
            if (!strcmp(root->value, "LVALUEID") || !strcmp(root->value, "LVALUEARR") || root->isChildOfAssign)
                temp->isChildOfAssign = 1;
            else
                temp->isChildOfAssign = 0;
            IRcodegenerate(temp);
            temp = temp->astnextSibling;
        }
    }

    if (root->pair != NULL)
        IRcodegenerate(root->pair);
    

   
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //control comes to a node
    if (root->addr != NULL)
    {
        IRcodegenerate(root->addr);
    }
    else
    {
        if (!strcmp(root->value, "LVALUEID"))
        {
            struct treeNode *id = root->children;
            struct treeNode *temporary = id->astnextSibling;
            if (temporary->temp_variable_entry != NULL){
                quadTable[count].arg1.arg.arg_var = temporary->temp_variable_entry;
                 quadTable[count].arg1.entry = 0;
            }
            else
            {
                if (!strcasecmp(temporary->value, "NUM"))
                {
                    quadTable[count].arg1.arg.arg_num = temporary->tk_data.val;
                    quadTable[count].arg1.entry = 1;
                }
                else if (!strcasecmp(temporary->value, "RNUM"))
                {
                    quadTable[count].arg1.arg.arg_rnum = temporary->tk_data.realVal;
                    quadTable[count].arg1.entry = 2;
                }
                else if(!strcasecmp(temporary->value,"TRUE"))
                {
                    quadTable[count].arg1.arg.arg_bool = "true";
                    quadTable[count].arg1.entry = 3;
                }
                else if(!strcasecmp(temporary->value,"FALSE"))
                {
                    quadTable[count].arg1.arg.arg_bool = "false";
                    quadTable[count].arg1.entry = 3;
                }
                // introduce Array case later
                else{
                    quadTable[count].arg1.arg.arg_var = temporary->symbol_table_entry;
                    quadTable[count].arg1.entry = 0;
                }
            }
            // setting arg2 to 0 in the case of LVALUEID
            quadTable[count].arg2.arg.arg_var = NULL;
            quadTable[count].arg2.entry = 0;
            strcpy(quadTable[count].op, "=");
            quadTable[count].result.arg.arg_var = id->symbol_table_entry;
            quadTable[count].result.entry = 0;
            count++;
        }

        else if (!strcmp(root->value, "LVALUEARRAY"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
            struct treeNode *id = root->children;
            struct treeNode *temporary = id->astnextSibling;
            
            if (temporary->temp_variable_entry != NULL){ 
                quadTable[count].arg1.arg.arg_var = temporary->temp_variable_entry;
                quadTable[count].arg1.entry = 0;
            }
                
            else
            {
                if (!strcmp(temporary->value, "NUM"))
                {
                    quadTable[count].arg1.arg.arg_num = temporary->tk_data.val;
                    quadTable[count].arg1.entry = 1;
                }
                else if (!strcmp(temporary->value, "RNUM"))
                {
                    quadTable[count].arg1.arg.arg_rnum = temporary->tk_data.realVal;
                    quadTable[count].arg1.entry = 2;
                }
                else if(!strcasecmp(temporary->value,"TRUE"))
                {
                    quadTable[count].arg1.arg.arg_bool = "true";
                    quadTable[count].arg1.entry = 3;
                }
                else if(!strcasecmp(temporary->value,"FALSE"))
                {
                    
                    quadTable[count].arg1.arg.arg_bool = "false";
                    quadTable[count].arg1.entry = 3;
                }
                // introduce Array case later
                else
                {
                    quadTable[count].arg1.arg.arg_var = temporary->symbol_table_entry;
                    quadTable[count].arg1.entry = 0;
                }
            }

            // setting arg2 to 0 in the case of LVALUEID
            if (id->children->astnextSibling->temp_variable_entry != NULL)
            {
                quadTable[count].arg2.arg.arg_var = id->children->astnextSibling->temp_variable_entry;
                quadTable[count].arg2.entry = 0;
            }
            else
            {
                if (!strcmp(temporary->value,"NUM"))
                {
                    quadTable[count].arg2.arg.arg_num = id->children->astnextSibling->tk_data.val;
                    quadTable[count].arg2.entry = 1;
                }
                else if (!strcmp(temporary->value, "RNUM"))
                {
                    quadTable[count].arg2.arg.arg_rnum = id->children->astnextSibling->tk_data.realVal;
                    quadTable[count].arg2.entry = 2;
                }
                else
                {
                    quadTable[count].arg2.arg.arg_var = id->children->astnextSibling->symbol_table_entry;
                    quadTable[count].arg2.entry = 0;
                }
            }
            strcpy(quadTable[count].op, "=");
            quadTable[count].result.arg.arg_var = id->children->symbol_table_entry;
            quadTable[count].result.entry = 0;
            count++;
        }
        
        // //offset computation for array access
        // else if(!strcmp(root->value,"ARRAY_ACCESS"))
        // {
        //     struct treeNode* arr_name = root->children;
        //     struct treeNode* index = root->children->astnextSibling;  
        //     int base = arr_name->symbol_table_entry->offset;
        //     char* type = arr_name->symbol_table_entry->type.arr_type.arr_dt;
        //     int low_range = arr_name->symbol_table_entry->type.arr_type.lowRange.start;

        //     //A[i] = A(base) + (i-lowRange)*width

        //     //i-lowRange instruction
        //     strcpy(quadTable[count].op,"MINUS");
        //     if(index->temp_variable_entry != NULL)
        //     {
        //         quadTable[count].arg1.arg.arg_var = index->temp_variable_entry;
        //         quadTable[count].arg1.entry = 0;
        //     }

        //     else
        //     {
        //         if(!strcmp(index->value,"NUM")){
        //             quadTable[count].arg1.arg.arg_num =  index->tk_data.val;
        //             quadTable[count].arg1.entry = 1;
        //         }

        //         else if(!strcmp(index->value,"ID")){
        //             quadTable[count].arg1.arg.arg_var =  index->symbol_table_entry;
        //             quadTable[count].arg1.entry = 0;
        //         }
        //     }

        //     if(arr_name->symbol_table_entry->type.arr_type.isStatic == 1){
        //         quadTable[count].arg2.arg.arg_num = arr_name->symbol_table_entry->type.arr_type.lowRange.start;
        //         quadTable[count].arg2.entry = 1;
        //     }
        //     else{
        //         //DIKKAT HAI ISME :( if array is not static,need symbol table entry
        //         //quadTable[count].arg2.arg.arg_var = arr_name->symbol_table_entry->type.arr_type.lowRange.low_id;
        //     }
        //     TYPE t;
        //     t.id_type.id_dt = "INTEGER";
        //     ST_ENTRY *tempVarEntry2 = newTemp(t);
        //     quadTable[count].result.arg.arg_var = tempVarEntry2;
        //     quadTable[count].result.entry = 0;
        //     count++;

        //     //temp*width instrcution
        //     strcpy(quadTable[count].op,"MUL");
        //     quadTable[count].arg1.arg.arg_var = tempVarEntry2;
        //     quadTable[count].arg1.entry = 0;
            
        //     if(!strcasecmp(type,"INTEGER")){
        //         quadTable[count].arg2.arg.arg_num = 2;
        //         quadTable[count].arg2.entry = 1;
        //     }
        //     else if(!strcasecmp(type,"REAL")){
        //         quadTable[count].arg2.arg.arg_num = 4;
        //         quadTable[count].arg2.entry = 1;
        //     }
        //     else
        //     {
        //         //boolean
        //         quadTable[count].arg2.arg.arg_num = 1;
        //         quadTable[count].arg2.entry = 1;
        //     }

        //     ST_ENTRY *tempVarEntry = newTemp(t);
        //     quadTable[count].result.arg.arg_var = tempVarEntry;
        //     quadTable[count].result.entry = 0;
        //     count++;

        //     //A(base) + temp
        //     strcpy(quadTable[count].op,"PLUS");
        //     quadTable[count].arg1.arg.arg_num = base;
        //     quadTable[count].arg1.entry = 1;
        //     quadTable[count].arg2.arg.arg_var = tempVarEntry;
        //     quadTable[count].arg2.entry = 0;
        //     ST_ENTRY *tempVarEntry1 = newTemp(t);
        //     quadTable[count].result.arg.arg_var = tempVarEntry1;
        //     quadTable[count].result.entry = 0;
        //     root->temp_variable_entry = tempVarEntry1; //use this temp variable in case of array access
        //     count++;
        // }

        else if (!strcmp(root->value, "PLUS") || !strcmp(root->value, "MINUS") || !strcmp(root->value, "MUL") || !strcmp(root->value, "DIV"))
        {
            operator_appears(root);
        }
        else if (!strcmp(root->value, "AND") || !strcmp(root->value, "OR") || !strcmp(root->value, "LT") || !strcmp(root->value, "LE") || !strcmp(root->value, "GT") || !strcmp(root->value, "GE") || !strcmp(root->value, "NE") || !strcmp(root->value, "EQ"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel(); // label for the expression
            if (root->isChildOfAssign == 1)
            {
                operator_appears(root);
            }
            else
            { // WHILE ka case
                if(!strcmp(root->value,"AND") ||!strcmp(root->value,"OR")){
                    while_expression_codeGen(root->children);
                    while_expression_codeGen(root->children->astnextSibling);
                }
                else    
                    while_expression_codeGen(root);

            }
        }
        else if (!strcmp(root->value, "U_MINUS") || !strcmp(root->value, "U_PLUS"))
        {  
            // set operator
            strcpy(quadTable[count].op, root->value);
           
            TYPE t;
            struct treeNode *rightChild = root->children->astnextSibling;
            if (rightChild->temp_variable_entry != NULL){
                quadTable[count].arg1.arg.arg_var = rightChild->temp_variable_entry;
                quadTable[count].arg1.entry = 0;
                t = rightChild->temp_variable_entry->type;
            }
            else if (!strcmp(rightChild->value, "ID")){
                quadTable[count].arg1.arg.arg_var = rightChild->symbol_table_entry;
                quadTable[count].arg1.entry = 0;
                if(rightChild->symbol_table_entry->is_array == 1)
                    t.id_type.id_dt = rightChild->symbol_table_entry->type.arr_type.arr_dt;
                else if(rightChild->symbol_table_entry->is_for == 1)
                    t.id_type.id_dt = "INTEGER";
                t = rightChild->symbol_table_entry->type;
            }
            else if (!strcmp(rightChild->value, "NUM")){
                quadTable[count].arg1.arg.arg_num = rightChild->tk_data.val;
                quadTable[count].arg1.entry = 1;
                t.id_type.id_dt = "INTEGER";
            }
            else{ // RNUM case
                quadTable[count].arg1.arg.arg_rnum = rightChild->tk_data.realVal;
                quadTable[count].arg1.entry = 2;
                t.id_type.id_dt = "REAL";
            }
            quadTable[count].arg2.arg.arg_var = NULL;
            quadTable[count].arg2.entry = 0;
            
            // NUM KA SYMBOL TABLE ENTRY NAHI HAI
            ST_ENTRY *tempVarEntry = newTemp(t);
           
            quadTable[count].result.arg.arg_var = tempVarEntry;
            quadTable[count].result.entry = 0;
            root->temp_variable_entry = tempVarEntry; // set temp_variable entry of node
            count++;
        }
        // aakhri jagah
        else if (!strcmp(root->value, "GET-VALUE"))
        {
            // set operator
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel(); // giving construct label to get-value
            if(root->children->symbol_table_entry->is_array == 1 && root->children->symbol_table_entry->type.arr_type.isStatic == 1 )
            {
                int low_index = root->children->symbol_table_entry->type.arr_type.lowRange.start;
                int high_index = root->children->symbol_table_entry->type.arr_type.highRange.end; 
                int width;
                if(!strcasecmp(root->children->symbol_table_entry->type.arr_type.arr_dt,"INTEGER"))
                    width = 2;
                else if(!strcasecmp(root->children->symbol_table_entry->type.arr_type.arr_dt,"REAL"))
                    width = 4;
                else{
                    width = 1;
                }
                for(int i = low_index; i <= high_index; i++)
                {
                    // ST_ENTRY* offset_entry = get_offset_for_array(root,i,low_index); //storing offset in a temp variable here
                    // quadTable[count].result.arg.arg_var =  offset_entry;
                    quadTable[count].result.arg.arg_num = (i-low_index)*width;
                    strcpy(quadTable[count].op, "GET");
                    quadTable[count].result.entry = 1;            
                    quadTable[count].arg2.arg.arg_var = NULL;
                    quadTable[count].arg2.entry = 0;
                    quadTable[count].arg1.arg.arg_var = root->children->symbol_table_entry;  //arg1 stores the variable in which input is to be read
                    quadTable[count].arg1.entry = 0;
                    quadTable[count].index1.arg.arg_num = i;// this specifies the element number in case of array
                    quadTable[count].index1.entry = 1;
                    quadTable[count].instruction = root->children->symbol_table_entry->type.arr_type.arr_dt;// this stores type
                    count++;
                }
            }

            else{
                strcpy(quadTable[count].op, "GET");
                quadTable[count].result.arg.arg_var = NULL;
                quadTable[count].result.entry = 0;            
                quadTable[count].arg2.arg.arg_var = NULL;
                quadTable[count].arg2.entry = 0;
                quadTable[count].arg1.arg.arg_var = root->children->symbol_table_entry;
                quadTable[count].arg1.entry = 0;
                quadTable[count].instruction = root->children->symbol_table_entry->type.arr_type.arr_dt;
                count++;
            }
        }
        else if (!strcmp(root->value, "PRINT"))
        {
            // set operator
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel(); // giving construct label to print
            if(strcmp(root->children->value,"NUM") && strcmp(root->children->value,"RNUM") && root->children->symbol_table_entry->is_array == 1 && root->children->symbol_table_entry->type.arr_type.isStatic == 1 )
            {
                int low_index = root->children->symbol_table_entry->type.arr_type.lowRange.start;
                int high_index = root->children->symbol_table_entry->type.arr_type.highRange.end; 
                int width;
                if(!strcasecmp(root->children->symbol_table_entry->type.arr_type.arr_dt,"INTEGER"))
                    width = 2;
                else if(!strcasecmp(root->children->symbol_table_entry->type.arr_type.arr_dt,"REAL"))
                    width = 4;
                else{
                    width = 1;
                }
                for(int i = low_index; i <= high_index; i++)
                {
                    // ST_ENTRY* offset_entry = get_offset_for_array(root,i,low_index); //storing offset of variable in a temp entry
                    // quadTable[count].result.arg.arg_var = offset_entry;
                    quadTable[count].result.arg.arg_num = (i-low_index)*width;
                    quadTable[count].result.entry = 1;   
                    strcpy(quadTable[count].op, "PRINT");         
                    quadTable[count].arg2.arg.arg_var = NULL;
                    quadTable[count].arg2.entry = 0;
                    quadTable[count].arg1.arg.arg_var = root->children->symbol_table_entry;  //arg1 stores the variable in which input is to be read
                    quadTable[count].arg1.entry = 0;
                    quadTable[count].index1.arg.arg_num = i;// this specifies the element number in case of array
                    quadTable[count].index1.entry = 1;
                    quadTable[count].instruction = root->children->symbol_table_entry->type.arr_type.arr_dt;// this stores type
                    count++;
                }
            }

            else{
                strcpy(quadTable[count].op, "PRINT");
                quadTable[count].arg1.arg.arg_var = NULL;
                quadTable[count].arg1.entry = 0;
                quadTable[count].arg2.arg.arg_var = NULL;
                quadTable[count].arg2.entry = 0;
                if (!strcmp(root->children->value, "NUM"))
                {
                    quadTable[count].result.arg.arg_num = root->children->tk_data.realVal;
                    quadTable[count].result.entry = 1;
                }
                else if (!strcmp(root->children->value, "RNUM"))
                {
                    quadTable[count].result.arg.arg_rnum = root->children->tk_data.val;
                    quadTable[count].result.entry = 2;
                }
                else
                { // boolean case
                    quadTable[count].result.arg.arg_var = root->children->symbol_table_entry;
                    quadTable[count].result.entry = 0;
                }
                count++;
            }
        }
        else if (!strcmp(root->value, "WHILE"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
            while_appears(root);
        }
        else if (!strcmp(root->value, "FOR"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
            for_appears_bottom(root);
        }
        else if (!strcmp(root->value, "SWITCH"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
        }
        else if (!strcmp(root->value, "MODULE-INVOKE"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
            struct treeNode* moduleID = root->children;
            struct treeNode* arguments = root->children->astnextSibling->children;
            if(arguments->astnextSibling != NULL){
                //optional statment
                while(arguments!=NULL){
                    quadTable[count].instruction = "RETURN-TO";
                    quadTable[count].result.arg.arg_var = arguments->symbol_table_entry;
                    quadTable[count].result.entry = 0;
                    count++;
                    arguments = arguments->next;
                }
                arguments = arguments->astnextSibling;
            }
            while(arguments!=NULL){
                //idList
                quadTable[count].instruction = "MODULE-INVOKE";
                quadTable[count].result.arg.arg_var = arguments->symbol_table_entry;
                quadTable[count].result.entry = 0;
                count++;
                arguments = arguments->next;
            }
        }
        
        else if (!strcmp(root->value, "DECLARE"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
           
            struct treeNode* variables = root->children->astnextSibling;
            while(variables != NULL)
            {
                //quadTable[count].instruction = "declare";
                strcpy(quadTable[count].op,"declare");
                quadTable[count].result.arg.arg_var = variables->symbol_table_entry;
                quadTable[count].result.entry = 0;
                // quadTable[count].arg1.arg_var = NULL;
                // quadTable[count].arg2.arg_var = NULL;
                count++;
                variables = variables->next;
            }
           
        }


        // check-kar lena ek baar switch(essentially checking for case stmts)
        if (root->parent!=NULL && !strcmp(root->parent->value, "SWITCH") && root->pair != NULL)
        {
            case_appears_bottom(root);
        }
        // check for default
        if (root->parent!=NULL && !strcmp(root->parent->value, "SWITCH") && root->isDefault == 1)
        {
            quadTable[count].instruction = "GOTO";
            if (root->parent->next == NULL && !strcmp(root->parent->parent->value, "MODULE_DEF") && !strcmp(root->parent->parent->value,"PROGRAM"))
            {
                quadTable[count].goTolabel = "return:";
            }
            else if (root->parent->next == NULL && root->parent->parent->parent!=NULL && strcmp(root->parent->parent->parent->value, "SWITCH"))
            {
                quadTable[count].goTolabel = root->parent->parent->constructLabel;
            }
            // KYA YE SHI HAI?? Bhagwan jaane
            else if (root->parent->next == NULL)
            {
                struct treeNode *temp = root->parent->parent;
                while (strcmp(temp->value, "MODULE_DEF") && strcmp(temp->value, "PROGRAM") && temp->next == NULL)
                    temp = temp->parent;
                if (!strcmp(temp->value, "MODULE_DEF") || !strcmp(temp->value,"PROGRAM"))
                    quadTable[count].goTolabel = "return:";
                else
                    quadTable[count].goTolabel = temp->next->constructLabel;
            }
            else
            {
                quadTable[count].goTolabel = root->parent->next->constructLabel;
            }
            quadTable[count].arg1.arg.arg_var = NULL;
            quadTable[count].arg1.entry = 0;
            quadTable[count].arg2.arg.arg_var = NULL;
            quadTable[count].arg2.entry = 0;
            quadTable[count].result.arg.arg_var = NULL;
            quadTable[count].result.entry = 0;
            quadTable[count].label = NULL;
            count++;
        }
    }

    // if (root->pair != NULL)
    //     IRcodegenerate(root->pair);

    root = root->next;
    if (root != NULL)
    {
        IRcodegenerate(root);
        // root = root->next;
    }
}

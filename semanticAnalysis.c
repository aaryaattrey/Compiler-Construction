/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#include "symbolTable.c"

// to populate symbol table and open new symbol tables

// 0 for int, 1 for real, 2 for bool, -1 for error
int get_type(struct treeNode *node, struct id_symbol_table *table) // to get type for expression construct
{
    if (node == NULL)
    {
        printf("\n Node null inside get_type");
        
    }

    // if there is no child (expression is either id/num/rnum/true/false) **array access has children and is handled below**
    if (node->children == NULL || !strcasecmp(node->value, "U1_MINUS") || !strcasecmp(node->value, "U1_PLUS") || !strcasecmp(node->value, "U_MINUS") || !strcasecmp(node->value, "U_PLUS") || !strcasecmp(node->value,"ARRAY_ACCESS")) 
    {   
        if (!strcasecmp(node->value, "ID"))
        {
            // printf("\n**get lexeme called for:%s \n",node->tk_data.lexeme);
            ST_ENTRY *temp = get_lexeme(table, node->tk_data.lexeme,node->line_no);
           
            // printf("\n**get lexeme after call for:%s \n", node->tk_data.lexeme);
            if(temp==NULL){
                // printf("ID IS NULL");
                return -1;
            }
            
            //creating symbol table entry for a node
            node->symbol_table_entry=temp;
            // printf("\n HEre before type\n");
            char *datatype;
            if(temp->is_for == 1)
                datatype = "INTEGER";
            else if(temp->is_array==1){
                programHasSemanticError = 1;
                printf("\n****Error:Array operand not allowed, line no : %d, from here****\n",node->line_no);
                return -1;
            }
            else
                datatype = temp->type.id_type.id_dt;
            
            // printf("\nTYPE is:%s \n",datatype);
            // printf("\nHERE AFTER PRINTING TYPE\n");
            // id should not be of array type
            if (strlen(datatype) == 0)
            {
                programHasSemanticError = 1;
                printf("\n Expression cannot contain array type variable. ");
                return -1;
            }
            if (!strcasecmp(datatype, "INTEGER"))
            {
                return 0;
            }
            else if (!strcasecmp(datatype, "REAL"))
            {
                return 1;
            }
            else if (!strcasecmp(datatype, "BOOLEAN"))
            {
                return 2;
            }
        }
        if (!strcasecmp(node->value, "NUM"))
        {
            return 0;
        }
        if (!strcasecmp(node->value, "RNUM"))
        {
            return 1;
        }
        if (!strcasecmp(node->value, "TRUE") || !strcasecmp(node->value, "FALSE"))
        {
            return 2;
        }
        if (!strcasecmp(node->value, "U1_MINUS") || !strcasecmp(node->value, "U1_PLUS"))
        {
            return get_type(node->children->astnextSibling, table);
        }
        if (!strcasecmp(node->value, "U_MINUS") || !strcasecmp(node->value, "U_PLUS"))
        {   
        
            return get_type(node->children->astnextSibling, table);
        }
        if (!strcasecmp(node->value, "ARRAY_ACCESS"))
        {   
        int ans;
        ST_ENTRY *temp = get_lexeme(table, node->children->tk_data.lexeme,node->children->line_no);
        if(temp==NULL){
            return -1;
        }

        // creating symbol table entry for array id
        node->children->symbol_table_entry=temp;
        char *datatype;
        datatype = temp->type.arr_type.arr_dt;
        if (!strcasecmp(datatype, "integer"))
        {
            ans = 0;
        }
        else if (!strcasecmp(datatype, "real"))
        {
            ans = 1;
        }
        else if (!strcasecmp(datatype, "boolean"))
        {
            ans = 2;
        }
        // check array bounds if array index is a num
        int lr = temp->type.arr_type.lowRange.start;
        int hr = temp->type.arr_type.highRange.end;
        if (!strcasecmp(node->children->astnextSibling->value, "NUM"))
        {
            int arr_index = node->children->astnextSibling->tk_data.val;
            if (arr_index >= lr && arr_index <= hr)
            {
                // printf("\n Array index: %d falls in range, ans = %d ", node->children->astnextSibling->tk_data.val,ans);

            }
            else
            {
                programHasSemanticError = 1;
                printf("\n****Error:Array index: %d not in range, line no: %d****", node->children->astnextSibling->tk_data.val,node->children->line_no);
                return -1;
            }
        }

        // check array index type (should be integer)
        int array_expr_type = get_type(node->children->astnextSibling, table);

        if (array_expr_type != 0)
        {
            programHasSemanticError = 1;
            printf("\n Array index is not integer, at line no:");
            return -1;
        }
        return ans;
    }
    }

    // if there are 2 children of an operator node (left operand, right operand)
    int leftOpType = get_type(node->children, table);
    int rightOpType = get_type(node->children->astnextSibling, table);
    int ans;

    if (leftOpType == -1 || rightOpType == -1) // if there is a type error coming from below
    {
        return -1;
    }

    if (!strcasecmp(node->value, "PLUS") || !strcasecmp(node->value, "MINUS"))
    {
        
        if (leftOpType == 0 && rightOpType == 0)
        {
            
            return 0;
        }
        if (leftOpType == 1 && rightOpType == 1)
        {
            
            return 1;
        }
        else
        {
            
            // printf("\n Incompatible addition/subtraction %s leftOp:%d rightOp:%d \n",node->value,leftOpType,rightOpType);
            printf("\n Incompatible addition/Subtraction");
            programHasSemanticError = 1;
            return -1;
        }
    }

    if (!strcasecmp(node->value, "DIV"))
    {
        if ((leftOpType == 0 && rightOpType == 0) || (leftOpType == 1 && rightOpType == 1) || (leftOpType == 0 && rightOpType == 1) || (leftOpType == 1 && rightOpType == 0))
        {
            return 1;
        }
        else
        {
            programHasSemanticError=1;
            printf("\n Incompatible Division");
            return -1;
        }
    }

    if (!strcasecmp(node->value, "MUL"))
    {
        
        if ((leftOpType == 1 && rightOpType == 1))
        {
            return 1;
        }
        else if ((leftOpType == 0 && rightOpType == 0))
        {
            return 0;
        }
        else
        {
            programHasSemanticError=1;
            printf("\n Incompatible Multiplication");
            return -1;
        }
    }
    if (!strcasecmp(node->value, "AND") || !strcasecmp(node->value, "OR"))
    {
        if (leftOpType == 2 && rightOpType == 2)
        {
            return 2;
        }
        else
        {
            programHasSemanticError=1;
            printf("\n Incompatible AND/OR ");
            return -1;
        }
    }
    if (!strcasecmp(node->value, "LT") || !strcasecmp(node->value, "LE") || !strcasecmp(node->value, "GT") || !strcasecmp(node->value, "GE") || !strcasecmp(node->value, "EQ") || !strcasecmp(node->value, "NE"))
    {
        if ((leftOpType == 0 && rightOpType == 0) || (leftOpType == 1 && rightOpType == 1))
        {
            
            return 2;
        }
        else
        {
            programHasSemanticError=1;
            printf("\n Incompatible relational op \n");
            return -1;
        }
    }
}

int check_assignment(struct treeNode *root, struct id_symbol_table *table) //-1 error, 1 fine, root = LVALUEID/LVALUEARR
{
    if (root == NULL)
    {
        printf("\n root null inside check_assignment"); 
    }
    int lval;
    struct treeNode *lchild = root->children;
    // printf("\nRoot is:%s \n",root->value);
    if (!strcasecmp(root->value, "LVALUEID"))
    { // lchild will be id

        if(!strcmp(root->children->value,"ID") &&!strcmp(root->children->astnextSibling->value,"ID")){
        //printf("\nYAHA POHOHCHA\n");
        struct treeNode* lchild = root->children;
        struct treeNode* rchild = root->children->astnextSibling;
        ST_ENTRY* lentry = get_lexeme(table,lchild->tk_data.lexeme,lchild->line_no);
        ST_ENTRY* rentry = get_lexeme(table,rchild->tk_data.lexeme,rchild->line_no);
        if(lentry==NULL || rentry==NULL)return -1;
        if(lentry!=NULL && rentry!=NULL){
            lchild->symbol_table_entry=lentry;
            rchild->symbol_table_entry=rentry;
            // printf("\n**Entered here for:%s ",lchild->tk_data.lexeme);
            if(lentry->is_array ==1 && rentry->is_array == 1){
                // printf("\n inside is array \n");
                if(!strcasecmp(lentry->type.arr_type.arr_dt,rentry->type.arr_type.arr_dt)){
                    // printf("\n inside is array type match \n");
                    if(lentry->type.arr_type.isStatic==1 && rentry->type.arr_type.isStatic==1){
                        // printf("\n inside is array static \n");
                        int lr1 = lentry->type.arr_type.lowRange.start;
                        int lr2 = rentry->type.arr_type.lowRange.start;
                        int hr1 = lentry->type.arr_type.highRange.end;
                        int hr2 = rentry->type.arr_type.highRange.end;
                        if((hr1-lr1)==(hr2-lr2)){
                            // printf("\n Assignment statement type match(array) line no : %d \n ",root->children->line_no);
                            return 1;
                        }
                    }
                }
                else{
                    programHasSemanticError=1;
                    printf("\n ***Error: Assignment statement type mismatch(array) line no : %d****\n ",root->children->line_no);
                    return -1;
                }
            }

            if(lentry->is_array + rentry->is_array > 0){
                programHasSemanticError=1;
                printf("\n ****Error:Assignment statement type mismatch(array - id) line no : %d**** ",root->children->line_no);
                return -1;
            }
        }
    }
     
        ST_ENTRY *temp = get_lexeme(table, lchild->tk_data.lexeme,lchild->line_no);
        if(temp==NULL){
            return -1;
        }
        lchild->symbol_table_entry=temp;
        char *datatype;
        if(temp->is_for == 1)
            datatype = "INTEGER";
        else if(temp->is_array == 1){
            programHasSemanticError=1;
            printf("\n****Error:Array operand not allowed, line no : %d**** \n",lchild->line_no);
            return -1;
        }
        else 
            datatype = temp->type.id_type.id_dt;
        if (!strcasecmp(datatype, "INTEGER"))
        {
            lval = 0;
        }
        else if (!strcasecmp(datatype, "REAL"))
        {
            lval = 1;
        }
        else if (!strcasecmp(datatype, "BOOLEAN"))
        {
            lval = 2;
        }
        

        
        //printf("\nIDHAR POHOCHA?\n");
        int rval = get_type(lchild->astnextSibling, table);
       // printf("\nYES POHOCHA\n");
        // printf("rval is:%d \n",rval);
        if (lval == rval)
        {
            // printf("\n Assignment statement type match, line no : %d \n",root->children->line_no);
            return 1;
        }
        else
        {
            programHasSemanticError=1;
            printf("\n****Error: Assignment statement type mis-match, line no : %d**** \n",root->children->line_no);
            return -1;
        }
    }
    else if (!strcasecmp(root->value, "LVALUEARRAY"))
    { // lchild will be array access
    
        ST_ENTRY *temp = get_lexeme(table, lchild->children->tk_data.lexeme,lchild->children->line_no);
        if(temp==NULL){
            return -1;
        }
        char *datatype = temp->type.arr_type.arr_dt;
        if (!strcasecmp(datatype, "INTEGER"))
        {
            lval = 0;
        }
        else if (!strcasecmp(datatype, "REAL"))
        {
            lval = 1;
        }
        else if (!strcasecmp(datatype, "BOOLEAN"))
        {
            lval = 2;
        }

        // check array bounds if array index is a num
        int lr = temp->type.arr_type.lowRange.start;
        int hr = temp->type.arr_type.highRange.end;
        if (!strcasecmp(root->children->children->astnextSibling->value, "NUM"))
        {
            int arr_index = root->children->children->astnextSibling->tk_data.val;
            if (arr_index >= lr && arr_index <= hr)
            {
                // printf("\n Array index: %d falls in range, ", root->children->children->astnextSibling->tk_data.val);

            }
            else
            {
                programHasSemanticError=1;
                printf("\n****Error: Array index: %d not in range, line no: %d**** \n", root->children->children->astnextSibling->tk_data.val,lchild->children->line_no);
                return -1;
            }
        }
        //array expr check
        int array_expr_type = get_type(root->children->children->astnextSibling, table);

        if (array_expr_type != 0)
        {
            programHasSemanticError=1;
            printf("\n Array index is not integer \n");
            return -1;
        }
        
        int rval = get_type(lchild->astnextSibling, table);
        if (lval == rval)
        {
            // printf("\n Assignment statement type match, types = %d, line no: %d ", lval,root->children->children->line_no);
            return 1;
        }
        else
        {
            // printf("\n****Error: Assignment statement type mis-match, lval type = %d, rval type = %d,line no %d ****", lval, rval,root->children->children->line_no);
            printf("\n****Error: Assignment statement type mis-match, line no %d ****",root->children->children->line_no);
            programHasSemanticError=1;
            return -1;
        }
    }
}

int check_while(struct treeNode *root, struct id_symbol_table *table)
{
    // Rule is : iterativeStmt.addr = make_new_node("WHILE",arithmeticOrBooleanExpr.addr,statements.syn);
    int type = get_type(root->children, table);
    struct treeNode* temp = root->children;

    if (type == 2)
    {
        // printf("\n While condition is boolean \n");
        return 1;
    }
    else
    {
        programHasSemanticError=1;
        printf("\n While condition is not boolean \n");
        return -1;
    }
}

int check_function_call(struct treeNode *root, struct id_symbol_table *table)
{
    //printf("\nEntered here\n");
    // Semantic rule is : moduleReuseStmt=make_new_node(“MODULE-INVOKE”,ID.addr,(make_new_node(“ARGUMENTS”,optional.syn,idList.syn));
    FN_ENTRY *func = get_func_name(fn_table, root->children->tk_data.lexeme);
    if (func == NULL)
    {
        programHasSemanticError=1;
        printf("\n****Error:Function not defined/declared,line no : %d****\n",root->children->line_no);
        return -1;
    }
    
    //function cant be invoked recursively
    if(table->parent_function!=NULL){
        FN_ENTRY* fn = table->parent_function;
        if(!strcasecmp(func->fn_name,fn->fn_name)){
            programHasSemanticError=1;
            printf("\n****Error: Recursive function call line no : %d****\n ",root->children->line_no);
            return -1;
        }
    }
    if(func->is_declared==1 && func->ip_head==NULL){
        func->is_valid=1;
    }
    //printf("Function is:%s and is_declared:%d \n",func->fn_name,func->is_declared);
    if(func->is_declared == 1 && func->ip_head!=NULL && func->is_valid!=1){
        func->is_valid = 0;
        programHasSemanticError=1;
        printf("\n****Error:Redundant Defintion of function %s, line no:%d****\n",func->fn_name,root->children->line_no);
        return -1;
    }
    
    struct treeNode *called_op_list = NULL;
    struct treeNode *called_ip_list = NULL;
    if(root->children->astnextSibling->children->astnextSibling !=NULL){
        
    called_op_list = root->children->astnextSibling->children;
    called_ip_list = root->children->astnextSibling->children->astnextSibling;
    }
    else{
        // The case where opLIst is null so first child will be ip list
        called_op_list = NULL;
        called_ip_list = root->children->astnextSibling->children;
    }
    //
    FN_ENTRY *func2 = get_func_name(fn_table_pass1, root->children->tk_data.lexeme);
    int comparison_flag_type_checking = 0;
    LISTNODE *defined_op_list = func2->op_head;
    LISTNODE *defined_ip_list = func2->ip_head;

    //printf("Before output list\n");
    // checking if output params in both lists have the same type
    // printf("\n line 435\n");
    while (defined_op_list != NULL)
    {
        //printf("checking o/p: %s \n",defined_op_list->parameter_name);
        if (called_op_list == NULL)
        { // if one is null but the other list is not null
            comparison_flag_type_checking = 1;
            programHasSemanticError=1;
            printf("No of parameters is different in function call, line no: %d \n", root->children->line_no);
            return -1;
        }
        
        // printf("\n inside check function call, the lexeme going into get_lexeme is '%s' \n",called_op_list->tk_data.lexeme);
        ST_ENTRY *op_entry = get_lexeme(table, called_op_list->tk_data.lexeme,called_op_list->line_no);
        if(op_entry==NULL){
            return -1;
        }
        // if(op_entry==NULL)printf("\n OP ENTRY IS NULL \n");
        if(op_entry->is_for==1){
            programHasSemanticError=1;
            printf("\n****Error: For loop variable cannot be an output parameter, line no: %d ****\n", root->children->line_no);
            return -1;
            if(strcasecmp(defined_op_list->parameter_type.id_type.id_dt,"INTEGER")){
                comparison_flag_type_checking = 1;
                printf("\n*****Error: type doesnt match in function call, line no: %d***** \n", root->children->line_no);
                return -1;
            }
        }
        else if(op_entry->is_array == 1){
            comparison_flag_type_checking = 1;
            programHasSemanticError=1;
            printf("\n**** Error: Array cannot be an output parameter, line no: %d ****\n", root->children->line_no);
            return -1;
        }
        else if (strcasecmp(defined_op_list->parameter_type.id_type.id_dt, op_entry->type.id_type.id_dt)) // if comparison results in non 0 meaning both arent the same hence error
        {
            comparison_flag_type_checking = 1;
            programHasSemanticError=1;
            printf("\n****Error: type doesnt match in function call, line no: %d ****\n", root->children->line_no);
            return -1;
        }
        //printf("\nEnd of while,%s\n",defined_op_list->parameter_name);
        called_op_list = called_op_list->next;
        defined_op_list = defined_op_list->next;
        if (defined_op_list == NULL && called_op_list != NULL)
        { // if one is null but the other is not null
            comparison_flag_type_checking = 1;
            printf("No of parameters is different in function call, line no: %d \n", root->children->line_no);
            programHasSemanticError=1;
            return -1;
        }
    }

    if((defined_op_list==NULL && called_op_list!=NULL) || (defined_op_list!=NULL && called_op_list==NULL)){
        comparison_flag_type_checking = 1;
        programHasSemanticError=1;
        printf("No of parameters is different in function call, line no: %d \n", root->children->line_no);
        return -1;
    }

    //printf("\nBefore checking\n");
    // checking if input params in both lists have the same type
    // printf("\n line 482 \n");
    while (defined_ip_list != NULL)
    {
        //printf("\nchecking: \n");
        // printf("\n line 485 \n");
        if (called_ip_list == NULL)
        { // if one is null but the other list is not null
            comparison_flag_type_checking = 1;
            programHasSemanticError=1;
            printf("No of parameters is different in function call, line no: %d \n", root->children->line_no);
            return -1;
        }
        // printf("\n line 493 \n");
        struct treeNode *actual_child = NULL;
        if(called_ip_list->children->astnextSibling==NULL){
              actual_child = called_ip_list->children;
        }
        else actual_child = called_ip_list->children->astnextSibling;

        // Flag to choose which struct to access to obtain type
        int flag_for_type = 1; // 1- to access id struct, 2 to access array struct
        if (!strcasecmp(actual_child->value, "ARRAY_ACCESS"))
        {
            // printf("\n line 504 \n");
            flag_for_type = 2;
        }
        if (flag_for_type == 1) // if comparison results in non 0 meaning both arent the same hence error
        {
            // printf("\n line 509 \n");
            if(!strcasecmp(actual_child->value,"ID")){
                // printf("\n line 511 \n");
                // printf("\n inside check function call, the lexeme going into get_lexeme for ip_entry is '%s' \n",actual_child->tk_data.lexeme);
                ST_ENTRY *ip_entry = get_lexeme(table, actual_child->tk_data.lexeme,actual_child->line_no);
                if(ip_entry==NULL){
                    // printf("\n line 514 \n");
                    return -1;
                }
                
                else if(ip_entry->is_for==1){
                    // printf("\n line 518 \n");
                    if(strcasecmp(defined_ip_list->parameter_type.id_type.id_dt,"INTEGER"))
                        comparison_flag_type_checking =1;
                }
                else if(ip_entry->is_array==1){
                    // printf("\n line 517 \n");
                    if(defined_ip_list->is_array==1){
                        if(strcasecmp(defined_ip_list->parameter_type.arr_type.arr_dt,ip_entry->type.arr_type.arr_dt))
                            comparison_flag_type_checking =1;
                        if(defined_ip_list->parameter_type.arr_type.isStatic==1 && ip_entry->type.arr_type.isStatic==1){
                            if(defined_ip_list->parameter_type.arr_type.lowRange.start!=ip_entry->type.arr_type.lowRange.start || defined_ip_list->parameter_type.arr_type.highRange.end!=ip_entry->type.arr_type.highRange.end)
                                comparison_flag_type_checking =1;
                        }
                    }
                    else{
                        comparison_flag_type_checking =1;
                    }
                    // printf("\n line 529 \n");
                }
                else if(defined_ip_list->is_array==1){
                    comparison_flag_type_checking =1;
                }
                else{
                    // printf("\n line 532:%s\n",defined_ip_list->parameter_name);
                if(strcasecmp(defined_ip_list->parameter_type.id_type.id_dt,ip_entry->type.id_type.id_dt))
                    comparison_flag_type_checking =1;
                }
                actual_child->symbol_table_entry=ip_entry;
                // printf("\n line 543 \n");
            }
            // printf("\n line 536 \n");
            if(!strcasecmp(actual_child->value,"NUM")){
                if(strcasecmp(defined_ip_list->parameter_type.id_type.id_dt,"INTEGER"))
                    comparison_flag_type_checking =1;
            }
            if(!strcasecmp(actual_child->value,"RNUM")){
                if(strcasecmp(defined_ip_list->parameter_type.id_type.id_dt,"REAL"))
                    comparison_flag_type_checking =1;
            }
            // printf("\n line 545 \n");
            if(comparison_flag_type_checking){
                // printf("\nbefore error\n");
                programHasSemanticError=1;
                printf("\n ****Error:type doesnt match in function call, line no: %d**** \n", root->children->line_no);
                // printf("\n after error 549\n ");
                return -1;
            }
            // printf("\n lin 552\n");
        }
        else if (flag_for_type == 2) // if comparison results in non 0 meaning both arent the same hence error
        {

            
            ST_ENTRY *ip_entry = get_lexeme(table, actual_child->children->tk_data.lexeme,actual_child->children->line_no);
            if(ip_entry==NULL){
                    return -1;
                }
            if(strcasecmp(defined_ip_list->parameter_type.id_type.id_dt,ip_entry->type.arr_type.arr_dt)){
                comparison_flag_type_checking =1;
            }
            if(comparison_flag_type_checking){
                printf("\n ****Error:type doesnt match in function call, line no: %d ****\n", root->children->line_no);
                programHasSemanticError=1;
                return -1;
            }
            int lr = ip_entry->type.arr_type.lowRange.start;
            int hr = ip_entry->type.arr_type.highRange.end;
            if (!strcasecmp(actual_child->children->astnextSibling->value, "NUM"))
            {
                int arr_index = actual_child->children->astnextSibling->tk_data.val;
                if (arr_index >= lr && arr_index <= hr)
                {
                    // printf("\n Array index: %d falls in range", actual_child->children->astnextSibling->tk_data.val);

                }
                else
                {
                    programHasSemanticError=1;
                    printf("****Error:\n Array index: %d not in range****,line no : %d \n ", actual_child->children->astnextSibling->tk_data.val,actual_child->children->line_no);
                    return -1;
                }
            }
            
        }
        // printf("\n line 586 \n");
        called_ip_list = called_ip_list->next;
        defined_ip_list = defined_ip_list->next;
        if (defined_ip_list == NULL && called_ip_list != NULL)
        { // if one is null but the other is not null
            comparison_flag_type_checking = 1;
            programHasSemanticError=1;
            printf("No of parameters is different in function call, line no: %d \n", root->children->line_no);
            return -1;
        }
        // printf("\n line 595 \n");
    }

    if((defined_ip_list!=NULL && called_ip_list==NULL) || (defined_ip_list==NULL && called_ip_list!=NULL)){
        comparison_flag_type_checking = 1;
        programHasSemanticError=1;
        printf("No of parameters is different in function call, line no: %d \n", root->children->line_no);
        return -1;
    }


    // printf("\n before all types match \n");
    // printf("\n All types match in function call, line no: %d \n", root->children->line_no);
    return 1; // all types match
}

int check_switch_case(struct treeNode *root, struct id_symbol_table *table)
{
    // printf("Entered type check - switch case \n");
    // conditionalStmt.syn = make_new_node(“SWITCH”,ID.addr, caseStmt.syn,default.syn);
    struct treeNode *switch_id = root->children;
    ST_ENTRY *temp = get_lexeme(table, switch_id->tk_data.lexeme,switch_id->line_no);
    if(temp==NULL){
        return -1;
    }
    root->children->symbol_table_entry=temp; //setting symbol table entry for switch var
    char *datatype;
    if(temp->is_for==1){
         datatype = "INTEGER";
    }
    else{
        datatype = temp->type.id_type.id_dt;
    }
   
    int switch_type;
    if (!strcasecmp(datatype, "REAL"))
    {
        programHasSemanticError=1;
        printf("\n***Error:Switch case cant have real value,line no:%d****\n",root->children->line_no); 
        return -1;
    }
    else
    {
        if (!strcasecmp(datatype, "INTEGER"))
        {
            switch_type = 0;
        }
        else
        {
            switch_type = 2;
        }
        struct treeNode *caseList = switch_id->astnextSibling;

        while (caseList != NULL)
        {
            if (!strcasecmp(caseList->value, "NUM"))
            {
                // if ((switch_type == 0))
                // {
                //     return 1;
                // }
                if(switch_type!=0)
                {
                    programHasSemanticError=1;
                    printf("\n*** Error:Switch type is not integer, case type is integer %d***\n",root->children->line_no);
                    return -1;
                }
            }
            else if (!strcasecmp(caseList->value, "TRUE") || !strcasecmp(caseList->value, "FALSE"))
            {
                // if (switch_type == 2)
                // {
                //     return 1;
                // }
                if(switch_type!=2)
                {
                    programHasSemanticError=1;
                    printf("\n***Error:Switch type is not boolean, case type is boolean, line no : %d***\n",root->children->line_no);
                    return -1;
                }
            }
            caseList = caseList->next;
        }
        struct treeNode *defaultNode = switch_id->astnextSibling->astnextSibling;
        // printf("Switch type is:%d \n",switch_type);
        if (switch_type == 0)
        {
            if (defaultNode == NULL)
            {
                programHasSemanticError=1;
                printf("\n****Error:Switch cases for integer type must have default statement,line no: %d****\n",root->children->line_no);
                return -1;
            }
        }
        if (switch_type == 2)
        {
            if (defaultNode != NULL)
            {
                programHasSemanticError=1;
                printf("\n****Error:Switch cases for boolean type must not have default statement, line no : %d***\n",root->children->line_no);
                return -1;
            }
        }
    }
    return 1;
}

void check_for_variable(struct treeNode* root,struct id_symbol_table* table){
    
    char* str = root->children->tk_data.lexeme;
    //
    int hash_value= get_sym_table_hash(str);
    struct id_symbol_table* temp_table = table;
    while(temp_table!=NULL){
        if(temp_table->arr[hash_value]!=NULL){
            ST_ENTRY* temp = temp_table->arr[hash_value];
            while(temp!=NULL){
                if(!strcmp(temp->id_lexeme,str)){
                    if(temp->is_for==1){
                        programHasSemanticError=1;
                        printf("\n****Error:FOR's variable cannot be assigned a value, line no : %d**** \n",root->children->line_no);
                    }
                }
                temp = temp->next;
            }
        }
        temp_table = temp_table->parent_table; 
    }
    //
}

int check_while_assignment(struct treeNode* root,struct id_symbol_table* table,int expr_nesting){
    if(root == NULL){
        return 0;
    }
    if(!strcasecmp(root->value,"ID")){
        
        int hash_value = get_sym_table_hash(root->tk_data.lexeme);
        ST_ENTRY* temp = table->arr[hash_value];
            while(temp!=NULL){
                if(!strcmp(temp->id_lexeme,root->tk_data.lexeme)){
                    // printf("While condn being checked for:%s and last assigned nesting:%d \n",temp->id_lexeme,temp->last_assigned_nesting);
                    if(temp->last_assigned_nesting>=expr_nesting && temp->is_array==0){
                        return 1;
                    }
                }
                temp = temp->next;
            }
    }
    int flag = check_while_assignment(root->children,table,expr_nesting);
    if(root->children!=NULL){
        flag = flag || check_while_assignment(root->children->astnextSibling,table,expr_nesting);
    }
    return flag;
}

void semanticAnalysis(struct treeNode *root, struct id_symbol_table *id_table, int nesting_num, int offset )
{
    if (root == NULL || root->entry_visited == 1)
        return;
    root->entry_visited = 1;
    //printf("Entered for:%s\n",root->value);
    int type_check;
    // if(!strcmp(root->value,"ID"))
    //     printf("\nsemantic analysis called on : %s\n",root->tk_data.lexeme);
    
    /*pass symbol table in which data is to be inserted in the function call
    Constructs to be checked:- MODULE_DEF,FOR,DECLARE*/
    semanticAnalysis(root->children, id_table, nesting_num, offset);

    if (root->addr != NULL)
    {
        semanticAnalysis(root->addr, id_table, nesting_num,offset);
    }
    else
    {
        // check for respective nodes
        if (!strcasecmp(root->value, "MODULE_DEF"))
        {
            //printf("MODULE DEF FOR:%s\n",root->children->tk_data.lexeme);
            // check for pair
            struct treeNode *child1 = root->children;         // ID
            struct treeNode *child2 = child1->astnextSibling; // ip_list
            struct treeNode *child3 = child2->astnextSibling; // op_list
            struct treeNode *child4 = NULL;                         // module_def
            LISTNODE *ip_list = makeListNode(child2,0);
            struct treeNode* stmts = NULL;
            LISTNODE *op_list=NULL;
            struct id_symbol_table * child_table;
            if (child3 != NULL)
            {
                child4 = child3->astnextSibling;
                if (child4 == NULL)
                { //either module def is null, or op_list is null
                    if (child3->pair == NULL)
                    { //op_list is null,module def non null
                        stmts = child3;
                        FN_ENTRY *fn_entry = create_entry_and_insert_in_FST(fn_table, child1, ip_list, NULL,0);
                        if(fn_entry == NULL){
                            root = root->next;
                            if (root != NULL)
                            {
                                semanticAnalysis(root, id_table, nesting_num, 0);
                            }
                            return;
                        }
                        // id-symbol table
                        child_table = initST(nesting_num);
                        child_table->scope_start=child1->line_no;
                        child_table->parent_function = fn_entry;
                        if (fn_entry->child_table == NULL)
                            fn_entry->child_table = child_table;
                        else
                        {
                            struct id_symbol_table *temp = fn_entry->child_table;
                            if (temp->right_sibling != NULL)
                            {
                                temp = temp->right_sibling;
                            }
                            child_table->left_sibling = temp;
                            temp->right_sibling = child_table;
                        }

                        // change table being passed in function call
                        //id_table = child_table;
                    }
                    else
                    { //module def null ,op list non null
                        op_list = makeListNode(child3,1);
                        create_entry_and_insert_in_FST(fn_table, child1, ip_list, op_list,0);
                    }
                }
                else
                {//both module def and op_list non null
                    stmts = child4;
                    op_list = makeListNode(child3,1);
                    FN_ENTRY *fn_entry = create_entry_and_insert_in_FST(fn_table, child1, ip_list, op_list,0);
                    if(fn_entry == NULL){
                        root = root->next;
                        if (root != NULL)
                        {
                            semanticAnalysis(root, id_table, nesting_num, 0);
                        }
                        return;
                    }
                    //printf("\n Entry created for:%s \n",child1->tk_data.lexeme);
                    // id-symbol table
                    child_table = initST(nesting_num);
                    child_table->parent_function = fn_entry;
                    child_table->scope_start=child1->line_no;
                    if (fn_entry->child_table == NULL)
                        fn_entry->child_table = child_table;
                    else
                    {
                        struct id_symbol_table *temp = fn_entry->child_table;
                        if (temp->right_sibling != NULL)
                        {
                            temp = temp->right_sibling;
                        }
                        child_table->left_sibling = temp;
                        temp->right_sibling = child_table;
                    }
                    // change table being passed in function call
                    //id_table = child_table; 

                }
            }
            else
            {//both module def and op_list is null
                create_entry_and_insert_in_FST(fn_table, child1, ip_list, NULL,0);
            }

            //fill symbol table for i/p list
            LISTNODE* temp = ip_list;
            while(temp!=NULL){
                char* name = temp->parameter_name;
                int hash_value = get_sym_table_hash(name);
                if(child_table->arr[hash_value]!=NULL){
            ST_ENTRY* curr = child_table->arr[hash_value];
            while(curr){
                if(!strcmp(curr->id_lexeme,name)){
                    programHasSemanticError=1;
                    printf("\n Input list cannot have same variable appearing more than once %s \n",name);
                    return;
                }
                curr = curr->next;
            }
        }

        ST_ENTRY* block = malloc(sizeof(ST_ENTRY));
        block->id_lexeme = name;
        block->next = NULL;
        block->type = temp->parameter_type;
        block->is_array=temp->is_array;
        block->is_for=0;
        block->isList=1;
        if(temp->is_array==1){
            if(temp->parameter_type.arr_type.isStatic==1){
                block->offset=offset;
                // printf("INPUT LIST : Offset for:%s in ip list is:%d \n",name,offset);
                int lr = temp->parameter_type.arr_type.lowRange.start;
                int hr = temp->parameter_type.arr_type.highRange.end;
                if(lr >hr){
                    printf("Error: Low Range should be smaller than high range\n");
                    programHasSemanticError=1;
                }
                if(!strcasecmp(temp->parameter_type.arr_type.arr_dt,"INTEGER")){
                    block->width=2*(hr-lr+1);
                    offset+=2*(hr-lr+1);
                }
                else if(!strcasecmp(temp->parameter_type.arr_type.arr_dt,"REAL")){
                    block->width=4*(hr-lr+1);
                    offset+=4*(hr-lr+1);
                }
                else if(!strcasecmp(temp->parameter_type.arr_type.arr_dt,"BOOLEAN")){
                    block->width=(hr-lr+1);
                    offset+=(hr-lr+1);
                }
            }
        }
        else{
            //is id
            block->offset = offset;
            // printf("INPUT LIST : Offset for:%s in ip list is:%d \n",name,offset);

            char* datatype = temp->parameter_type.id_type.id_dt;
            if(!strcasecmp(temp->parameter_type.id_type.id_dt,"INTEGER")){
                offset+=2;
                block->width = 2;
            }
            else if(!strcasecmp(temp->parameter_type.id_type.id_dt,"REAL")){
                offset += 4;
                block->width = 4;
            }
            else if (!strcasecmp(temp->parameter_type.id_type.id_dt,"BOOLEAN"))
            {
                offset += 1;
                block->width = 1;
            } 
        }

        if(global_max_offset < offset)
            global_max_offset = offset;

        insert_in_table(child_table,block);
        // printf("Entry done for:%s, line no : %d \n",name,root->children->line_no);
        temp = temp->next;
        }
       // printf("After creating entries\n");

            //fill symbol table for o/p list

        LISTNODE* temp2 = op_list;
        // if(temp2!=NULL){
            
        //     printf("ye andar nhi ghusega \n");
        // }
        while(temp2!=NULL){
            char* name = temp2->parameter_name;
            int hash_value = get_sym_table_hash(name);
            if(child_table->arr[hash_value]!=NULL){
                ST_ENTRY* curr = child_table->arr[hash_value];
                while(curr){
                    if(!strcmp(curr->id_lexeme,name)){
                        printf("\n Output list cannot have same variable appearing more than once %s \n",name);
                        programHasSemanticError=1;
                        return;
                    }
                    curr = curr->next;
                }
            }

            ST_ENTRY* block = malloc(sizeof(ST_ENTRY));
            block->id_lexeme = name;
            block->next = NULL;
            block->type = temp2->parameter_type;
            block->is_array=temp2->is_array;
            block->isList=1;

            if(temp2->is_array==1){
            if(temp2->parameter_type.arr_type.isStatic==1){
                block->offset=offset;
                // printf("OUTPUT LIST : Offset for:%s in op list is:%d \n",name,offset);

                int lr = temp2->parameter_type.arr_type.lowRange.start;
                int hr = temp2->parameter_type.arr_type.highRange.end;
                if(!strcasecmp(temp2->parameter_type.arr_type.arr_dt,"INTEGER")){
                    block->width=2*(hr-lr+1);
                    offset+=2*(hr-lr+1);
                }
                else if(!strcasecmp(temp2->parameter_type.arr_type.arr_dt,"REAL")){
                    block->width=4*(hr-lr+1);
                    offset+=4*(hr-lr+1);
                }
                else if(!strcasecmp(temp2->parameter_type.arr_type.arr_dt,"BOOLEAN")){
                    block->width=(hr-lr+1);
                    offset+=(hr-lr+1);
                }
            }
        }
        else{
            //is id
            block->offset = offset;
            // printf("OUTPUT LIST : Offset for:%s in ip list is:%d \n",name,offset);

            char* datatype = temp2->parameter_type.id_type.id_dt;
            if(!strcasecmp(temp2->parameter_type.id_type.id_dt,"INTEGER")){
                offset+=2;
                block->width = 2;
            }
            else if(!strcasecmp(temp2->parameter_type.id_type.id_dt,"REAL")){
                offset += 4;
                block->width = 4;
            }
            else if (!strcasecmp(temp2->parameter_type.id_type.id_dt,"BOOLEAN"))
            {
                offset += 1;
                block->width = 1;
            } 
        }

            if(global_max_offset < offset)
                global_max_offset = offset;
            insert_in_table(child_table,block);
            // printf("Entry done for:%s, %d \n",name,root->children->line_no);
            temp2 = temp2->next;
        }

        //printf("\nAfter output list\n");

        while(stmts!=NULL){
            semanticAnalysis(stmts,child_table,nesting_num,offset);
            if(stmts->next==NULL){//if end is switch/for/while show's end line no as the start line no of swicth/for/while
                // child_table->scope_end = stmts->line_no;
            }
            stmts = stmts->next;
        }

        //
        //bottom up here
        //printf("Entered here\n");
        LISTNODE* temp3 = op_list;
        while(temp3!=NULL){
            char* op_param = temp3->parameter_name;
            ST_ENTRY* t = NULL;
            //
            int hash_value= get_sym_table_hash(op_param);
            struct id_symbol_table* temp_table = child_table;
            while(temp_table!=NULL){
                if(temp_table->arr[hash_value]!=NULL){
                    ST_ENTRY* temp = temp_table->arr[hash_value];
                    while(temp!=NULL){
                        if(!strcmp(temp->id_lexeme,op_param)){
                            t = temp;
                        }
                        temp = temp->next;
                    }
                }
                temp_table = temp_table->parent_table;
            }
            //
            if(t!=NULL && t->is_used!=1){
                programHasSemanticError=1;
                printf("\n****Error: Output param:%s not assigned value inside the function %s****\n",op_param,child_table->parent_function->fn_name);
            }
            temp3 = temp3->next;
        }
        //

        root = root->next;
        if (root != NULL)
        {
            semanticAnalysis(root, id_table, nesting_num,0);
        }

        return;
        }
        else if(!strcasecmp(root->value,"PROGRAM")){
            struct treeNode* leftChild = root->children;

            // module declaration hai, CHANGE HUA HAI IDHAR
            if(leftChild!=NULL && !strcmp(leftChild->value,"ID")){
                while(leftChild!=NULL){
                    //printf("FUNC DECLARE:%s \n",leftChild->tk_data.lexeme);
                    FN_ENTRY* fn_declare = create_entry_and_insert_in_FST(fn_table,leftChild,NULL,NULL,0); //set i/p and o/p as NULL
                    fn_declare->is_declared = 1;
                    leftChild = leftChild->next;
                }
            }

            /// 
            struct treeNode* driver;
            if(root->children!=NULL){
                if(!strcasecmp(root->children->value,"ID")){
                    struct treeNode* t1 = root->children;
                    if(t1->astnextSibling!=NULL && !strcmp(t1->astnextSibling->value,"MODULE_DEF")){
                        driver = t1->astnextSibling->astnextSibling;

                    }
                    else driver = t1->astnextSibling;
                }
                else if(!strcasecmp(root->children->value,"MODULE_DEF")){
                    driver = root->children->astnextSibling;
                }
                else driver = root->children;

                
                // struct treeNode* tempdriver = driver;
                // while(tempdriver!=NULL){
                //     if(tempdriver->next==NULL){
                //         struct treeNode* temp5 = tempdriver;
                //         while(temp5!=NULL){
                //             if(temp5->isTerminal==1){
                //                 id_table->scope_end = temp5->line_no;
                //                 break;
                //             }
                //             temp5 = temp5->children;
                //         }
                //     }
                //     tempdriver = tempdriver->next;
                // }
                

                while(driver!=NULL){
                      if(driver->isTerminal==1){
                            id_table->scope_start = driver->line_no;
                            break;
                      }
                    driver=driver->children;
                }   
            }
        }
    
        
        else if (!strcasecmp(root->value, "FOR"))
        {
            TYPE type;
            struct treeNode *rangeNode = root->children->astnextSibling;
            if(rangeNode==NULL){
                printf("Range node is null\n");
                exit(0);
            }
            if (!strcasecmp(rangeNode->children->children->value, "NUM"))
            {
                type.for_type.low_range = rangeNode->children->children->tk_data.val;
            }
            else
            {
                if (!strcasecmp(rangeNode->children->children->value, "MINUS"))
                {
                    type.for_type.low_range = -1 * (rangeNode->children->children->astnextSibling->tk_data.val);
                }
                else
                {
                    type.for_type.low_range = rangeNode->children->children->astnextSibling->tk_data.val;
                }
            }

            if (!strcasecmp(rangeNode->children->astnextSibling->children->value, "NUM"))
            {
                type.for_type.high_range = rangeNode->children->astnextSibling->children->tk_data.val;
            }
            else
            {
                if (!strcasecmp(rangeNode->children->astnextSibling->children->value, "MINUS"))
                {
                    type.for_type.high_range = -1 * (rangeNode->children->astnextSibling->children->astnextSibling->tk_data.val);
                }
                else
                {
                    type.for_type.high_range = rangeNode->children->astnextSibling->children->astnextSibling->tk_data.val;
                }
            }

            struct id_symbol_table *child_table = initST(nesting_num + 1);
            child_table->scope_start=root->children->line_no;
            ST_ENTRY* temp = create_entry_and_insert(child_table, root->children, type);
            if(temp!=NULL){
                temp->is_array=0;
                temp->is_for = 1;
                temp->offset=offset;
                // printf("Offset for:%s is %d \n",root->children->tk_data.lexeme,offset);
                offset+=2;
                if(global_max_offset < offset)
                    global_max_offset = offset;
                temp->width=2;
                root->children->symbol_table_entry=temp; //symbol table entry for conditional variable node of "for"
            }
            child_table->parent_table = id_table;
            if (id_table->child_table == NULL)
            {
                id_table->child_table = child_table;
            }
            else
            {
                struct id_symbol_table *temp = id_table->child_table;
                while (temp->right_sibling != NULL)
                {
                    temp = temp->right_sibling;
                }
                child_table->left_sibling = temp;
                temp->right_sibling = child_table;
            }
            //id_table = child_table;


            // Rule is iterativeStmt.addr = make_new_node("FOR",ID.addr ,range_for.addr,statements.syn);
            struct treeNode* child1= root->children;
            struct treeNode* statements = root->children->astnextSibling->astnextSibling;
            int flag_for_for_in_semantic_analysis = 1;
            while(statements!=NULL && flag_for_for_in_semantic_analysis){
                if(!strcasecmp(statements->value, "LVALUEID")){
                    if(!strcasecmp(statements->children->tk_data.lexeme,child1->tk_data.lexeme)){
                        // printf("\n ERROR: FOR STATEMENT REDEFINING VARIABLE, LINE NO %d \n",statements->children->line_no);
                        flag_for_for_in_semantic_analysis=0;
                        // if(root->next!=NULL)semanticAnalysis(root->next, child_table, nesting_num);
                        //     return;
                    }
                }
                statements = statements->next;
            }

            struct treeNode* stmts = root->children->astnextSibling->astnextSibling;

            while(stmts!=NULL){
                semanticAnalysis(stmts,child_table,nesting_num+1,offset);
                if(stmts->next==NULL){
                    struct treeNode* temp5 = stmts;
                    while(temp5!=NULL){
                        if(temp5->isTerminal){
                            child_table->scope_end = temp5->line_no;
                            break;
                        }
                        temp5=temp5->children;
                    }
                }
                stmts = stmts->next;
            }

            root = root->next;
            if (root != NULL)
            {
                semanticAnalysis(root, id_table, nesting_num,offset);
            }

            return;
        }
        else if (!strcasecmp(root->value, "DECLARE"))
        {
            struct treeNode *type_attribute = root->children;
            struct treeNode *idList = root->children->astnextSibling;
            TYPE t;
            int array=0;
            if(!strcasecmp(type_attribute->value,"ARRAY-DCL")){
                array=1;
                t.arr_type.arr_dt = type_attribute->children->astnextSibling->tk_data.lexeme;
                struct treeNode* li = type_attribute->children->children;
                struct treeNode* ri = li->astnextSibling;
            if(!strcasecmp(li->children->value,"ID")){
            t.arr_type.lowRange.low_id = li->children->tk_data.lexeme;
            t.arr_type.isStatic = 0; //dynamic array
        }
        else if(!strcasecmp(li->children->value,"NUM")){
            t.arr_type.lowRange.start = li->children->tk_data.val;
            t.arr_type.isStatic = 1; //static array

        }
        else{
            if(!strcasecmp(li->children->astnextSibling->value,"ID"))
            {
                   t.arr_type.lowRange.low_id = strcat(strcat(li->children->value,","),li->children->astnextSibling->tk_data.lexeme);
                   t.arr_type.isStatic = 0; //dynamic array
            }
            else
            {
                if(!strcasecmp(li->children->value,"MINUS"))
                    t.arr_type.lowRange.start = -1*li->children->astnextSibling->tk_data.val;
                else   
                    t.arr_type.lowRange.start = li->children->astnextSibling->tk_data.val;
                t.arr_type.isStatic = 1; //static array
            }
        }
        
        // ri // Changed a few THINGS HERE, TAKE NOTE
        if(!strcasecmp(ri->children->value,"ID")){
            t.arr_type.highRange.high_id = ri->children->tk_data.lexeme;
            t.arr_type.isStatic = 0; //dynamic array
        }
        else if(!strcasecmp(ri->children->value,"NUM")){
            t.arr_type.highRange.end = ri->children->tk_data.val;
            if(!strcmp(li->children->value,"NUM") || ( li->children->astnextSibling!=NULL && !strcmp(li->children->astnextSibling->value,"NUM")))
                t.arr_type.isStatic = 1; //static array
            else
                t.arr_type.isStatic = 0; //dynamic array
        }
        else{
            if(!strcasecmp(ri->children->astnextSibling->value,"ID"))
            {
                   t.arr_type.highRange.high_id = strcat(strcat(ri->children->value,","),ri->children->astnextSibling->tk_data.lexeme);
                   t.arr_type.isStatic = 0; //dynamic array
            }
            else
            {
                if(!strcasecmp(ri->children->value,"MINUS"))
                    t.arr_type.highRange.end = -1*ri->children->astnextSibling->tk_data.val;
                else   
                    t.arr_type.highRange.end = ri->children->astnextSibling->tk_data.val;
                if(!strcmp(li->children->value,"NUM") || ( li->children->astnextSibling!=NULL && !strcmp(li->children->astnextSibling->value,"NUM")))
                    t.arr_type.isStatic = 1; //static array
                else
                    t.arr_type.isStatic = 0; //dynamic array
            }

            
        }
        
            }
            else{
                t.id_type.id_dt = type_attribute->value;
                //printf("****************being populated");
            }
            
            while (idList != NULL)
            {   
                //printf("%s\n",idList->tk_data.lexeme);
                ST_ENTRY* temp = create_entry_and_insert(id_table, idList, t);
                if(temp!=NULL){
                    temp->is_array = array;
                    temp->is_for = 0;
                    if(temp->is_array==1){
                        if(t.arr_type.isStatic==1){
                            int lr =t.arr_type.lowRange.start;
                            int hr =t.arr_type.highRange.end;
                            if(lr > hr){
                                printf("Error: Low Range should be smaller than high range\n");
                                programHasSemanticError=1;
                            }
                            char* datatype = t.arr_type.arr_dt;
                            if(!strcasecmp(datatype,"INTEGER")){
                                temp->offset=offset;
                                offset+=2*(hr-lr+1);
                                temp->width=2*(hr-lr+1);
                            }
                            else if(!strcasecmp(datatype,"REAL")){
                                temp->offset=offset;
                                offset+=4*(hr-lr+1);
                                temp->width=4*(hr-lr+1);
                            }
                            else if(!strcasecmp(datatype,"BOOLEAN")){
                                temp->offset=offset;
                                offset+=(hr-lr+1);
                                temp->width=(hr-lr+1);
                            }
                            
                         }
                    }
                    else{
                        char* datatype = t.id_type.id_dt;
                        if(!strcasecmp(datatype,"INTEGER")){
                            temp->offset=offset;
                            offset+=2;
                            temp->width=2;
                        }
                        else if(!strcasecmp(datatype,"REAL")){
                            temp->offset = offset;
                            offset += 4;
                            temp->width = 4;
                        }
                        else if(!strcasecmp(datatype,"BOOLEAN")){
                            temp->offset=offset;
                            offset+=1;
                            temp->width=1;
                        }
                    }
                    if(global_max_offset < offset)
                        global_max_offset = offset;
                    // printf("Offset for:%s is %d \n",temp->id_lexeme,temp->offset);
                }
                idList = idList->next;
            }
            //printf("After filling offset for declare\n");
        }
        else if (!strcasecmp(root->value, "WHILE"))
        {
            struct treeNode* stmts = root->children->astnextSibling;
            type_check = check_while(root, id_table); // checks if while has a boolean expr
            //UNCOMMENT IF PROBLEMATIC

            // if (type_check == -1)
            // {
            //     if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num);
            //     return;
            // }
            struct id_symbol_table *child_table = initST(nesting_num + 1);
            struct treeNode * temp = root->children;
            while(temp!=NULL){
            if(temp->isTerminal==1){
                child_table->scope_start = temp->line_no;
                // printf("\n while scope starts at:%s in here \n",temp->value);
                break;
            }
            temp = temp->children;
            }
            child_table->parent_table = id_table;
            if (id_table->child_table == NULL)
            {
                id_table->child_table = child_table;
            }
            else
            {
                struct id_symbol_table *temp = id_table->child_table;
                while (temp->right_sibling != NULL)
                {
                    temp = temp->right_sibling;
                }
                child_table->left_sibling = temp;
                temp->right_sibling = child_table;
            }
            //id_table = child_table;
            while(stmts!=NULL){
                semanticAnalysis(stmts,child_table,nesting_num+1,offset);
                if(stmts->next==NULL){
                    struct treeNode* temp5= stmts;
                    while(temp5!=NULL){
                        if(temp5->isTerminal==1){
                            child_table->scope_end = temp5->line_no;
                            break;
                        }
                        temp5 = temp5->children;
                    }
                }
                stmts=stmts->next;
            }

            //traverse expr and check
            struct treeNode* while_expr = root->children;
            int is_while_assigned = check_while_assignment(while_expr,id_table,nesting_num+1);
            if(is_while_assigned == 0){
                printf("\n****Error: No variable in expression of while was assigned a value****\n");
                programHasSemanticError = 1;
            }

            root = root->next;
            if (root != NULL)
            {
                semanticAnalysis(root, id_table, nesting_num, offset);
            }



            return;
        }

        else if (!strcasecmp(root->value, "SWITCH"))
        {
            //printf("INSIDE SWITCH\n");
            type_check = check_switch_case(root, id_table); // checks if while has a boolean expr
           // printf("\nAFTER TYPE CHECK\n");
            //POINT OF FAILURE
            
            // if (type_check == -1)
            // {
            //     if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num);
            //     return;
            // }
            struct treeNode *caseList = root->children->astnextSibling;
            struct treeNode *defaultStmt = caseList->astnextSibling;
            while (caseList != NULL)
            {
                // value->pair = stmts
                struct id_symbol_table *child_table = initST(nesting_num + 1);
                child_table->scope_start=caseList->line_no;
                child_table->parent_table = id_table;
                if (id_table->child_table == NULL)
                {
                    id_table->child_table = child_table;
                }
                else
                {
                    //printf("Sibling being set");
                    struct id_symbol_table *temp = id_table->child_table;
                    while (temp->right_sibling != NULL)
                    {
                        temp = temp->right_sibling;
                    }
                    child_table->left_sibling = temp;
                    temp->right_sibling = child_table;
                }
                //struct id_symbol_table *temp = id_table;
                //id_table = child_table;

                struct treeNode *stmts = caseList->pair;
                while (stmts != NULL)
                {
                    semanticAnalysis(stmts, child_table, nesting_num+1, offset);
                    //printf("\nREACHED HERE\n");
                    // if(stmts->next==NULL){
                    //      struct treeNode* temp5 = stmts;
                    //      while(temp5!=NULL){
                    //         if(temp5->isTerminal==1){
                    //             child_table->scope_end = temp5->line_no+1;
                    //             break;
                    //         }
                    //         temp5=temp5->children;

                    //      }
                    // }
                    stmts = stmts->next;
                }
                // id_table = temp;
                caseList = caseList->next;
                //printf("After this\n");
            }
            //printf("after case\n");
            if(defaultStmt == NULL){
                //printf("Default is NULL \n");
                return;
            }
            //printf("COMES TO DEFAULT\n");
            struct treeNode *stmts = defaultStmt; // right child
            // open new symbol table
            struct id_symbol_table *child_table = initST(nesting_num + 1);
            child_table->parent_table = id_table;
            if (id_table->child_table == NULL)
            {
                id_table->child_table = child_table;
            }
            else
            {
                struct id_symbol_table *temp = id_table->child_table;
                while (temp->right_sibling != NULL)
                {
                    temp = temp->right_sibling;
                }
                child_table->left_sibling = temp;
                temp->right_sibling = child_table;
            }
            int default_line_no_flag = 0;
            while (stmts != NULL)
            {
                if(default_line_no_flag == 0){
                     struct treeNode* temp5 = stmts;
                        while(temp5!=NULL){
                            if(temp5->isTerminal==1){
                                child_table->scope_start = temp5->line_no;
                                default_line_no_flag = 1;
                                break;
                            }
                            temp5=temp5->children;
                        }
                }
                // printf("Default is not NULL \n");
                semanticAnalysis(stmts, child_table, nesting_num+1, offset);
                if(stmts->next==NULL){
                    struct treeNode* temp5 = stmts;
                        while(temp5!=NULL){
                            if(temp5->isTerminal==1){
                                child_table->scope_end = temp5->line_no;
                                break;
                            }
                            temp5=temp5->children;
                        }
                }
                stmts = stmts->next;
            }
            //id_table = child_table;

            root = root->next;
            if (root != NULL)
            {
                semanticAnalysis(root, id_table, nesting_num, offset);
            }

            return;
        }

        else if (!strcasecmp(root->value, "LVALUEID") || !strcasecmp(root->value, "LVALUEARRAY"))
        {          
            //printf("\nINSIDE LVALUEID\n");
            type_check = check_assignment(root, id_table);
            if(!strcasecmp(root->value, "LVALUEID"))
                check_for_variable(root,id_table);
            //printf("\nAFTER CHECK\n");
            
            //---------Point of failure-----//
            // if (type_check == -1)
            // {
               
            //     if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num);
            //     retur
            // }

             // Setting is_used in that function to be one to idnetify that that variable has been assigned a value in that function

            // int hash_value = get_sym_table_hash(root->children->tk_data.lexeme);
            // if (id_table->arr[hash_value] != NULL)
            // {
            //     ST_ENTRY *temp = id_table->arr[hash_value];
            //     int found_entry_for_assign_op = 1;
            //     while (temp && found_entry_for_assign_op)
            //     {
            //         if (!strcasecmp(temp->id_lexeme, root->children->tk_data.lexeme))
            //         {
            //             temp->is_used = 1;
            //             printf("Setting is_used for:%s \n",temp->id_lexeme);
            //             found_entry_for_assign_op = 0;
            //         }
            //         temp = temp->next;
            //     }
            // }

            //
            // printf("\n HERE \n");
            int is_used_set = 1;
            int hash_value= get_sym_table_hash(root->children->tk_data.lexeme);
            struct id_symbol_table* temp_table = id_table;
            while(temp_table!=NULL && is_used_set){
                if(temp_table->arr[hash_value]!=NULL){
                    ST_ENTRY* temp = temp_table->arr[hash_value];
                    while(temp!=NULL && is_used_set){
                        if(!strcmp(temp->id_lexeme,root->children->tk_data.lexeme)){
                            temp->is_used=1;
                            temp->last_assigned_nesting = nesting_num;
                            // printf("Nesting num set to: %d \n",nesting_num);
                            is_used_set = 0;
                        }
                        temp = temp->next;
                    }
                }
                temp_table = temp_table->parent_table;
            }
            // printf("\n HERE AFTER \n");
            //
        }
        // Checking if variable in for is assigned value anywhere inside loop
        

        // function call
        else if (!strcasecmp(root->value, "MODULE-INVOKE"))
        {
            // a)moduleReuseStmt=make_new_node(“MODULE-INVOKE”,ID.addr,(make_new_node(“ARGUMENTS”,optional.syn,idList.syn)); //bottom up

            if(root->children->astnextSibling->children->astnextSibling!=NULL){
                struct treeNode* opList = root->children->astnextSibling->children;
                while(opList!=NULL){
                    ST_ENTRY* temp = get_lexeme(id_table,opList->tk_data.lexeme,opList->line_no);
                    if(temp!=NULL){
                        temp->is_used=1;
                        temp->last_assigned_nesting=nesting_num;
                    }
                    opList = opList->next;
                }
            }
            type_check = check_function_call(root, id_table);
            
            // if (type_check == -1)
            // {
            //     if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num);
            //     return;
            // }

        }

        else if(!strcasecmp(root->value,"GET-VALUE")){
            struct treeNode* getVal = root->children;
            ST_ENTRY* temp = get_lexeme(id_table,getVal->tk_data.lexeme,getVal->line_no);
            if(temp==NULL){
                if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num, offset);
                return;   
            }
            root->children->symbol_table_entry=temp;
            temp->is_used = 1;
            // else if(temp->is_array==1){
            //     printf("\n Error: Get-Value for array \n");
            //     if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num, offset);
            //     return;
            // }
        }

        else if(!strcasecmp(root->value,"PRINT")){
            //can be id/num/rnum/boolvar/arr-print
            ST_ENTRY* temp;
            if(!strcasecmp(root->children->value,"ARR-PRINT")){
                temp=get_lexeme(id_table,root->children->children->tk_data.lexeme,root->children->children->line_no);
                if(temp!=NULL){
                    root->children->children->symbol_table_entry=temp;
                }
            }
            else if(!strcasecmp(root->children->value,"ID")){
                temp = get_lexeme(id_table,root->children->tk_data.lexeme,root->children->line_no);
                if(temp!=NULL){
                    root->children->symbol_table_entry=temp;
                }
                
            }

            if(temp==NULL){
                if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num, offset);
                return;
            }

            // if(temp->is_used!=1){
            //     printf("\nError: Print variable should be assigned some value \n");
            //     if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num, offset);
            //     return;
            // }
        }
    }

    if (root->children != NULL)
    {
        struct treeNode *temp;
        temp = root->children->astnextSibling;
        // z = x + arr;
        while (temp != NULL)
        {
            semanticAnalysis(temp, id_table, nesting_num, offset);
            temp = temp->astnextSibling;
        }
    }
    // printf("\n in line 1507 \n");
    // printf("root is :%s\n",root->value);

    // needs to happen only for statements wala node
    root = root->next;
    if (root != NULL)
    {
        // printf("\n inside root not NULL \n");
        semanticAnalysis(root, id_table, nesting_num, offset);
    }
    
}

void insert_input_output(struct treeNode* root){

    struct treeNode *child1 = root->children;         // ID
    struct treeNode *child2 = child1->astnextSibling; // ip_list
    struct treeNode *child3 = child2->astnextSibling; // op_list
    struct treeNode *child4 = NULL;                   // module_def
    LISTNODE *ip_list = makeListNode(child2,0);
    LISTNODE *op_list=NULL;
    if (child3 != NULL)
    {
        child4 = child3->astnextSibling;
        if (child4 == NULL)
        { //either module def is null, or op_list is null
            if (child3->pair == NULL)
            { //op_list is null,module def non null
                FN_ENTRY *fn_entry = create_entry_and_insert_in_FST(fn_table_pass1, child1, ip_list, NULL,1);
                if(fn_entry == NULL){
                    return;
                }
                fn_entry->defLine = child1->line_no;
            }
            else
            { //module def null ,op list non null
                op_list = makeListNode(child3,1);
                FN_ENTRY *fn_entry = create_entry_and_insert_in_FST(fn_table_pass1, child1, ip_list, op_list,1);
                if(fn_entry==NULL){
                    return;
                }
                fn_entry->defLine = child1->line_no;
            }
        }
        else
        {//both module def and op_list non null
            op_list = makeListNode(child3,1);
            FN_ENTRY *fn_entry = create_entry_and_insert_in_FST(fn_table_pass1, child1, ip_list, op_list,1);
            if(fn_entry == NULL){
                return;
            }
            fn_entry->defLine = child1->line_no;
        }
    }
    else
    {//both module def and op_list is null
        FN_ENTRY* fn_entry = create_entry_and_insert_in_FST(fn_table_pass1, child1, ip_list, NULL,1);
        fn_entry->defLine = child1->line_no;
    }
}

void fillDef(struct treeNode* root){ //fill def and declns 
    // take PROGRAM node as the root
    struct treeNode* childA = root->children;  //can be drivermodule/moduledef/moduledecln
    struct treeNode* childB = NULL;
    if(childA!=NULL)
        childB = childA->astnextSibling; 
    struct treeNode* childC = NULL; 
    struct treeNode* childD = NULL;
    if(childB!=NULL)
        childC = childB->astnextSibling; 
    if(childC!=NULL)
        childD = childC->astnextSibling;

    //this is module declaration    
    if(childA!=NULL && !strcmp(childA->value,"ID")){
        while(childA!=NULL){
            FN_ENTRY* func_entry = create_entry_and_insert_in_FST(fn_table_pass1,childA,NULL,NULL,1); //first pass
            func_entry->is_declared = 1;
            func_entry->declLine = childA->line_no;
            childA = childA->next;
        }
    }

    //printf("After declare\n");

    //this is module def
    if(childA!=NULL && !strcmp(childA->value,"MODULE_DEF")){
        while(childA!=NULL){
            insert_input_output(childA);
            childA = childA->next;
        }
    }
    //printf("After module def\n");
    if(childB!=NULL && !strcmp(childB->value,"MODULE_DEF")){  //child1 is decln and child2 is moduledef or child1 is driver and child2 is moduledef
        while(childB!=NULL){
            insert_input_output(childB);
            childB = childB->next;
        }
    }
    if(childC!=NULL && !strcmp(childC->value,"MODULE_DEF")){  //if child1 is decln, child2 is driver, child3 is moduledef or child1 is moduledef child2 is driver and child3 is moduledef
        while(childC!=NULL){
            insert_input_output(childC);
            childC = childC->next;
        }
    }
    if(childD!=NULL && !strcmp(childD->value,"MODULE_DEF")){ //if child1 is decln child2 is d
        while(childD!=NULL){
            insert_input_output(childD);
            childD = childD->next;
        }
    }
    //printf("\nAfter first pass\n");
}
/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#include "treeDef.h"
#include<string.h>
#include<stdio.h>

int get_sym_table_hash(char* str){
    //implement hash fn
    int n = strlen(str);
    long long p = 31, m = 10e9+7;
    long long hash = 0;
    long long p_pow = 1;
    for (int i = 0; i < n; i++)
    {
        hash = (hash + (str[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return abs(hash)%TABLE_SIZE;
}


void insert_fn_in_table(struct fn_symbol_table* table, FN_ENTRY* entry){
    int hash_value = get_sym_table_hash(entry->fn_name);
    if(table->arr[hash_value]==NULL){
        table->arr[hash_value] = entry;
        return;
    }
    else{
        FN_ENTRY* temp = table->arr[hash_value];
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next = entry;
        return;
    }
}

void insert_in_table(struct id_symbol_table* table,  ST_ENTRY * entry){
    int hash_value = get_sym_table_hash(entry->id_lexeme);
    //printf("\n %s:%d \n",entry->id_lexeme,hash_value);
    if(table->arr[hash_value] == NULL){
        //printf("First-filling:%s \n",entry->id_lexeme);
        table->arr[hash_value]= entry;
        return;
    }
    else{
        ST_ENTRY* temp = table->arr[hash_value];

        //printf("\n Not-vacant:%s ",entry->id_lexeme);
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next = entry;
        entry->next=NULL;
        return;
    }
}
 
FN_ENTRY* get_func_name(struct fn_symbol_table* table, char* str){
    int hash_value= get_sym_table_hash(str);
    if(table->arr[hash_value]==NULL){
        programHasSemanticError=1;
        printf("\n****Error:1. Error: Function %s not declared ****\n",str);
        return NULL;
    }
    else{
        FN_ENTRY* temp = table->arr[hash_value];
        while(temp){
            if(!strcmp(temp->fn_name,str)){
                return temp;
            }
            temp = temp->next;
        }
        programHasSemanticError=1;
        printf("\n****Error:2. Error: Function %s not declared ****\n",str);
        return NULL;
    }
}

ST_ENTRY* get_lexeme(struct id_symbol_table* table, char* str, int line_no){
    int hash_value= get_sym_table_hash(str);
    struct id_symbol_table* temp_table = table;
    while(temp_table!=NULL){
        if(temp_table->arr[hash_value]!=NULL){
            ST_ENTRY* temp = temp_table->arr[hash_value];
            while(temp!=NULL){
                if(!strcmp(temp->id_lexeme,str)){
                    return temp;
                }
                temp = temp->next;
            }
        }
        temp_table = temp_table->parent_table;
        
    }
    programHasSemanticError=1;
    printf("\n****Error: Variable %s not declared,line no : %d **** \n",str,line_no);
    return NULL;
}

ST_ENTRY* create_entry_and_insert(struct id_symbol_table* table,struct treeNode* node,TYPE t1){
        // if(!strcmp(node->tk_data.lexeme,"new"))
            // printf("GHUS GAYA\n");
        if(node==NULL){
            printf("AST Node:%s is null \n",node->value);
            return NULL;
        }
        int redeclare_flag = 0;

        int hash_value= get_sym_table_hash(node->tk_data.lexeme);
        // if(table->arr[hash_value] == NULL && !strcmp(node->tk_data.lexeme,"new")){
            // printf("PEHLI BAAR DECLARE,%s\n",node->tk_data.lexeme);
        // }
        if(table->arr[hash_value]!=NULL){
            // if(!strcmp(node->tk_data.lexeme,"new"))
                // printf("REDECLARED,%s\n",node->tk_data.lexeme);
            ST_ENTRY* temp = table->arr[hash_value];
            while(temp){
                if(!strcmp(temp->id_lexeme,node->tk_data.lexeme)){
                    if(table->parent_function!=NULL){
                        LISTNODE* temp_list = table->parent_function->ip_head;
                        while(temp_list){
                            if(!strcmp(temp_list->parameter_name,node->tk_data.lexeme)){
                                redeclare_flag = 1;
                                break;
                            }
                            temp_list = temp_list->next;
                        }
                    }
                    if(redeclare_flag==0){
                        // printf("\n********************reached********************\n");
                        programHasSemanticError=1;
                        printf("\n*****Error: Redeclaration of variable %s, line no : %d *****\n",node->tk_data.lexeme,node->line_no);
                        return NULL;
                    }
                }
                temp = temp->next;
            }
        }

        if(redeclare_flag==1){
            ST_ENTRY* element_to_be_deleted = table->arr[hash_value];
            ST_ENTRY* prev = element_to_be_deleted;
            while(element_to_be_deleted!=NULL){
                if(!strcasecmp(node->tk_data.lexeme,element_to_be_deleted->id_lexeme)){
                    if(prev==element_to_be_deleted){
                        ST_ENTRY* temp = malloc(sizeof(ST_ENTRY));
                        temp->id_lexeme = node->tk_data.lexeme;
                        temp->next = element_to_be_deleted->next;
                        temp->type = t1;
                        table->arr[hash_value] = temp;
                        node->symbol_table_entry = temp;
                        // printf("\nEntry done for %s, line no: %d \n",node->tk_data.lexeme,node->line_no);
                        return temp;
                    }
                    else{
                        prev->next = element_to_be_deleted->next;
                        element_to_be_deleted->next = NULL;
                    }
                }
                prev = element_to_be_deleted;
                element_to_be_deleted=element_to_be_deleted->next;
            }
        }
        

        ST_ENTRY* temp = malloc(sizeof(ST_ENTRY));
        temp->id_lexeme = node->tk_data.lexeme;
        temp->next = NULL;
        temp->type = t1;
        temp->last_assigned_nesting=-1;
        temp->isList=0;


        insert_in_table(table,temp);
        node->symbol_table_entry=temp;
        // printf("Node's symbol table entry:%s\n",node->symbol_table_entry->id_lexeme);
        // printf("\nEntry done for %s, line no: %d \n",node->tk_data.lexeme,node->line_no);
        return temp;
}

FN_ENTRY* create_entry_and_insert_in_FST(struct fn_symbol_table* table,struct treeNode* node,LISTNODE* ip_list,LISTNODE* op_list,int is_first_pass){
        if(node==NULL){
            printf("AST Node:%s is null inside create entry in FST \n",node->value);
            return NULL;
        }
        // FN_ENTRY* temp_entry = get_func_name(table,node->tk_data.lexeme);
        // if(temp_entry!=NULL){
        //     printf("Error: Redeclaration of function %s \n",node->tk_data.lexeme);
        //     return NULL;
        // }

        int hash_value= get_sym_table_hash(node->tk_data.lexeme);
        if(table->arr[hash_value]!=NULL){
            FN_ENTRY* temp = table->arr[hash_value];
            while(temp){
                if(!strcmp(temp->fn_name,node->tk_data.lexeme)){
                    if(temp->ip_head!=NULL){
                        if(!is_first_pass){
                            printf("\n****Error: Redeclaration of function / Overloading of function %s, line no  %d**** \n",node->tk_data.lexeme,node->line_no);
                            programHasSemanticError=1;
                        }
                        return NULL;
                    }
                    else{
                        temp->ip_head = ip_list;
                        temp->op_head = op_list;
                        node->function_table_entry = temp;
                        return temp;
                    }
                }
                temp = temp->next;
            }
        }

        FN_ENTRY* temp = malloc(sizeof(FN_ENTRY));
        temp->ip_head = ip_list;
        temp->op_head = op_list;
        temp->fn_name = node->tk_data.lexeme;
        temp->child_table = NULL;
        temp->next = NULL;
        
        insert_fn_in_table(table,temp);
        node->function_table_entry = temp;
        return temp;
}

struct id_symbol_table* initST(int nesting_num){
    //use this fn to initialise ST after offset,scopes etc have been added
    struct id_symbol_table* t = malloc(sizeof(struct id_symbol_table));
    t->child_table=NULL;
    t->parent_table=NULL;
    t->right_sibling=NULL;
    t->left_sibling=NULL;
    t->parent_function = NULL;
    t->nesting_value = nesting_num;
    t->scope_start=0;
    t->scope_end=0;
    for(int i=0;i<TABLE_SIZE;i++){
        t->arr[i]=NULL;
    }
    return t;
}

struct fn_symbol_table* initFST(){
    struct fn_symbol_table* t = malloc(sizeof(struct fn_symbol_table));
    for(int i=0;i<TABLE_SIZE;i++){
        t->arr[i]=NULL;
    }
    return t;
}

LISTNODE* makeListNode(struct treeNode* head,int isOpList){  
    if(head == NULL)
        return NULL;
    LISTNODE* temp = malloc(sizeof(LISTNODE));
    temp->parameter_name = head->tk_data.lexeme;    
    if(!strcmp(head->pair->value,"ARRAY-DCL")){
        //ALERT
        if(isOpList == 1){
            printf("Error:Array cannot be function Output Parameter\n");
            programHasSemanticError=1;
        }
        temp->parameter_type.arr_type.arr_dt = head->pair->children->astnextSibling->value;
        temp->is_array=1;
        struct treeNode* li = head->pair->children->children;
        struct treeNode* ri = head->pair->children->children->astnextSibling;
        // li //
        if(!strcmp(li->children->value,"ID")){
            temp->parameter_type.arr_type.lowRange.low_id = li->children->tk_data.lexeme;
            temp->parameter_type.arr_type.isStatic = 0; //dynamic array
        }
        else if(!strcmp(li->children->value,"NUM")){
            temp->parameter_type.arr_type.lowRange.start = li->children->tk_data.val;
            temp->parameter_type.arr_type.isStatic = 1; //static array
        }
        else{
            if(!strcmp(li->children->astnextSibling->value,"ID"))
            {
                   temp->parameter_type.arr_type.lowRange.low_id = strcat(strcat(li->children->value,","),li->children->astnextSibling->tk_data.lexeme);
                   temp->parameter_type.arr_type.isStatic = 0; //dynamic array
            }
            else
            {
                if(!strcmp(li->children->value,"MINUS"))
                    temp->parameter_type.arr_type.lowRange.start = -1*li->children->astnextSibling->tk_data.val;
                else   
                    temp->parameter_type.arr_type.lowRange.start = li->children->astnextSibling->tk_data.val;
                temp->parameter_type.arr_type.isStatic = 1; //static array
            }
        }
        
        // ri //
        if(!strcmp(ri->children->value,"ID")){
            temp->parameter_type.arr_type.highRange.high_id = ri->children->tk_data.lexeme;
            temp->parameter_type.arr_type.isStatic = 0; //dynamic array
        }
        else if(!strcmp(ri->children->value,"NUM")){
            temp->parameter_type.arr_type.highRange.end = ri->children->tk_data.val;
            temp->parameter_type.arr_type.isStatic = 1; //static array
        }
        else{
            if(!strcmp(ri->children->astnextSibling->value,"ID"))
            {
                   temp->parameter_type.arr_type.highRange.high_id = strcat(strcat(ri->children->value,","),ri->children->astnextSibling->tk_data.lexeme);
                   temp->parameter_type.arr_type.isStatic = 0; //dynamic array
            }
            else
            {
                if(!strcmp(ri->children->value,"MINUS"))
                    temp->parameter_type.arr_type.highRange.end = -1*ri->children->astnextSibling->tk_data.val;
                else   
                    temp->parameter_type.arr_type.highRange.end = ri->children->astnextSibling->tk_data.val;
                temp->parameter_type.arr_type.isStatic = 1; //static array
            }
        }
        
    }
    else{
        //normal ID
        temp->is_array=0;
        temp->parameter_type.id_type.id_dt = head->pair->value; 
    }
    temp->next = makeListNode(head->next,isOpList);
    return temp;
}


//  int main(){
//     struct fn_symbol_table* fst = initFST(1);
//     printf("\n init success");

//     struct treeNode* n1 = malloc(sizeof(struct treeNode));
//     n1->tk_data.val=2;
//     struct id_symbol_table* st = initST(1);
//     printf("\ninit success \n");

//     struct treeNode* n1 = malloc(sizeof(struct treeNode));

//     struct treeNode* n11 = malloc(sizeof(struct treeNode));
//     n1->addr = n11;
//     strcpy(n11->value,"abc");
//    // strcpy(n1->addr->value,"abc");
//     TYPE t1;
//     t1.id_type.id_dt=INTEGER;

//     create_entry_and_insert(st,n1,t1);


//     struct treeNode* n2 = malloc(sizeof(struct treeNode));
//     struct treeNode* n22 = malloc(sizeof(struct treeNode));
//     n2->addr = n22;
//     strcpy(n22->value,"def");
//    // strcpy(n2->addr->value,"def");
//     TYPE t2;
//     t2.id_type.id_dt=REAL;
//     create_entry_and_insert(st,n2,t2);

//     struct treeNode* n3 = malloc(sizeof(struct treeNode));
//     struct treeNode* n33 = malloc(sizeof(struct treeNode));
//     n3->addr = n33;
//     strcpy(n33->value,"aarya");
//     //strcpy(n3->addr->value,"aarya");
//     TYPE t3;
//     t3.id_type.id_dt=INTEGER;
    
//     create_entry_and_insert(st,n3,t3);


//     struct treeNode* n4 = malloc(sizeof(struct treeNode));
//     struct treeNode* n44 = malloc(sizeof(struct treeNode));
//     n4->addr = n44;
//     strcpy(n44->value,"great");
//     //strcpy(n4->addr->value,"great");
//     TYPE t4;
//     t4.id_type.id_dt=BOOLEAN;
//     create_entry_and_insert(st,n4,t4);
    


//     ST_ENTRY* temp = get_lexeme(st,"def");
//     // printf("IDHAR");
//     if(temp==NULL){
//         printf("FAIL \n");
//         return 0;
//     }
//     printf("** %s ** \n",temp->id_lexeme);

//      return 0;
//  }
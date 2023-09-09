/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/

// #include "lexer.c"
int programHasSemanticError = 0;
#include "parser.c"
#include "ast.c"
#include "semanticAnalysis.c"
#include "IRCodeGen.c"
#include "codegen.c"
#include<time.h>

int noOfASTNodes = 0;
int noOfParseTreeNodes = 0;

void removeComments(char *name)
{
    FILE *cfp = fopen(name, "r");
    if (cfp == NULL)
    {
        printf("File not found");
        return;
    }
    int flag = 0;
    while (!feof(cfp))
    {
        char c = fgetc(cfp);
        int flag2 = 0;
        if (c == '*' && flag == 0)
        {
            char d = fgetc(cfp);
            if (d == '*')
            {
                flag = 1;
            }
            fseek(cfp, -1, SEEK_CUR);
        }
        else if (c == '*' && flag == 1)
        {
            char d = fgetc(cfp);
            if (d == '*')
            {
                flag = 0;
                printf("  ");
                flag2 = 1;
            }
            else
                fseek(cfp, -1, SEEK_CUR);
        }
        if (!flag2)
        {
            if (flag == 0)
                printf("%c", c);
            else
                printf(" ");
        }
    }
    fclose(cfp);
}

void preProcessing(char* name){
    FILE* prefp1 = fopen(name, "r");
    if (prefp1 == NULL)
    {
        return;
    }
    fclose(prefp1);
    FILE* prefp = fopen(name, "a");
    
    // append new line character at the end of file
    fprintf(prefp,"\n");
    fclose(prefp);
}

void print_token()
{
    if (global_token.hasError == 1)
    {
        printf("\n %d \t \t %s \t \t LEXICAL ERROR \n", global_token.line_no, global_token.tk_data.lexeme);
    }
    else
    {
        if (global_token.tk_name == ID)
        {
            printf("%-20d%-20s%-20s \n", global_token.line_no, global_token.tk_data.lexeme, token_strings[global_token.tk_name]);
        }
        else if (global_token.tk_name == NUM)
        {
            printf("%-20d%-20d%-20s\n", global_token.line_no, global_token.tk_data.val, token_strings[global_token.tk_name]);
        }
        else if (global_token.tk_name == RNUM)
        {
            printf("%-20d%-20f%-20s \n", global_token.line_no, global_token.tk_data.realVal, token_strings[global_token.tk_name]);
        }
        else if (global_token.tk_name == COMMENTMARK)
        {
            printf("%-20d%-20s%-20s \n", global_token.line_no, "", token_strings[global_token.tk_name]);
        }
        else
        {
            printf("%-20d%-20s%-20s \n", global_token.line_no, global_token.tk_data.lexeme, token_strings[global_token.tk_name]);
        }
    }
}
void print_choices()
{
    printf("\n Enter one option out of below: \n ***********");
    printf("\n 0. Exit");
    printf("\n 1. Print token list on terminal:");
    printf("\n 2. Print Parse tree on terminal:");
    printf("\n 3. Print AST on terminal:");
    printf("\n 4. Print Size of AST/Parse Tree:");
    printf("\n 5. Print Symbol Table :");
    printf("\n 6. Activation record size :");
    printf("\n 7. Print Static Dynamic Arrays :");
    printf("\n 8. Error Analysis(Lexical, Syntax, Semantic) :");
    printf("\n 9. Codegen :");
    printf("\n******\n");
}

void totalTime()
{

    // lexer initialisations
    if (fp == NULL)
    {
        printf("Source File not found");
        return;
    }

    // parser initialisations

    // grammar generate
    // stack initialisations
    stackElement *element = malloc(sizeof(stackElement));
    element->isTerminal = 0;
    strcpy(element->value, "startprogram");
    s_push(element);

    // root element of tree creation
    struct treeNode *tree_node = malloc(sizeof(struct treeNode));
    tree_node->children = NULL;
    tree_node->isTerminal = 0;
    tree_node->parent = NULL;
    tree_node->nextSibling = NULL;
    tree_node->prevSibling = NULL;
    strcpy(tree_node->value, "startprogram");

    // setting root and currentExpand
    root = tree_node;
    currExpand = tree_node;

    getnextblock(fp, buff1);
    driverFlag2 = 0;
    driverFlag = 0;
    while (!driverFlag)
    {
        getNextToken();
        runPDA();
        // if(driverFlag2)driverFlag=1;
    }
    while (isEpsilon[get_hash(s_top->value)])
    {
        s_pop();
    }

    if (!strcmp(s_top->value, "DOLLAR") && !errorToken)
        printf("\n Parsing successful \n");
    else if (errorToken)
        printf("Syntax Error\n");
    else
    {
        printf("Input finish but stack not empty");
        while (s_top != NULL)
        {
            printf("%s\n", s_top->value);
            s_pop();
        }
    }

    free(element);
    free(tree_node);
    return;
}

void printParseTree(struct treeNode *node, FILE *outfile, int flag)
{
    // if flag is 0 just count, if flag is one print
    // EPSILON IS THERE IN THE PARSE TREE
    if (node == NULL || !strcmp(node->value, "EPSILON"))
    {
        if (!strcmp(node->value, "EPSILON")){
            if(flag==1)fprintf(outfile, "Lexeme: ----, Line no: %d, Tk name: EPSILON, ValueIfNumber: ----, Parent: %s, isLeaf: YES, NodeSymbol: ----\n", node->line_no, node->parent->value);
            noOfParseTreeNodes++;
        }
        return;
    }

    // if(!strcmp(node->value,"DOLLAR")){
    //     printf("DOLLAR\n");
    //     return;
    // }

    // printf("\n%s\n", node->value);

    if (node->children == NULL)
    { // fprintf(outfile,"getlost");
      // printf("*********%s,%d,%s,-,%s,YES,%s*********\n",node->tk_data.lexeme,node->line_no,node->value,node->parent->value,node->value);
        noOfParseTreeNodes++;
        if(flag==1){
        if (!strcmp(node->value, "NUM"))
            fprintf(outfile, "Lexeme: ----, Line no: %d, Tk name: %s, ValueIfNumber: %d, Parent: %s, isLeaf: YES, NodeSymbol: ----\n", node->line_no, node->value, node->tk_data.val, node->parent->value);
        else if (!strcmp(node->value, "RNUM"))
            fprintf(outfile, "Lexeme: ----, Line no: %d, Tk name: %s, ValueIfNumber: %f, Parent: %s, isLeaf: YES, NodeSymbol: ----\n", node->line_no, node->value, node->tk_data.realVal, node->parent->value);
        else if (node->parent == NULL)
            fprintf(outfile, "Lexeme: %s, Line no: %d, Tk_name: %s, ValueIfNumber: ----, Parent: ROOT, YES, NodeSymbol: ----\n", node->tk_data.lexeme, node->line_no, node->value);
        else
            fprintf(outfile, "Lexeme: %s, Line no: %d, Tk_name: %s, ValueIfNumber: ----, Parent: %s, YES, NodeSymbol: ----\n", node->tk_data.lexeme, node->line_no, node->value, node->parent->value);
        }
        // printf("%s\n", node->value);
        return;
    }
    // print left child
    // if (node->children == NULL)
    //     printf("Root's child is null\n");
    // else
    //     printf("Root's child is not null\n");
    printParseTree(node->children, outfile,flag);

    // print node
    if(flag==1){
    if (node->parent != NULL)
        fprintf(outfile, "RuleNo: %d, Lexeme: ----, Line no: ----, Tk name: ----, ValueIfNumber: ----, Parent: %s, IsLeaf: NO, NodeSymbol: %s\n", node->ruleNo, node->parent->value, node->value);
    else
        fprintf(outfile, "RuleNo: %d, Lexeme: ----, Line no: ----, Tk name: ----, ValueIfNumber: ----, Parent: ROOT, IsLeaf: NO, Node Symbol: %s\n", node->ruleNo, node->value);
    }
    
    noOfParseTreeNodes++;
    // print other children
    if (node->children == NULL)
        return;

    struct treeNode *temp;
    temp = node->children->nextSibling;

    while (temp != NULL)
    {
        printParseTree(temp, outfile,flag);
        temp = temp->nextSibling;
    }
    return;
}

// prints AST in inorder traversal
void printAST(struct treeNode *root, FILE *outfile,int flag)
{
    if (root == NULL)
        return;
    
    if (root->children != NULL)
        root->children->parent = root;
    printAST(root->children, outfile,flag);

   


    if (root->addr != NULL)
    {
        printAST(root->addr, outfile,flag);
    }
    else
    { 
        if(!strcmp(root->value,"SWITCH")){
            if(root->children->astnextSibling->astnextSibling!=NULL)
                root->children->astnextSibling->astnextSibling->isDefault = 1; //setting isdefault one for deault ka node
        }
        
        noOfASTNodes++;
        if(flag==1)fprintf(outfile, "%s\n", root->value);
    }

    if (root->children != NULL)
    {
        struct treeNode *temp;
        temp = root->children->astnextSibling;

        while (temp != NULL)
        {
            temp->parent = root;
            printAST(temp, outfile,flag);
            temp = temp->astnextSibling;
        }
    }

    struct treeNode * temp_par = root->parent;

    if(root->pair!=NULL){
        root->pair->parent = temp_par;
        printAST(root->pair, outfile,flag);
    }

    root = root->next;
    if(root != NULL)
    {
        root->parent = temp_par;
        printAST(root, outfile,flag);
        
    }
    
}

void runParser()
{

    // lexer initialisations

    if (fp == NULL)
    {
        printf("Source code file path incorrect \n");
        return;
    }

    // parser initialisations

    // grammar generate in main

    // create parse table in main

    // stack initialisations
    stackElement *element = malloc(sizeof(stackElement));
    element->isTerminal = 0;
    element->link = NULL;
    strcpy(element->value, "startprogram");
    s_push(element);

    // root element of tree creation
    struct treeNode *tree_node = malloc(sizeof(struct treeNode));
    initTreeNode(tree_node);
    tree_node->children = NULL;
    tree_node->isTerminal = 0;
    tree_node->parent = NULL;
    tree_node->nextSibling = NULL;
    tree_node->prevSibling = NULL;
    tree_node->visited = 0;
    tree_node->addr = NULL;
    tree_node->pair = NULL;
    tree_node->astnextSibling = NULL;
    tree_node->astprevSibling = NULL;
    tree_node->symbol_table_entry = NULL;
    tree_node->temp_variable_entry = NULL;
    tree_node->function_table_entry = NULL;
    tree_node->next = NULL;
    tree_node->inh=NULL;
    tree_node->syn=NULL;
    tree_node->type_syn=NULL;
    tree_node->type_inh=NULL;



    strcpy(tree_node->value, "startprogram");

    // setting root and currentExpand
    root = tree_node;
    currExpand = tree_node;

    getnextblock(fp, buff1);
    driverFlag2 = 0;
    driverFlag = 0;
    while (!driverFlag)
    {
        getNextToken();
        runPDA();
        // if(driverFlag2)driverFlag=1;
    }
    while (isEpsilon[get_hash(s_top->value)])
    {
        // printf("Current Expand is:%s\n", currExpand->value);
        currExpand->ruleNo = 7;
        addChild(currExpand,grammar[6]->forward_link);
        s_pop();
    }

    if (!strcmp(s_top->value, "DOLLAR") && !errorToken)
        printf("\n Parsing successful \n");
    else if (errorToken)
        printf("Syntax Error\n");
    else
    {
        printf("Input finish but stack not empty");
        while (s_top != NULL)
        {
            printf("%s\n", s_top->value);
            s_pop();
        }
    }

    // printf("%s\n",root->value);
    // createAST(root);
    // printf("\nLMAOO DED\n");
    // // printf("%s",root->value);
    // //printParseTree(root,stdout);
    // fprintf(fp2, "AST TREE:\n");
    // // printParseTree(root, fp2);
    // //  printf("VALUE OF ADDR BEFORE:%s\n",root->addr->value);
    // printAST(root, fp2);
    // printf("VALUE OF ADDR AFTER:%s\n",root->addr->value);
    free(element);
    free(tree_node);
    return;
}

int getActivationSize(struct id_symbol_table* table){
    if(table==NULL){
        return 0;
    }
    int size = 0;
    ST_ENTRY *temp;
    for(int i=0;i<TABLE_SIZE;i++){
        temp = table->arr[i];
        while(temp!=NULL){
            size+=temp->width;
            temp = temp->next;
        }
    }
    return size;
}

void printStaticDynamic(struct id_symbol_table*table,FN_ENTRY* parent_function){
    if(table==NULL){
        return;
    }
    ST_ENTRY *temp;
    for(int i=0;i<TABLE_SIZE;i++){
        temp = table->arr[i];
        while(temp!=NULL){
            if(temp->is_array==1){
                printf("\n\n");
                printf("%-15s",parent_function==NULL?"Driver":parent_function->fn_name); // Scope- Module name
                // temporarily reducing scope line numbers to 3 digits
                table->scope_start=table->scope_start%1000;
                table->scope_end=table->scope_end%1000;
                printf(" %5d-%-5d",table->scope_start,table->scope_end); // Scope- Line number
                printf(" %-15s",temp->id_lexeme); // Name
                if(temp->type.arr_type.isStatic==1){
                    printf(" %-10s","static"); // is static
                    printf(" %-1c[%7d-%-7d]%4c",' ',temp->type.arr_type.lowRange.start,temp->type.arr_type.highRange.end,' '); // range

                }
                else{
                    printf(" %-10s","dynamic"); // is static
                    printf(" %-1c[%7s-%-7s]%4c",' ',temp->type.arr_type.lowRange.low_id,temp->type.arr_type.highRange.high_id,' '); // range
                }
                printf(" %-10s",temp->type.arr_type.arr_dt); // type

            }  
            temp=temp->next;
        }
        
    }
}

void printForEntriesInTable(struct id_symbol_table*table,FN_ENTRY* parent_function){
    if(table==NULL){
        return;
    }
    ST_ENTRY *temp;


    
    // For printing ip/oiutput list first 
    for(int i=0;i<TABLE_SIZE;i++){
        temp = table->arr[i];
        while(temp!=NULL){
            if(temp->isList==1){
            printf("\n\n");
            printf("%-15s",temp->id_lexeme); 
            printf(" %-15s",parent_function==NULL?"Driver":parent_function->fn_name); // Scope- Module name
            // temporarily reducing scope line numbers to 3 digits
            table->scope_start=table->scope_start%1000;
            table->scope_end=table->scope_end%1000;
            printf(" %5d-%-5d",table->scope_start,table->scope_end); // Scope- Line number
            if(temp->is_array==1){
                printf(" %-10s",temp->type.arr_type.arr_dt); // type
                printf(" %-10s","yes"); // is_array
                if(temp->type.arr_type.isStatic==1){
                    printf(" %-10s","static"); // is_static
                    printf(" %-1c[%7d-%-7d]%4c",' ',temp->type.arr_type.lowRange.start,temp->type.arr_type.highRange.end,' '); // range
                }
                else{
                    printf(" %-10s","dynamic"); // is_static
                    printf(" %-1c[%10s-%-10s]%4c",' ',temp->type.arr_type.lowRange.low_id,temp->type.arr_type.highRange.high_id,' '); // range
                }

            }
            else if(temp->is_for==1){
                printf(" %-10s","INTEGER"); // type
                printf(" %-10s","no"); // is_array
                printf(" %-10s","**"); // is_static
                printf(" %-10c**%10c",' ',' '); // range
            }
            else{
                printf(" %-10s",temp->type.id_type.id_dt); // type
                printf(" %-10s","no"); // is_array
                printf(" %-10s","**"); // is_static
                printf(" %-10c**%10c",' ',' '); // range
            }
            printf(" %-5d",temp->width); // width
            printf(" %-5d",temp->offset); // offset
            printf(" %-5d",temp->isList==1?table->nesting_value-1:table->nesting_value);// nesting level
            printf("\n");
            }
            temp=temp->next;

        }
    }

    for(int i=0;i<TABLE_SIZE;i++){
        temp = table->arr[i];
        while(temp!=NULL){
            if(temp->isList!=1){
            printf("\n\n");
            printf("%-15s",temp->id_lexeme); 
            printf(" %-15s",parent_function==NULL?"Driver":parent_function->fn_name); // Scope- Module name
            // temporarily reducing scope line numbers to 3 digits
            table->scope_start=table->scope_start%1000;
            table->scope_end=table->scope_end%1000;
            printf(" %5d-%-5d",table->scope_start,table->scope_end); // Scope- Line number
            if(temp->is_array==1){
                printf(" %-10s",temp->type.arr_type.arr_dt); // type
                printf(" %-10s","yes"); // is_array
                if(temp->type.arr_type.isStatic==1){
                    printf(" %-10s","static"); // is_static
                    printf(" %-1c[%7d-%-7d]%4c",' ',temp->type.arr_type.lowRange.start,temp->type.arr_type.highRange.end,' '); // range
                }
                else{
                    printf(" %-10s","dynamic"); // is_static
                    printf(" %-1c[%10s-%-10s]%4c",' ',temp->type.arr_type.lowRange.low_id,temp->type.arr_type.highRange.high_id,' '); // range
                }

            }
            else if(temp->is_for==1){
                printf(" %-10s","INTEGER"); // type
                printf(" %-10s","no"); // is_array
                printf(" %-10s","**"); // is_static
                printf(" %-10c**%10c",' ',' '); // range
            }
            else{
                printf(" %-10s",temp->type.id_type.id_dt); // type
                printf(" %-10s","no"); // is_array
                printf(" %-10s","**"); // is_static
                printf(" %-10c**%10c",' ',' '); // range
            }
            printf(" %-5d",temp->width); // width
            printf(" %-5d",temp->offset); // offset
            printf(" %-5d",temp->isList==1?table->nesting_value-1:table->nesting_value);// nesting level
            printf("\n");
            }
            temp=temp->next;
        }
    }

}

void printSymbolTable(struct fn_symbol_table *table,int flag,struct id_symbol_table*initial_table){
    if(table==NULL){
        return;
    }
    FN_ENTRY *temp;
    struct id_symbol_table*queue[20];
    int front=0,rear=0;
    int flag_driver=0;
    for(int i=0;i<=TABLE_SIZE;i++){
        // When i is table size all entries checked print driver Flag
        if(i!=TABLE_SIZE)
            temp=table->arr[i];
        else{
            temp=NULL;
            flag_driver=1;
        }
        while(temp!=NULL || flag_driver){
        //Populate the queue initially;
            int size = 0;
            front=0,rear=0;
            struct id_symbol_table* temp2;
            if(!flag_driver)
                temp2=temp->child_table;
            else
                temp2=initial_table;
            
            while(temp2!=NULL){
                queue[rear++]=temp2;
                temp2=temp2->right_sibling;
            }
            //
            //Traverse through the queue
            while(front!=rear){
                if(front>20||rear>20)printf("\n size breached \n");
                // printf("\n 481 \n");
                struct id_symbol_table*temp3 = queue[front++];
                // printf("\n 483 \n");
                if(flag==0)printForEntriesInTable(temp3,temp);
                else if(flag==1) printStaticDynamic(temp3,temp);
                else if(flag==2) size += getActivationSize(temp3);
                // printf("\n WASSSUPPPPP\n");

                struct id_symbol_table* temp4 = temp3->child_table;
                // printf("\n AFTER WASSSUPPP \n");
                while(temp4!=NULL){
                    queue[rear++]=temp4;
                    temp4=temp4->right_sibling;
                }

            }
            if(flag==2)printf("\n%s:%d\n",temp!=NULL?temp->fn_name:"Driver",size);
            if(!flag_driver)temp = temp->next;
            else flag_driver=0;
        }
    }
    // printf("\nBYEEEEEEEEEE\n");
    return;
}

void initQuadTable(){
    for(int i=0;i<QUAD_SIZE;i++){
        quadTable[i].label = NULL;
        quadTable[i].goTolabel = NULL;
        quadTable[i].instruction = NULL;
        quadTable[i].arg_fn_name = NULL;
    }
}

int main(int argc, char *argv[])
{


    if (argc != 3)
    {
        printf("\n there must be only two arguments- SourceCode, .asm output file ");
        return 0;
    }

    preProcessing(argv[1]);

    printf("\n\nLEVEL 4: Symbol table/ AST/ Semantic Rules/Type checking modules work fully/handled static and dynamic arrays/Codegen done partially.\n\n");
    int option;

    buffer_size = 512;

    clock_t t1_start, t1_end;
    double t1, t1_in_seconds;
    t1_start = clock();

    initHashTable();
    FILE *fp1 = fopen("grammar.txt", "r");
    makeGrammar(fp1);
    fclose(fp1);

    memset(isEpsilon, 0, sizeof(isEpsilon));

    // create first
    for (int i = 0; i < TOTAL_TERM; i++)
    {
        char *non_terminal = nonTerminals[i];
        createfirst(non_terminal); // calculate first of lhs
        if (ntFirst[get_hash(non_terminal)][strlen(ntFirst[get_hash(non_terminal)]) - 1] == ',')
            ntFirst[get_hash(non_terminal)][strlen(ntFirst[get_hash(non_terminal)]) - 1] = '\0';
    }
    isEpsilon[get_hash("EPSILON")] = 1;

    for (int i = 0; i < NON_TERM; i++)
    {
        char *non_terminal = nonTerminals[i];
        // printf("%s\n", non_terminal);
        createFollow(non_terminal); // calculate first of lhs
        removeDuplicates(ntFollow[get_hash(non_terminal)]);
    }

    // compute first and follow
    computeFirstAndFollow();
    // for (int i = 0; i <NUM_RULES; i++)
    // {
    //     printf("RULE:%d  ", i+1);
    //     for (int j = 0; firstAndFollow[i][j] != NULL; j++)
    //         printf("%s ", firstAndFollow[i][j]);
    //     printf("\n");
    // }
    createParseTable();

    t1_end = clock();
    t1 = (double)(t1_end - t1_start);
    t1_in_seconds = t1 / CLOCKS_PER_SEC;

    print_choices();
    scanf("%d", &option);

    while (1)
    {
        buff1 = (char *)malloc(buffer_size * sizeof(char));
        buff2 = (char *)malloc(buffer_size * sizeof(char));
        state = 0;
        forward = 0;
        begin = 0;
        do_not_refill = 0;
        flag = 0;
        current_line_no = 1;
        generateToken = 0;
        token_found = 0;
        errorToken = 0;
        root=NULL;
        initTreeNode(root);
        currExpand=NULL;
        initTreeNode(currExpand);
        fp = fopen(argv[1], "r");
        noOfASTNodes = 0;
        noOfParseTreeNodes = 0;
        s_top = NULL;
        programHasLexicalError = 0;
        programHasParsingError=0;
        programHasSemanticError=0;

        switch (option)
        {
        case 0:
        {
            printf("\n Exiting succesfully \n");
            exit(0);
        }
        break;

        case 1:
        {
            // Token list
            FILE *source_code = fopen(argv[1], "r");
            if (source_code == NULL)
            {
                printf("\n Source code file not found");
                exit(0);
            }
            printf("Line number       Lexeme              Token Name: \n");
            getnextblock(source_code, buff1);
            int flag = 0;
            while (!flag)
            {
                // global_token.hasError = 0;
                generateToken = 1;
                while (generateToken)
                {
                    char input = getnextchar(source_code, buff1, buff2);
                    dfa(input);
                    if (input == EOF)
                    {
                        //   printf("input over\n");
                        flag = 1;
                        break;
                    }
                }

                if (!generateToken)
                    print_token();
            }
            fclose(source_code);
        }
        break;

        case 2:
        {
            // Parse Tree
            runParser();
            printParseTree(root, stdout,1);
            printf("\nParse Tree printed in console \n");
        }
        break;

        // case 4:
        // {
        //     clock_t start_time, end_time;
        //     double total_CPU_time, total_CPU_time_in_seconds;
        //     start_time = clock();

        //     // invoke your lexer and parser here
        //     fp = fopen(argv[1], "r");
        //     totalTime();
        //     fclose(fp);

        //     end_time = clock();
        //     total_CPU_time = (double)(end_time - start_time);
        //     total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
        //     // Print both total_CPU_time and total_CPU_time_in_seconds
        //     printf("Total CPU Time is: %f, totoal cpu time in seconds is %f \n", total_CPU_time + t1, total_CPU_time_in_seconds + t1_in_seconds);
        // }
        // break;
        case 3:
        {
            // AST
            runParser();
            createAST(root);
            if (root == NULL){
                printf("error in creating AST\n");
                exit(0);
            }
            printf("\n%s\n", root->value);
            printAST(root, stdout,1);
            printf("\n AST printed in console \n");
        }
        break;
        case 4:
        {
            runParser();
            printParseTree(root, stdout,0);
            createAST(root);
            printAST(root, stdout,0);
            printf("\n Parse tree no of nodes is %d, size is %ld \n", noOfParseTreeNodes, noOfParseTreeNodes * sizeof(struct treeNode));
            printf("\n AST no of nodes is %d, size is %ld \n", noOfASTNodes, noOfASTNodes * sizeof(struct treeNode));
            printf("\n Compression percentage is %d \n", (noOfParseTreeNodes - noOfASTNodes) * 100 / noOfParseTreeNodes);
        }
        break;
        case 5:
        {
            runParser();
            createAST(root);
            fn_table = initFST(1);
            fn_table_pass1 = initFST(1);
            if(root==NULL) printf("ROOT IS NULL\n");
            else{
                // printf("root's addr is:%s\n",root->addr->value);
                fillDef(root->addr); //pass PROGRAM node
                struct id_symbol_table* initial_table = initST(1);
                semanticAnalysis(root,initial_table,1,0);
                printSymbolTable(fn_table,0,initial_table);
                
            }
            free(fn_table);
            free(fn_table_pass1);
        }
        break;
        case 6:
        {
            runParser();
            createAST(root);
            fn_table = initFST(1);
            fn_table_pass1 = initFST(1);
            if(root==NULL) printf("ROOT IS NULL\n");
            else{
                // printf("root's addr is:%s\n",root->addr->value);
                fillDef(root->addr); //pass PROGRAM node
                struct id_symbol_table* initial_table = initST(1);
                semanticAnalysis(root,initial_table,1,0);
                printSymbolTable(fn_table,2,initial_table);
            }
            free(fn_table);
            free(fn_table_pass1);
        }
        break;
        case 7:
        {
            runParser();
            createAST(root);
            fn_table = initFST(1);
            fn_table_pass1 = initFST(1);
            if(root==NULL) printf("ROOT IS NULL\n");
            else{
                // printf("root's addr is:%s\n",root->addr->value);
                fillDef(root->addr); //pass PROGRAM node
                struct id_symbol_table* initial_table = initST(1);
                semanticAnalysis(root,initial_table,1,0);
                printSymbolTable(fn_table,1,initial_table);
            }
            free(fn_table);
            free(fn_table_pass1);
        }
        break;
        case 8:
        {
            clock_t start_time, end_time;
            double total_CPU_time, total_CPU_time_in_seconds;
            start_time = clock();
            runParser();
            /// ********* ADDDD CHECK HERE TO SEE IF THERE IS NO PARSING?LEXICAL ERROR THEN ONLY CALL SEMANTIC ANALYSIS *************************
            if(!programHasLexicalError && !programHasParsingError){
                createAST(root);
                fn_table = initFST(1);
                fn_table_pass1 = initFST(1);
                if(root==NULL) printf("ROOT IS NULL\n");
                else{
                    // printf("root's addr is:%s\n",root->addr->value);
                    fillDef(root->addr); //pass PROGRAM node
                    struct id_symbol_table* initial_table = initST(1);
                    semanticAnalysis(root,initial_table,1,0);
                }
                if(!programHasSemanticError)printf("\n Code Compiled Succesfully \n");
                else printf("\n Program has Semantic Errors \n");
            }
            end_time = clock();
            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            // Print both total_CPU_time and total_CPU_time_in_seconds
            printf("\n\nTotal CPU Time is: %f, totoal cpu time in seconds is %f \n", total_CPU_time + t1, total_CPU_time_in_seconds + t1_in_seconds);
            free(fn_table);
            free(fn_table_pass1);
        }
        break;
        case 9:
        {
            runParser();
            createAST(root);
            printAST(root, stdout,0);
            fn_table = initFST(1);
            fn_table_pass1 = initFST(1);
            if(root==NULL) printf("ROOT IS NULL\n");
            else{
                printf("root's addr is:%s\n",root->addr->value);
                fillDef(root->addr); //pass PROGRAM node
                struct id_symbol_table* initial_table = initST(1);
                global_max_offset = 0;
                semanticAnalysis(root,initial_table,1,0);
                if(programHasSemanticError){
                    printf("\n ******Program has Semantic Errors Pls Rectify***** \n");
                    printf("\n****Codegen not done due to errors****\n");
                    break;
                }
                printf("\nGlobal max offset is %d\n",global_max_offset);
                root->addr->parent = NULL;
                count = 0;
                variable_count =0;
                temporaries_st=initST(0);
                initQuadTable();
                IRcodegenerate(root);
            }
            codegen(argv[2]);
            // system("nasm -felf64 code.asm && gcc -no-pie code.o && ./a.out");
            free(fn_table);
            free(fn_table_pass1);
        }
        break;
        default:
            printf("\n choose one of the given options\n");
            break;
        }
        free(buff1);
        free(buff2);
        print_choices();
        scanf("%d", &option);    
        fclose(fp);
    }

}
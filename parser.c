/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#include "parserDef.h"
#include "lexer.c"
#include "tree.c"
#include "stackAdt.c"

//hash function for terminals and non-terminals
void runPDA();
int get_hash(const char *s)
{
    int n = strlen(s);
    long long p = 31, m = 10e9+7;
    long long hash = 0;
    long long p_pow = 1;
    for (int i = 0; i < n; i++)
    {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return abs(hash)%HASH_MOD;
}

void getNextToken()
{
    if(!driverFlag){
        generateToken = 1; 
        while (generateToken)
        {
            char input = getnextchar(fp, buff1, buff2);
            dfa(input);
            if (input == EOF)
            {
                driverFlag = 1;
                break;
            }
        } 
    }
    if(global_token.hasError==1){
        printf("Lexical Error at line no. %d\n", global_token.line_no);
    }
}

int checkFollow(){
    char temp[300];
    strcpy(temp,ntFollow[get_hash(s_top->value)]);
    char* token = strtok(temp,comma);
    while(token!=NULL){
        if(!strcmp(strdup(token),token_strings[global_token.tk_name]))
            return 1;
        token = strtok(NULL,comma);
    }
    return 0;
}

//function for displaying error while parsing
void display_error(int type)
{
   programHasParsingError=1;
   printf("Parsing error at line no. %d\n", global_token.line_no);
   errorToken = 1;
   generateToken = 0;
   if(type == 0)
   {
            s_pop();
            runPDA();
   }
   else if(type == 1)
   {
        while(!checkFollow() && !driverFlag){
            getNextToken();
        }
        //printf("\n after while\n");
        s_pop();
        runPDA();
   }
   else{
        if(!driverFlag)
            printf("Input not finished but stack empty\n");
        else
            printf("parsing unsuccesful\n");
        exit(0);
        //return;
   }
//    printf("Parsing error at line no. %d\n", global_token.line_no);
   //to test errors
   //exit(0);
}

// function for storing grammar rules in the form of linked list
void makeGrammar(FILE* fp)
{
    char buff[1024];
    int counter = 0;
    char delim[] = {", \n"};
    
    while(fgets(buff, 1024, fp))
    {
        char * token = strtok(buff, delim);
        struct node* nonTerminal = (struct node*) malloc(sizeof(struct node));
        nonTerminal->isTerminal = 0;
        nonTerminal->backward_link = NULL;
        strcpy(nonTerminal->value,strdup(token));
        grammar[counter] = nonTerminal;
        struct node* curr_token = nonTerminal;

        while((token = strtok(NULL, delim)) != NULL)
        {
            struct node* temp = (struct node*) malloc(sizeof(struct node));
            if(islower(token[0])) 
                temp->isTerminal = 0;  
            else
                temp->isTerminal = 1;

            strcpy(temp->value,strdup(token));
            curr_token->forward_link = temp;
            temp->backward_link = curr_token;
            curr_token = temp;
        }
        curr_token->forward_link = NULL;
        counter++;
    }
}

void createParseTable()
{
    for(int i = 0; i < NUM_RULES; i++)
    {
        for(int j = 0; firstAndFollow[i][j] != NULL; j++)
        {
            parseTable[get_hash(grammar[i]->value)][get_hash(firstAndFollow[i][j])].gNode = grammar[i];
            parseTable[get_hash(grammar[i]->value)][get_hash(firstAndFollow[i][j])].ruleNo = i + 1;
        }
    }
}


void runPDA(){
    if(s_top == NULL){
        display_error(2);
    }
    else if (driverFlag || global_token.tk_name == COMMENTMARK || global_token.hasError==1)
        return;
    else
    {
        if(!strcmp(currExpand->value,"EPSILON")){
            //set NULL
            currExpand->children = NULL;
            while(currExpand->parent->nextSibling == NULL && currExpand->parent != root)
            {
                currExpand = currExpand->parent;
                while(currExpand->prevSibling != NULL)
                    currExpand = currExpand->prevSibling;
            }

            if(currExpand->parent != root)
            {
                currExpand = currExpand->parent->nextSibling;
            }
        }
        if(s_top->isTerminal)
        {
            if(!strcmp(s_top->value,token_strings[global_token.tk_name]))
            {
                //equating unions(setting leafNode's lexeme)
                currExpand->tk_data = global_token.tk_data;

                //set line number for leaf node
                currExpand->line_no = global_token.line_no;

                //match
                // printf("match,%s\n",s_top->value);
                s_pop();
                //tree-code

                //set NULL
                currExpand->children = NULL;

                if(currExpand->nextSibling != NULL){
                    currExpand = currExpand->nextSibling;
                }
                else
                {
                    while(currExpand->prevSibling != NULL){
                        currExpand = currExpand->prevSibling;
                    }

                    while(currExpand->parent->nextSibling == NULL && currExpand->parent != root)
                    {
                        currExpand = currExpand->parent;
                        while(currExpand->prevSibling != NULL)
                            currExpand = currExpand->prevSibling;
                    }

                    if(currExpand->parent != root)
                    {
                        currExpand = currExpand->parent->nextSibling;
                    }
                }
            }
            else{
                //printf("not match");
                display_error(0);
            }
        }
        else
        {   
            if(parseTable[get_hash(s_top->value)][get_hash(token_strings[global_token.tk_name])].gNode != NULL)
            {
                struct node* curr;
                curr = parseTable[get_hash(s_top->value)][get_hash(token_strings[global_token.tk_name])].gNode;
                 //setting the rule number of a non terminal
                currExpand->ruleNo = parseTable[get_hash(s_top->value)][get_hash(token_strings[global_token.tk_name])].ruleNo;
                s_pop();
                //tree generation
                if(!strcmp(currExpand->value,"EPSILON")){
                     while(currExpand->parent->nextSibling == NULL && currExpand->parent != root)
                    {
                        currExpand = currExpand->parent;
                        while(currExpand->prevSibling != NULL)
                            currExpand = currExpand->prevSibling;
                    }

                    if(currExpand->parent != root)
                    {
                        currExpand = currExpand->parent->nextSibling;
                    }
                }
                addChild(currExpand,curr->forward_link);
                currExpand = currExpand->children;
                while(curr->forward_link != NULL)
                {
                    curr = curr->forward_link;
                }
                while(curr->backward_link != NULL)
                {
                    
                    stackElement* stackNode = malloc(sizeof(stackElement));
                    stackNode->isTerminal = curr->isTerminal;
                    strcpy(stackNode->value,curr->value);
                    if(strcmp(stackNode->value,"EPSILON")){
                        s_push(stackNode);
                    }
                    
                    curr = curr->backward_link;
                }
                runPDA();
            }
            else
            {
                display_error(1);
            }
        }
    }
}

int cnt = 0;
int createfirst(char *term)
{ // returns 0 if first contains epsilon,otherwise returns 1
  

    if (strlen(ntFirst[get_hash(term)]))
    {
        
        return isEpsilon[get_hash(term)];
    }
    
    if (isupper(term[0]))
    { // if term is a terminal
        if (strcmp(term, "EPSILON"))
        {                                              // and its not an epsilon
            if (!strlen(ntFirst[get_hash(term)]))      // if ntFirst of that term is empty
                strcat(ntFirst[get_hash(term)], term); // append term
            return 0;
        }
        else
        {
            isEpsilon[get_hash(term)] = 1; // if term is epsilon set isEpsilon 1 and return 1
            return 1;
        }
    }

    // if term is a non terminal,find first
    for (int i = 0; i < NUM_RULES; i++)
    {
        if (!strcmp(grammar[i]->value, term))
        {                                         // matching nt found
            struct node *temp = grammar[i]->forward_link; // LHS
            while (temp != NULL && createfirst(temp->value))
            {                                                                                                               // if temp is a terminal (except epsilon) or a nt (which doesnt derive epsilon)
                if (strlen(ntFirst[get_hash(term)]) && ntFirst[get_hash(term)][strlen(ntFirst[get_hash(term)]) - 1] != ',') // if ntFirst of that term isnt empty
                    strcat(ntFirst[get_hash(term)], ",");
                strcat(ntFirst[get_hash(term)], ntFirst[get_hash(temp->value)]);
                temp = temp->forward_link;
            }
            if (temp == NULL)
                isEpsilon[get_hash(term)] = 1;
            else
            {
                if (strlen(ntFirst[get_hash(term)]) && ntFirst[get_hash(term)][strlen(ntFirst[get_hash(term)]) - 1] != ',') // if ntFirst of that term isnt empty
                    strcat(ntFirst[get_hash(term)], ",");
                
                strcat(ntFirst[get_hash(term)], ntFirst[get_hash(temp->value)]);
            }
        }
    }
    return isEpsilon[get_hash(term)];
}

void bubbleSort(char *arr[500], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (strcmp(arr[j], arr[j + 1]) > 0)
            {
                char *temp;
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
}

void removeDuplicates(char *str)
{
    char *arr[500];
    char *token = strtok(str, ",");
    int i = 0;
    while (token != NULL)
    {
        arr[i] = token;
        i++;
        token = strtok(NULL, ",");
    }
    bubbleSort(arr, i);
    int n = i;
    char str1[2000] = {};
    for (i = 0; i < n; i++)
    {
        if (i != n - 1 && strcmp(arr[i], arr[i + 1]))
        {
            if (strlen(str1))
            {
                strcat(str1, ",");
            }
            strcat(str1, arr[i]);
        }
        if (i == n - 1)
        {
            if (strlen(str1))
            {
                strcat(str1, ",");
            }
            strcat(str1, arr[i]);
        }
    }
    
    strcpy(str, str1);
}

void createFollow(char *non_terminal)
{
    if (strlen(ntFollow[get_hash(non_terminal)]))
        return;
    for (int i = 0; i < NUM_RULES; i++)
    {
        struct node *temp = grammar[i]->forward_link;
        while (temp != NULL && strcmp(temp->value, non_terminal))
        {
            temp = temp->forward_link;
        }

        if (temp != NULL)
        {
            temp = temp->forward_link;
            
            while (temp != NULL && isEpsilon[get_hash(temp->value)])
            {
                if (strlen(ntFollow[get_hash(non_terminal)]) && ntFollow[get_hash(non_terminal)][strlen(ntFollow[get_hash(non_terminal)]) - 1] != ',')
                    strcat(ntFollow[get_hash(non_terminal)], ",");
                strcat(ntFollow[get_hash(non_terminal)], ntFirst[get_hash(temp->value)]);
                temp = temp->forward_link;
            }

            if (temp == NULL)
            {
                if (strcmp(grammar[i]->value, non_terminal))
                {
                    if (strlen(ntFollow[get_hash(non_terminal)]) && ntFollow[get_hash(non_terminal)][strlen(ntFollow[get_hash(non_terminal)]) - 1] != ',')
                        strcat(ntFollow[get_hash(non_terminal)], ",");
                    if (!strlen(ntFollow[get_hash(grammar[i]->value)]))
                        createFollow(grammar[i]->value);
                    strcat(ntFollow[get_hash(non_terminal)], ntFollow[get_hash(grammar[i]->value)]);
                }
            }
            else
            {
                
                if (strlen(ntFollow[get_hash(non_terminal)]) && ntFollow[get_hash(non_terminal)][strlen(ntFollow[get_hash(non_terminal)]) - 1] != ',')
                    strcat(ntFollow[get_hash(non_terminal)], ",");
                
                strcat(ntFollow[get_hash(non_terminal)], ntFirst[get_hash(temp->value)]);
            }
        }
    }
}

void computeFirstAndFollow()
{
    for (int i = 0; i < NUM_RULES; i++)
    {
        int j = 0;
        char *first;
        char *follow;
        char *token;
        char temp2[300];
        struct node *temp = grammar[i]->forward_link;
        while (temp != NULL && isEpsilon[get_hash(temp->value)])
        {
            if(strcmp(temp->value,"EPSILON")){
                first = ntFirst[get_hash(temp->value)];
                strcpy(temp2,ntFirst[get_hash(temp->value)]);
                token = strtok(temp2, comma); 
                firstAndFollow[i][j++]=strdup(token);
                while ((token = strtok(NULL, comma)) != NULL) 
                {
                    firstAndFollow[i][j++]=strdup(token);
                }
            }
            temp = temp->forward_link;
        }
        if (temp == NULL)
        {
            follow = ntFollow[get_hash(grammar[i]->value)];
            strcpy(temp2,ntFollow[get_hash(grammar[i]->value)]);
            token = strtok(temp2, comma);
            firstAndFollow[i][j++]=strdup(token);
            while ((token = strtok(NULL, comma)) != NULL)
            {
                firstAndFollow[i][j++]=strdup(token);
            }
        }
        else
        {
            first = ntFirst[get_hash(temp->value)];
            strcpy(temp2,ntFirst[get_hash(temp->value)]);
            token = strtok(temp2, comma);
            firstAndFollow[i][j++] = strdup(token);
            while ((token = strtok(NULL, comma)) != NULL)
            {
                firstAndFollow[i][j++] = strdup(token);
            }
        }
    }
}

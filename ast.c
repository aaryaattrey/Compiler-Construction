/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#include "treeDef.h"

int is_important_terminal(char *t)
{
    if (!(strcmp(t, "NUM") && strcmp(t, "RNUM") && strcmp(t, "ID") && strcmp(t, "INTEGER") && strcmp(t, "REAL") && strcmp(t, "BOOLEAN") && strcmp(t, "TRUE") && strcmp(t, "FALSE") && strcmp(t, "AND") && strcmp(t, "OR") && strcmp(t, "LT") && strcmp(t, "LE") && strcmp(t, "GT") && strcmp(t, "GE") && strcmp(t, "EQ") && strcmp(t, "NE") && strcmp(t, "PLUS") && strcmp(t, "MINUS") && strcmp(t, "DIV") && strcmp(t, "MUL") && strcmp(t, "EPSILON")))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

struct treeNode *insert_at_beginning(struct treeNode *head, struct treeNode *node)
{
    //printf("\nInside list\n");
    if (head == NULL)
    {
        head = node;
        node->next = NULL;
        return head;
    }

    node->next = head;
    head = node;
    return head;
}

struct treeNode *insert_pair_at_beginning(struct treeNode *head, struct treeNode *node1, struct treeNode *node2)
{
    if (head == NULL)
    {
        node1->pair = node2;
        node1->next = NULL;
        head = node1;
        return head;
    }

    node1->pair = node2;
    node1->next = head;
    head = node1;
    return head;
}

struct treeNode *makeNewNode(char *name, struct treeNode *list[], int count)
{
    // printf("Count:%d\n", count);
    // if (list[0] == NULL)
    //     printf("LIST[0] is null\n");
    // if (list[1] == NULL)
    //     printf("LIST[1] is null\n");
    // printf("after null check\n");
    //printf("ENTRY HOGAYI\n");
    struct treeNode *node = malloc(sizeof(struct treeNode));
    // printf("NODE %s \n",name);
    strcpy(node->value, name);
    node->addr = NULL;
    node->visited = 0;
    node->pair = NULL;
    node->next = NULL;
    node->astnextSibling = NULL;
    node->astprevSibling = NULL;
    node->function_table_entry = NULL;
    node->temp_variable_entry = NULL;
    node->symbol_table_entry = NULL;
    node->type_syn = NULL;
    node->type_inh = NULL;
    // printf("\nafter strcpy\n");
    int i;
    node->children = list[0];
    node->tk_data.val = 0;
    struct treeNode *temp, *temp2;
    temp = node->children;
    i = 1;
    while (temp == NULL && i < count)
    {
        temp = list[i];
        i++;
    }
    //printf("2800\n");

    if (i <= count)
        node->children = temp;

    if (temp != NULL)
        temp->astprevSibling = NULL;

    //printf("2200,%d\n",i);

    for (int k = i; k < count; k++)
    {
        if (list[k] == NULL)
            continue;
        // printf("Entries:%s\n", list[k]->value);
        temp->astnextSibling = list[k];
        temp2 = temp;
        temp = temp->astnextSibling;
        temp->astprevSibling = temp2;
    }
    // printf("HELLO:%s\n",list[2]->value);
    if(temp!=NULL)
        temp->astnextSibling = NULL;
    // printf("ISME KYA DIKKAT?\n");

    // printf("Child 1:%s\n",node->children->tk_data.lexeme);
    // printf("\nChild 2:%s\n",node->children->nextSibling->tk_data.lexeme);

    return node;
}

void deleteNode(struct treeNode *node)
{
    if (node == NULL)
    {
        return;
    }

    // printf("DELETED NODE:%s\n", node->value);

    if (node->prevSibling == NULL)
    {
        node->parent->children = node->nextSibling;
        if (node->nextSibling != NULL)
        {
            node->nextSibling->prevSibling = NULL;
        }
    }

    // if(!strcmp(node->value,"whichStmt"))
    // {
    //     printf("CHILD1:%s\n",node->parent->addr->children->value);
    //     printf("CHILD2:%s\n",node->parent->addr->children->nextSibling->value);
    // }

    else if (node->nextSibling == NULL)
    {
        node->prevSibling->nextSibling = NULL;
    }

    else
    {
        node->prevSibling->nextSibling = node->nextSibling;
        node->nextSibling->prevSibling = node->prevSibling;
    }

    free(node);

    // node = NULL;
}

void createAST(struct treeNode *root)
{
    // base-case
    if (root == NULL)
        return;

    if (!strcmp(root->value, "EPSILON"))
    {
        deleteNode(root);
        return;
    }

    char *node_name;        // name of a node
    struct treeNode *temp;  // for storing list
    struct treeNode *temp1; // for stroing pair in list
    struct treeNode *children[4];
    int count = 0;
    struct treeNode *freenode;

    // post-order traversal

    // capture leftmost-child
    struct treeNode *temp_child = root->children;

    // capture rule number from treeNode
    int rule_no = root->ruleNo;
    if (!strcmp(root->value, "otherModules"))
        //printf("Rule no : %d %d %s \n", rule_no, root->isTerminal, root->children->value);

    // if(!strcmp(root->value,"EPSILON"))
    //     printf("EPSILON's isTerminal:%d",root->isTerminal);

    // base-case, is Epsilon's isTerminal == 1?
    // problem
    if (root->isTerminal)
    {
        return;
    }
    // printf("RULE NO. IS:%d for %s\n",rule_no,root->value);
    // if(root->children == NULL) printf("\nNULL\n");
    // printf("CHILDREN IS:%f\n",root->children);
    // if(!strcmp(root->value,"otherModules"))
    // printf("\nOthermodules called on %d\n",rule_no);
    // iterate through all children(top-down rules to be added here)
    while (temp_child != NULL)
    {
        // printf("\n%s\n",temp_child->line_no);
        freenode = temp_child;
        if (temp_child->isTerminal && !is_important_terminal(temp_child->value))
        {
            // printf("ISKE ANDAR GHUS GAYA KYA?\n");
            //  printf("****%s*****: NODE FREED",temp_child->value);
            temp_child = temp_child->nextSibling;
            // if(freenode != NULL)
            // printf("delete node called:%s\n",freenode->value);
            deleteNode(freenode);
            // root->children = NULL;
            // printf("VALUE////%s",root->children->value);
            // if(root->children == NULL) printf("Root's children is NULL\n");
            // if(temp_child == NULL) printf("Temp_child is NULL\n");
            // printf("isTerminal:%s\n",root->value);
            // printf("RULE NO:%d\n",rule_no);
            // printf("RHS:%s",grammar[rule_no-1]->forward_link->value);
            continue;
        }
        // printf("\nDID OTHERMODULES COME HERE?\n");

        switch (rule_no)
        {
        case 1:
            // top-down
            // printf("CreateAST Called on:%s\n",temp_child->value);
            //printf("Hello\n");
            createAST(temp_child);

            // printf("\nAfter createAST\n");
            if (!strcmp(temp_child->value, "program"))
            {
                root->addr = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                // printf("delete called on program\n");
                //  printf("value of startprogram's children:%s\n",root->children->value);
                continue;
            }

            break;

        case 2:
            //printf("create ast called on: %s\n", temp_child->value);
            createAST(temp_child);

            if (!strcmp(temp_child->value, "moduleDeclarations"))
            {
                children[count++] = temp_child->syn;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "otherModules"))
            {
                //printf("Othermodules\n");
                children[count++] = temp_child->syn;
                // printf("\nBefore making PROGRAM node\n");
                if (temp_child->nextSibling == NULL)
                {
                    //printf("ABHI TO PARTY SHURU HUI HAI\n");
                    root->addr = makeNewNode("PROGRAM", children, 4);
                    // printf("AFTER MAKE NEW NODE:%s\n",root->addr->children->value);
                }
                // printf("\nAfter making PROGRAM node\n");
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "driverModule"))
            {
                //printf("Entered here\n");
                children[count++] = temp_child->addr;
                if(temp_child->addr == NULL)
                    printf("WTF IS THIS NULL\n");
                // root->addr = makeNewNode("PROGRAM", children, 4);
                temp_child = temp_child->nextSibling;
                //printf("Last sibling ka value: %s\n", temp_child->value);
                deleteNode(freenode);
                //printf("Last sibling ka value: %s\n", temp_child->value);
                continue;
            }

            break;

        case 3:
            createAST(temp_child);

            if (!strcmp(temp_child->value, "moduleDeclaration"))
            {
                temp = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "moduleDeclarations"))
            {
                root->syn = insert_at_beginning(temp_child->syn, temp);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            break;

        case 4:
            root->syn = NULL;
            // printf("\nCreateAST Called on:%s's child\n",root->value);
            createAST(temp_child);
            return;

        case 5:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                root->addr = temp_child;
            }
            break;

        case 6:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "module"))
            {
                temp = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "otherModules"))
            {
                root->syn = insert_at_beginning(temp_child->syn, temp);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 7:
            // printf("\nCreated AST called on %s's child\n",root->value);
            //printf("\n:()\n");
            root->syn = NULL;
            createAST(temp_child);
            //printf("SAD LYF\n");
            return;

        case 8:
            // printf("CreateAST Called on:%s\n",temp_child->value);
            createAST(temp_child);
            if (!strcmp(temp_child->value, "moduleDef"))
            {

                root->addr = temp_child->syn;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 9:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                children[count++] = temp_child;
            }
            if (!strcmp(temp_child->value, "input_plist"))
            {
                children[count++] = temp_child->syn;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "ret"))
            {
                children[count++] = temp_child->syn;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "moduleDef"))
            {
                children[count++] = temp_child->syn;
                root->addr = makeNewNode("MODULE_DEF", children, 4);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            break;
        case 10:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "output_plist"))
            {
                root->syn = temp_child->syn;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 11:
            root->syn = NULL;
            createAST(temp_child);
            return;

        case 12:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                temp = temp_child;
            }
            if (!strcmp(temp_child->value, "dataType"))
            {
                temp1 = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "moreList"))
            {
                root->syn = insert_pair_at_beginning(temp_child->syn, temp, temp1);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 13:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                temp = temp_child;
            }
            if (!strcmp(temp_child->value, "dataType"))
            {
                temp1 = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "moreList"))
            {
                root->syn = insert_pair_at_beginning(temp_child->syn, temp, temp1);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 14:
            root->syn = NULL;
            createAST(temp_child);
            return;

        case 15:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                temp = temp_child;
            }
            if (!strcmp(temp_child->value, "type"))
            {
                temp1 = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "moreOutput"))
            {
                root->syn = insert_pair_at_beginning(temp_child->syn, temp, temp1);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 16:
            root->syn = NULL;
            createAST(temp_child);
            return;

        case 17:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                temp = temp_child;
            }
            if (!strcmp(temp_child->value, "type"))
            {
                temp1 = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "moreOutput"))
            {
                root->syn = insert_pair_at_beginning(temp_child->syn, temp, temp1);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 18:
            root->syn = NULL;
            createAST(temp_child);
            return;

        case 19:
            root->addr = temp_child;
            break;

        case 20:
            root->addr = temp_child;
            break;

        case 21:
            root->addr = temp_child;
            break;

        case 22:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "range"))
            {
                children[count++] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "type"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("ARRAY-DCL", children, 2);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 23:
            root->addr = temp_child;
            break;

        case 24:
            root->addr = temp_child;
            break;

        case 25:
            root->addr = temp_child;
            break;

        case 26:
            //("Creating AST called on:%s\n",temp_child->value);
            createAST(temp_child);
            //printf("WHAT ABOUT THIS?\n");
            if (!strcmp(temp_child->value, "statements"))
            {
                //("\nInside if for statements\n");
                root->syn = temp_child->syn;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 27:
            createAST(temp_child);
            //printf("IDHAR AAYA KYA?\n");
            if (!strcmp(temp_child->value, "statement"))
            {
                temp = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "statements"))
            {
                root->syn = insert_at_beginning(temp_child->syn, temp);
                // printf("SYN KI VALUE HAI:%s\n",root->syn->children->astnextSibling->value);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 28:
           
            createAST(temp_child);
            root->syn = NULL;
            return;

        case 29:
            createAST(temp_child);
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 30:
            createAST(temp_child);
            root->addr = temp_child->addr;
            // printf("Statement's addr contains:%s",root->addr->value);
            deleteNode(freenode);
            return;

        case 31:
            createAST(temp_child);
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 32:
            createAST(temp_child);
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 33:
            // only one child possible
            createAST(temp_child);
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 34:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                children[0] = temp_child;
                root->addr = makeNewNode("GET-VALUE", children, 1);
            }
            break;

        case 35:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "print_var"))
            {
                children[0] = temp_child->addr;
                //printf("HEY THERE\n");
                root->addr = makeNewNode("PRINT", children, 1);
                //printf("HELLO WORLD\n");
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 36:
            if (!strcmp(temp_child->value, "whichId2"))
                temp_child->inh = root->syn;

            createAST(temp_child);

            if (!strcmp(temp_child->value, "whichId2"))
            {
                root->addr = temp_child->addr;
                deleteNode(freenode);
                return;
            }

            if (!strcmp(temp_child->value, "ID"))
                root->syn = temp_child;

            break;

        case 37:
            createAST(temp_child);
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 38:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 39:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 40:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "sign"))
            {
                children[count++] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "index"))
            {
                children[count++] = temp_child->addr;
                children[2] = root->inh;
                children[3] = makeNewNode("INDEX", children, 2);
                children[0] = children[2];
                children[1] = children[3];
                root->addr = makeNewNode("ARR-PRINT", children, 2);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 41:
            createAST(temp_child);
            root->addr = root->inh;
            return;

        case 42:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 43:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 44:
            createAST(temp_child);
            if (count == 0)
                children[count++] = root->inh;
            if (!strcmp(temp_child->value, "newArithmeticExpr"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("ARRAY_ACCESS", children, 2);
                // printf("IS NODE KA NAAM HAI:%s\n", root->addr->value);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 45:
            // printf("*****WHICH ID'S ADDR IS*****:%s\n", root->addr->value);
            createAST(temp_child);
            root->addr = root->inh;
            return;

        case 46:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 47:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 48:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 49:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 50:
            createAST(temp_child);
            root->addr = NULL;
            return;

        case 51:
            if (!strcmp(temp_child->value, "whichId"))
            {
                temp_child->inh = root->syn;
                // if (temp_child->inh != NULL)
                //     printf("whichId's inh is:%s\n", temp_child->inh->value);
                // else
                //     printf("WHICH ID KA INH_NULL\n");
            }

            createAST(temp_child);

            if (!strcmp(temp_child->value, "whichId"))
            {
                // if(temp_child->addr!=NULL)
                //     printf("whichId's addr is:%s\n",temp_child->addr->value);
                // else
                //     printf("WHICH ID KA NULL\n");
                root->addr = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            if (!strcmp(temp_child->value, "ID"))
                root->syn = temp_child;
            break;

        case 52:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 53:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 54:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 55:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 56:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 57:
            createAST(temp_child);
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 58:
            createAST(temp_child);
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 59:
            if (!strcmp(temp_child->value, "newA1"))
                temp_child->inh = root->syn;

            createAST(temp_child);
            if (!strcmp(temp_child->value, "newA1"))
            {
                root->addr = temp_child->syn;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            if (!strcmp(temp_child->value, "newTerm"))
            {
                root->syn = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 60:
            if (!strcmp(temp_child->value, "newA1"))
                temp_child->inh = root->addr;
            createAST(temp_child);
            if (!strcmp(temp_child->value, "op1"))
            {
                node_name = temp_child->addr->value;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "newA1"))
            {
                root->syn = temp_child->syn;
                deleteNode(freenode);
                return;
            }
            else if (!strcmp(temp_child->value, "newTerm"))
            {
                children[count++] = root->inh;
                children[count++] = temp_child->addr;
                root->addr = makeNewNode(node_name, children, 2);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 61:
            // check rule once//checked annd corrected
            root->syn = root->inh;
            createAST(temp_child);
            return;

        case 62:
            if (!strcmp(temp_child->value, "newA2"))
                temp_child->inh = root->syn;

            createAST(temp_child);

            if (!strcmp(temp_child->value, "newA2"))
            {
                root->addr = temp_child->syn;
                deleteNode(freenode);
                return;
            }

            if (!strcmp(temp_child->value, "newNextTerm"))
            {
                root->syn = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 63:
            if (!strcmp(temp_child->value, "newA2"))
                temp_child->inh = root->addr;
            createAST(temp_child);
            if (!strcmp(temp_child->value, "op2"))
            {
                node_name = temp_child->addr->value;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "newA2"))
            {
                root->syn = temp_child->syn;
                deleteNode(freenode);
                return;
            }
            else if (!strcmp(temp_child->value, "newNextTerm"))
            {
                children[count++] = root->inh;
                children[count++] = temp_child->addr;
                root->addr = makeNewNode(node_name, children, 2);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 64:
            // check rule once
            root->syn = root->inh;
            createAST(temp_child);
            return;

        case 65:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "startExpr"))
            {
                root->addr = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 66:
            // top down
            createAST(temp_child);
            // only one child possible
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 67:
            // top down
            createAST(temp_child);
            if (!strcmp(temp_child->value, "after_u1"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("U1_MINUS", children, 2);
                deleteNode(freenode);
                return;
            }

            if (!strcmp(temp_child->value, "MINUS"))
            {
                children[count++] = temp_child;
            }

            break;

        case 68:
            // top down
            createAST(temp_child);
            if (!strcmp(temp_child->value, "after_u1"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("U1_PLUS", children, 2);
                deleteNode(freenode);
                return;
            }

            if (!strcmp(temp_child->value, "PLUS"))
            {
                children[count++] = temp_child;
            }

        case 69:

            createAST(temp_child);
            if (!strcmp(temp_child->value, "startExpr"))
            {
                root->addr = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 70:
            // top down
            createAST(temp_child);
            // only one child possible
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 71:

            // don't call createast
            root->addr = temp_child;
            return;

        case 72:
            // don't call createast
            root->addr = temp_child;
            return;

        case 73:
            // top down
            createAST(temp_child);
            // only one child possible
            // printf("temp child is:%s\n", temp_child->value);
            // printf("CHILD1 of temp:%s\n", temp_child->addr->children->value);
            // if (temp_child->addr->children->nextSibling == NULL)
            //     printf("CHILD2 of temp:NULL\n");
            // else
            //     printf("CHILD2 of temp:%s\n", temp_child->addr->children->nextSibling->value);
            root->addr = temp_child->addr;
            // printf("THIS IS;%s\n",root->addr->value);
            // printf("CHILD1:%s\n", root->addr->children->value);
            //  if (root->addr->children->nextSibling == NULL)
            //      printf("CHILD2:NULL\n");
            //  else
            //      printf("CHILD2:%s\n", root->addr->children->nextSibling->value);
            deleteNode(freenode);
            return;

        case 74:
            // top down
            createAST(temp_child);
            // only one child possible
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 75:
            // top down
            // marker
            if (!strcmp(temp_child->value, "ID"))
            {
                root->syn = temp_child;
            }
            if (!strcmp(temp_child->value, "whichStmt"))
            {
                temp_child->inh = root->syn;
            }

            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "whichStmt"))
            {
                root->addr = temp_child->addr;
                // printf("element is %s\n", root->addr->value);
                //  printf("CHILD1:%s\n", root->addr->children->value);
                //  printf("CHILD2:%s\n", root->addr->children->nextSibling->value);
                deleteNode(freenode);
                //  printf("root's left child is %s\n",root->children->value);
                //  if(root->children->nextSibling == NULL)
                //      printf("root's right child is NULL\n");
                //  printf("ROOT'S ADDRESS:%s\n",root->addr->value);
                // free(temp_child);
                return;
            }

            break;

        case 76:
            // only one child possible
            temp_child->inh = root->inh;
            createAST(temp_child);
            root->addr = temp_child->addr;
            // printf("CHILD1:%s\n",root->addr->children->value);
            // printf("CHILD2:%s\n",root->addr->children->nextSibling->value);
            deleteNode(freenode);
            return;

        case 77:
            // only one child possible
            temp_child->inh = root->inh;
            createAST(temp_child);
            // printf("VALUE IS THIS:%s\n", temp_child->addr->value);
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 78:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "expression"))
            {
                children[count++] = root->inh;
                // if (temp_child->addr != NULL)
                //     printf("VALUE:%s\n", temp_child->addr->value);
                // else
                //     printf("NULL\n");
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("LVALUEID", children, 2);
                // printf("IS NODE KA NAAM HAI:%s\n", root->addr->value);
                //  printf("CHILD1:%s\n",root->addr->children->value);
                //  printf("CHILD2:%s\n",root->addr->children->nextSibling->value);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            break;

        case 79:
            // top down
            createAST(temp_child);
            if (!strcmp(temp_child->value, "newArithmeticExpr"))
            {
                children[count++] = root->inh;
                children[count++] = temp_child->addr;
                temp = makeNewNode("ARRAY_ACCESS", children, 2);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "expression"))
            {
                children[1] = temp_child->addr;
                children[0] = temp;
                root->addr = makeNewNode("LVALUEARRAY", children, 2);
                // printf("DOOSRA BACCHA IS:%s\n",root->addr->children->astnextSibling->value);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            break;

        case 80:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "optional"))
            {
                children[count++] = temp_child->syn;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "ID"))
            {
                temp = temp_child;
            }
            else if (!strcmp(temp_child->value, "idList"))
            {
                children[count++] = temp_child->syn;
                children[1] = makeNewNode("ARGUMENTS", children, 2);
                children[0] = temp;
                root->addr = makeNewNode("MODULE-INVOKE", children, 2);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 81:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "idList2"))
            {
                root->syn = temp_child->syn;
                temp_child = temp_child->next;
                deleteNode(freenode);
                continue;
            }

            break;

        case 82:
            root->syn = NULL;
            deleteNode(freenode);
            return;

        case 83:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "actualParameter"))
            {
                temp = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "moreId"))
            {
                root->syn = insert_at_beginning(temp_child->syn, temp);
                deleteNode(freenode);
                return;
            }

        case 84:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "actualParameter"))
            {
                temp = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "moreId"))
            {
                root->syn = insert_at_beginning(temp_child->syn, temp);
                deleteNode(freenode);
                return;
            }

        case 85:
            root->syn = NULL;
            deleteNode(freenode);
            return;

        case 86:
            // topdown
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "sign"))
            {
                children[count++] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "aVar"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("PARAMETER_NUM", children, 2);
                deleteNode(freenode);
                return;
            }

        case 87:
            // top down
            createAST(temp_child);
            // bottom up
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 88:
            // top down
            createAST(temp_child);
            // bottom up
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 89:
            // top down
            createAST(temp_child);
            // bottom up
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 90:
            // top down
            if (!strcmp(temp_child->value, "ab1"))
            {
                temp_child->inh = root->syn;
            }
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "ab1"))
            {
                root->addr = temp_child->syn;
                deleteNode(freenode);
                return;
            }
            if (!strcmp(temp_child->value, "anyTerm"))
            {
                root->syn = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

        case 91:
            // top down
            if (!strcmp(temp_child->value, "ab1"))
            {
                temp_child->inh = root->addr;
            }
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "bop"))
            {
                node_name = temp_child->addr->value;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "anyTerm"))
            {
                children[count++] = root->inh;
                children[count++] = temp_child->addr;
                root->addr = makeNewNode(node_name, children, 2);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "ab1"))
            {
                root->syn = temp_child->syn;
                deleteNode(freenode);
                return;
            }

        case 92:
            root->syn = root->inh;
            deleteNode(freenode);
            return;

        case 93:
            // top down
            if (!strcmp(temp_child->value, "ab2"))
            {
                temp_child->inh = root->syn;
            }
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "ab2"))
            {
                root->addr = temp_child->addr;
                deleteNode(freenode);
                return;
            }

            else if (!strcmp(temp_child->value, "arithmeticExpr"))
            {
                root->syn = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

        case 94:
            // top down
            createAST(temp_child);
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 95:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "relationalOp"))
            {
                node_name = temp_child->addr->value;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "arithmeticExpr"))
            {
                children[count++] = root->inh;
                children[count++] = temp_child->addr;
                root->addr = makeNewNode(node_name, children, 2);
                deleteNode(freenode);
                return;
            }

        case 96:
            root->addr = root->inh;
            deleteNode(freenode);
            return;

        case 97:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "MINUS"))
            {
                children[count++] = temp_child;
            }

            else if (!strcmp(temp_child->value, "after_unary"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("U_MINUS", children, 2);
                deleteNode(freenode);
                return;
            }

            break;

        case 98:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "PLUS"))
            {
                children[count++] = temp_child;
            }

            else if (!strcmp(temp_child->value, "after_unary"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("U_PLUS", children, 2);
                deleteNode(freenode);
                return;
            }

            break;

        case 99:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "arithmeticExpr"))
            {
                root->addr = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }

            break;

        case 100:
            // top-down
            createAST(temp_child);
            // only one child possible
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 101:
            // top-down
            if (!strcmp(temp_child->value, "a1"))
                temp_child->inh = root->syn;
            createAST(temp_child);
            // bottom-up
            if (!strcmp(temp_child->value, "term"))
                root->syn = temp_child->addr;
            else if (!strcmp(temp_child->value, "a1"))
                root->addr = temp_child->syn;
            temp_child = temp_child->nextSibling;
            deleteNode(freenode);
            continue;

        case 102:
            // top-down
            if (!strcmp(temp_child->value, "a1"))
                temp_child->inh = root->addr;
            createAST(temp_child);
            // bottom-up
            if (!strcmp(temp_child->value, "a1"))
                root->syn = temp_child->syn;
            else if (!strcmp(temp_child->value, "op1"))
            {
                node_name = temp_child->addr->value;
                // if(temp_child->addr == NULL) printf("NULL hai ye\n");
            }
            else if (!strcmp(temp_child->value, "term"))
            {
                children[count++] = root->inh;
                children[count++] = temp_child->addr;
                // printf("\nbefore making new node:%s\n", node_name);
                root->addr = makeNewNode(node_name, children, 2);
            }
            temp_child = temp_child->nextSibling;
            deleteNode(freenode);
            continue;

        case 103:
            // top-down
            root->syn = root->inh;
            createAST(temp_child);
            return;

        case 104:
            // top-down
            if (!strcmp(temp_child->value, "a2"))
                temp_child->inh = root->syn;
            createAST(temp_child);
            // bottom-up
            if (!strcmp(temp_child->value, "nextTerm"))
                root->syn = temp_child->addr;
            if (!strcmp(temp_child->value, "a2"))
                root->addr = temp_child->syn;
            temp_child = temp_child->nextSibling;
            deleteNode(freenode);
            continue;

        case 105:
            // top-down
            if (!strcmp(temp_child->value, "a2"))
                temp_child->inh = root->addr;
            createAST(temp_child);
            // bottom-up
            if (!strcmp(temp_child->value, "a2"))
                root->syn = temp_child->syn;
            else if (!strcmp(temp_child->value, "op2"))
                node_name = temp_child->addr->value;
            if (!strcmp(temp_child->value, "nextTerm"))
            {
                children[count++] = root->inh;
                children[count++] = temp_child->addr;
                root->addr = makeNewNode(node_name, children, 2);
            }
            temp_child = temp_child->nextSibling;
            deleteNode(freenode);
            continue;

        case 106:
            root->syn = root->inh;
            createAST(temp_child);
            return;

        case 107:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "arithmeticOrBooleanExpr"))
            {
                root->addr = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 108:
            createAST(temp_child);
            root->addr = temp_child->addr;
            deleteNode(freenode);
            return;

        case 109:
            root->addr = temp_child;
            // printf("*********VALUE%s*********\n", temp_child->value);
            createAST(temp_child);
            break;

        case 110:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 111:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 112:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 113:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 114:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 115:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 116:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 117:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 118:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 119:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 120:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 121:
            createAST(temp_child);

            if (!strcmp(temp_child->value, "idList2"))
            {
                children[1] = temp_child->syn;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "dataType"))
            {
                children[0] = temp_child->addr;
                root->addr = makeNewNode("DECLARE", children, 2);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 122:
            //koi error aaye to check this thing first
            createAST(temp_child);
            if (!strcmp(temp_child->value, "caseStmt"))
            {
                children[1] = temp_child->syn;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            else if(!strcmp(temp_child->value,"ID")){
                children[0] = temp_child;
            }
            else if (!strcmp(temp_child->value, "default"))
            {
                children[2] = temp_child->syn; 
                root->addr = makeNewNode("SWITCH",children,3);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 123:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "value"))
            {
                temp = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "statements"))
            {
                temp1 = temp_child->syn;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "post"))
            {
                root->syn = insert_pair_at_beginning(temp_child->syn, temp, temp1);
                // if(temp_child->syn== NULL) printf("TEMP_child ka syn IS NULL\n");
                // if(temp1== NULL) printf("TEMP1 IS NULL\n");
                //else printf("WHAT IS IN THE NEXT:%s/n",root->syn->next->value);
                deleteNode(freenode);
                return;
            }
            break;

        case 124:
            createAST(temp_child);
            root->syn = temp_child->syn;
            deleteNode(freenode);
            return;

        case 125:
            createAST(temp_child);
            root->syn = NULL;
            return;

        case 126:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 127:
            createAST(temp_child);
            root->addr = temp_child;
            return;

        case 128:
            createAST(temp_child);
            root->addr = temp_child;
            break;

        case 129:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "statements"))
            {
                root->syn = temp_child->syn;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 130:
            createAST(temp_child);
            root->syn = NULL;
            return;

        case 131:
            createAST(temp_child);
            if(!strcmp(temp_child->value, "ID"))
                children[count++] = temp_child;
            if (!strcmp(temp_child->value, "range_for"))
            {
                children[count++] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "statements"))
            {
                children[count++] = temp_child->syn;
                root->addr = makeNewNode("FOR", children, 3);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 132:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "arithmeticOrBooleanExpr"))
            {
                children[count++] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "statements"))
            {
                children[count++] = temp_child->syn;
                root->addr = makeNewNode("WHILE", children, 2);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            break;

        case 133:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "sign"))
            {
                children[0] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "NUM") && temp_child->nextSibling != NULL)
            {
                children[1] = temp_child;
                temp = makeNewNode("LEFT-FOR", children, 2);
            }
            else if (!strcmp(temp_child->value, "NUM") && temp_child->nextSibling == NULL)
            {
                children[1] = temp_child;
                children[1] = makeNewNode("RIGHT-FOR", children, 2);
                //printf("RIGHT FOR KA BACCHA:%s\n",children[1]->children->value);
                children[0] = temp;
                root->addr = makeNewNode("RANGE-FOR", children, 2);
                return;
            }
            break;

        case 134:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "sign"))
            {
                children[0] = temp_child->addr;
                // printf("ISME GHUSA\n");
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "index") && temp_child->nextSibling != NULL)
            {
                children[1] = temp_child->addr;
                temp = makeNewNode("LEFT-INDEX", children, 2);
                // printf("left index ka left child:%s\n",temp->children->value);
                // printf("left index ka right child:%s\n",temp->children->astnextSibling->value);
                temp_child = temp_child->nextSibling;
                deleteNode(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "index") && temp_child->nextSibling == NULL)
            {
                children[1] = temp_child->addr;
                // printf("BEFORE RIGHT-INDEX VALUE CHECKING:%s\n",temp->children->astnextSibling->value);
                children[1] = makeNewNode("RIGHT-INDEX", children, 2);
                children[0] = temp;
                // printf("BEFORE VALUE CHECKING:%s\n",temp->children->astnextSibling->value);
                root->addr = makeNewNode("RANGE", children, 2);
                // printf("VALUE CHECKING:%s\n",root->addr->children->children->astnextSibling->value);
                deleteNode(freenode);
                return;
            }
            break;

        case 135:
            // top down
            // printf("*********************case 135 executed\n");
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "ID"))
            {

                temp = temp_child;
            }

            else if (!strcmp(temp_child->value, "moreId2"))
            {
                root->syn = insert_at_beginning(temp_child->syn, temp);
                deleteNode(freenode);
                return;
            }
            break;

        case 136:
            // top down

            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "ID"))
            {

                temp = temp_child;
            }

            else if (!strcmp(temp_child->value, "moreId2"))
            {
                root->syn = insert_at_beginning(temp_child->syn, temp);
                deleteNode(freenode);
                return;
            }

            break;

        case 137:
            // printf("137 me ghusa\n");
            root->syn = NULL;
            deleteNode(freenode);
            return;

        default:
            printf("No such rule exists");
        }

        temp_child = temp_child->nextSibling;
    }

    return;
}

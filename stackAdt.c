/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#include "stackAdtDef.h"

// function definitions

void s_push(stackElement* element)
{
    //creating back link
    element->link = s_top;
    
    // change top
    s_top  = element;
}

void s_pop()
{
    stackElement* new_top = s_top;
    //update top
    s_top = s_top->link;
    // remove the top
    free(new_top);
}

int isEmpty()
{
    if(s_top == NULL)
    {
        return 1;
    }

    else
    {
        return 0;
    }
}



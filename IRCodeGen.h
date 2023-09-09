/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#include "treeDef.h"
#define QUAD_SIZE 5000
#define TEMP_LEN 10

union argument
{
    ST_ENTRY* arg_var;
    int arg_num;
    double arg_rnum;
    char* arg_bool;
};

/*
entry is 0 for:ID
         1 for:NUM
         2 for:RNUM
         3 for:BOOL
*/

typedef struct QuadEntry 
{
    union argument arg;
    int entry;// indicates which union field is populated
}qEn;

typedef struct Quadruple
{
    char op[20];
    qEn arg1;
    qEn arg2;
    qEn index1;
    qEn index2;
    qEn result;
    FN_ENTRY* arg_fn_name; //for function table entry
    char* goTolabel; //jump to this label
    char* instruction;  //if or goTO or blah blah
    char* label;  //contsructs own label
}quad;

quad quadTable[QUAD_SIZE];
//for quad entry

int count; // initialize count to 0 in the driver function

//in order to give names to temporaries
int variable_count; //initialize to zero in the driver function

//for labels
int label_count;

//symbol table for storing temporaries,allocate memory in driver function
struct id_symbol_table* temporaries_st;

//for function calls
char* return_label;

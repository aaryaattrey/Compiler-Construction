/*
Group : 11
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/


void initUninitialisedData(FILE* fp){  //to reserve memory for uninitialised data section: section.bss
    fprintf(fp,"section .bss\n");
    for(int i=0;i<count;i++){
        quad temp = quadTable[i];
        if(!strcasecmp(temp.op,"declare") && temp.result.arg.arg_var->is_array!=1){  //for non array elements
            char* label = temp.result.arg.arg_var->id_lexeme;
            fprintf(fp,"\t\t\t %s resb %d \n",label,temp.result.arg.arg_var->width); //storing width bytes for temporary 
        }
    }
}

void initDataSection(FILE* fp){    //to store messages :section.data
    fprintf(fp,"section .data\n");
    fprintf(fp,"\t\t\t inputMsgInteger: db \"Enter an integer value\",0\n"); //for prompting the user to enter value
    fprintf(fp,"\t\t\t trueMsg: db \"true\",0 \n");
    fprintf(fp,"\t\t\t falseMsg: db \"false\",0 \n");
    fprintf(fp,"\t\t\t format: db \"%%hi\",0 \n"); //this is for integer
    fprintf(fp,"\t\t\t formatBool: db \"%%hhi\",0 \n"); // this is for boolean
    fprintf(fp,"\t\t\t lineBreak: db 0ah\n");
}

void codegen(char * name){
    FILE* fp = fopen(name,"w");
    fprintf(fp,"global main\n");
    fprintf(fp,"extern printf,scanf\n\n");
    initDataSection(fp);
    initUninitialisedData(fp);
    fprintf(fp,"section .text\n");
    fprintf(fp,"main:\n");
    fprintf(fp,"\t\t\t push rbp\n");
    fprintf(fp,"\t\t\t mov rbp,rsp\n");
    fprintf(fp,"\t\t\t sub rsp,16\n");
    for(int i=0;i<count;i++){
       quad curr = quadTable[i];
       if(!strcasecmp(curr.op,"=")){
        //lval will be id
        //rval will be a temporary
       
        ST_ENTRY* lval = curr.result.arg.arg_var;
        qEn rval = curr.arg1;
        if(lval->is_array==0){
            
            if(rval.entry==0){//rval is a temporary or a id
                
                if(lval->width == 2){
                    fprintf(fp,"\t\t\t mov cx,[rbp - %d];moving rhs temporary/id:%s to temp reg rcx \n ",rval.arg.arg_var->offset,rval.arg.arg_var->id_lexeme);
                    fprintf(fp,"\t\t\t mov [rbp - %d],cx ;loading into %s's memory \n ",lval->offset,lval->id_lexeme);
                }
                else if(lval->width == 1){
                    fprintf(fp,"\t\t\t mov cl,[rbp - %d];moving rhs temporary/id:%s to temp reg rcx \n ",rval.arg.arg_var->offset,rval.arg.arg_var->id_lexeme);
                    fprintf(fp,"\t\t\t mov [rbp - %d],cl ;loading into %s's memory \n ",lval->offset,lval->id_lexeme);
                }
                else{
                    fprintf(fp,"\t\t\t mov ecx,[rbp - %d];moving rhs temporary/id:%s to temp reg rcx \n ",rval.arg.arg_var->offset,rval.arg.arg_var->id_lexeme);
                    fprintf(fp,"\t\t\t mov [rbp - %d],ecx ;loading into %s's memory \n ",lval->offset,lval->id_lexeme);
                }
            }
            else if(rval.entry==1){ //rval is an int
                
                fprintf(fp,"\t\t\t mov WORD [rbp - %d],%d ;loading into %s \n",lval->offset,rval.arg.arg_num,lval->id_lexeme);
            }
            else if(rval.entry==2){// rval is a float
                
                fprintf(fp,"\t\t\t mov DWORD [rbp - %d],%lf \n",lval->offset,rval.arg.arg_rnum);
            }
            else if(rval.entry==3){ //rval is boolean
                
                if(!strcasecmp(rval.arg.arg_bool,"true")){
                    fprintf(fp,"\t\t\t mov BYTE [rbp - %d],1 \n",lval->offset);
                }
                else{
                    fprintf(fp,"\t\t\t mov BYTE [rbp - %d],0 \n",lval->offset);
                }
            }
            
        }

       }
       else if(!strcasecmp(curr.op,"PRINT")){\
            
            int ent = curr.result.entry;
             
            if(curr.label!=NULL)
                fprintf(fp,"\t\t\t %s:\n ",curr.label);
             
            if(ent == 0 && curr.result.arg.arg_var->is_array != 1)
            {
                if(ent == 0){
                    ST_ENTRY* var = curr.result.arg.arg_var;
                    if(var->is_array == 0){
                        fprintf(fp,"\t\t\t xor eax, eax \n ");
                        fprintf(fp,"\t\t\t mov esi,[rbp - %d]\n ",var->offset);
                        if(!strcasecmp(var->type.id_type.id_dt,"INTEGER"))
                            fprintf(fp,"\t\t\t lea rdi,[format]\n");
                        else if(!strcasecmp(var->type.id_type.id_dt,"BOOLEAN"))
                            fprintf(fp,"\t\t\t lea rdi,[formatBool]\n");
                        fprintf(fp,"\t\t\t call printf  \n"); 
                        fprintf(fp,"\t\t\t xor eax,eax\n");
                        fprintf(fp,"\t\t\t lea rdi,[lineBreak]\n");
                        fprintf(fp,"\t\t\t call printf\n");
                    }
                }
                else if(ent == 1){
                    fprintf(fp,"\t\t\t xor eax, eax \n ");
                    fprintf(fp,"\t\t\t mov esi,%d\n",curr.result.arg.arg_num);
                    fprintf(fp,"\t\t\t lea rdi,[format]\n");
                    fprintf(fp,"\t\t\t call printf\n");
                    fprintf(fp,"\t\t\t xor eax,eax\n");
                    fprintf(fp,"\t\t\t lea rdi,[lineBreak]\n");
                    fprintf(fp,"\t\t\t call printf\n");
                }
                else if(ent == 2){
                    fprintf(fp,"\t\t\t xor eax, eax \n ");
                    fprintf(fp,"\t\t\t mov esi,%lf\n",curr.result.arg.arg_rnum);
                    fprintf(fp,"\t\t\t lea rdi,[format]\n");
                    fprintf(fp,"\t\t\t call printf  \n");
                    fprintf(fp,"\t\t\t xor eax,eax\n");
                    fprintf(fp,"\t\t\t lea rdi,[lineBreak]\n");
                    fprintf(fp,"\t\t\t call printf\n");
                }
                else if(ent == 3){
                    if(!strcasecmp(curr.result.arg.arg_bool,"true")){
                        fprintf(fp,"\t\t\t xor eax, eax \n ");
                        fprintf(fp,"\t\t\t mov esi, trueMsg\n");
                        fprintf(fp,"\t\t\t call printf  \n"); 
                        fprintf(fp,"\t\t\t xor eax,eax\n");
                        fprintf(fp,"\t\t\t lea rdi,[lineBreak]\n");
                        fprintf(fp,"\t\t\t call printf\n");  
                    }
                    else{
                        fprintf(fp,"\t\t\t xor eax, eax \n ");
                        fprintf(fp,"\t\t\t mov esi, falseMsg\n");
                        fprintf(fp,"\t\t\t call printf  \n");
                        fprintf(fp,"\t\t\t xor eax,eax\n");
                        fprintf(fp,"\t\t\t lea rdi,[lineBreak]\n");
                        fprintf(fp,"\t\t\t call printf\n"); 
                    }
                }
            }  
            else{
                //array-case  
                ST_ENTRY* var = curr.arg1.arg.arg_var;
                int off = curr.result.arg.arg_num;
                fprintf(fp,"\t\t\t xor eax, eax \n ");
                fprintf(fp,"\t\t\t mov esi,[rbp - %d]\n ",off);
                if(!strcasecmp(var->type.arr_type.arr_dt,"INTEGER"))
                    fprintf(fp,"\t\t\t lea rdi,[format]\n");
                else if(!strcasecmp(var->type.arr_type.arr_dt,"BOOLEAN"))
                    fprintf(fp,"\t\t\t lea rdi,[formatBool]\n");
                fprintf(fp,"\t\t\t call printf  \n"); 
                fprintf(fp,"\t\t\t xor eax,eax\n");
                fprintf(fp,"\t\t\t lea rdi,[lineBreak]\n");
                fprintf(fp,"\t\t\t call printf\n");
            }
        }
    
        else if(!strcmp(curr.op,"PLUS") ||!strcmp(curr.op,"MINUS") || !strcmp(curr.op,"MUL") || !strcmp(curr.op,"DIV")){
            qEn leftOp = curr.arg1;
            qEn rightOp = curr.arg2;
            int realFlag=0;
            //use rax for leftOp
            if(leftOp.entry==0){
                ST_ENTRY* t = leftOp.arg.arg_var;
                if(!strcasecmp(t->type.id_type.id_dt,"REAL")){
                    realFlag=1;
                }
                fprintf(fp,"\t\t\t mov ax,[rbp-%d];moving leftop:%s from memory to ax \n ",t->offset,t->id_lexeme);
            }
            else if(leftOp.entry==1){
                fprintf(fp,"\t\t\t mov ax,%d \n",leftOp.arg.arg_num);
            }
            else if(leftOp.entry==2){
                realFlag = 1;
                fprintf(fp,"\t\t\t mov ax,%lf \n",leftOp.arg.arg_rnum);
            }

            //use rbx for rightOp
            if(rightOp.entry==0){
                ST_ENTRY* t = rightOp.arg.arg_var;
                if(!strcasecmp(t->type.id_type.id_dt,"REAL")){
                    realFlag=1;
            }
                fprintf(fp,"\t\t\t mov bx,[rbp-%d] ;moving %s from memory to rbx \n",t->offset,t->id_lexeme);
            }
            else if(rightOp.entry==1){
                fprintf(fp,"\t\t\t mov bx,%d \n",rightOp.arg.arg_num);
            }
            else if(rightOp.entry==2){
                realFlag=1;
                fprintf(fp,"\t\t\t mov bx,%lf \n",rightOp.arg.arg_rnum);
            }

            //move into rcx the result
            ST_ENTRY* res_temp = curr.result.arg.arg_var;
            if(!strcmp(curr.op,"PLUS")){
                fprintf(fp,"\t\t\t add ax,bx \n \t\t\t mov [rbp -%d],ax ;done for PLUS \n",res_temp->offset);
            }
            else if(!strcmp(curr.op,"MINUS")){
                fprintf(fp,"\t\t\t sub ax,bx \n \t\t\t mov [rbp -%d],ax ;done for MINUS \n",res_temp->offset);
            }
            else if(!strcmp(curr.op,"MUL")){//check once
            if(!realFlag)
                fprintf(fp,"\t\t\t imul ax,bx \n \t\t\t mov [rbp -%d],ax ;done for mul\n",res_temp->offset);
            }
            else if(!strcmp(curr.op,"DIV")){
                fprintf(fp,"\t\t\t div bx \n \t\t\t mov [rbp -%d],ax \n",res_temp->offset);
            }
       }

       else if(!strcasecmp(curr.op, "AND") || !strcasecmp(curr.op,"OR")){
        qEn leftOp = curr.arg1;
        qEn rightOp = curr.arg2; 
        //loading left operand
        if(leftOp.entry== 0){
             ST_ENTRY* t = leftOp.arg.arg_var;
             fprintf(fp,"\t\t\t mov al,[rbp-%d] \n",t->offset);
        }
        else if(leftOp.entry== 3){
            if(!strcasecmp(leftOp.arg.arg_bool,"true")){
                fprintf(fp,"\t\t\t mov al,1\n");      
            }

            else if(!strcasecmp(leftOp.arg.arg_bool,"false")){
                fprintf(fp,"\t\t\t mov al,0\n");     
            }    
        }

        //loading right operand
         if(rightOp.entry== 0){
             ST_ENTRY* t = rightOp.arg.arg_var;
             fprintf(fp,"\t\t\t mov bl,[rbp-%d]\n",t->offset);
             
        }
        else if(rightOp.entry== 3){
            if(!strcasecmp(leftOp.arg.arg_bool,"true")){
                fprintf(fp,"\t\t\t mov bl,1\n");      
            }

            else if(!strcasecmp(rightOp.arg.arg_bool,"false")){
                fprintf(fp,"\t\t\t mov bl,0\n");     
            }    
        }
        //anding \ oring
        if(!strcasecmp(curr.op,"AND")){
            fprintf(fp,"\t\t\t and al,bl\n mov [rbp -%d],al\n",curr.result.arg.arg_var->offset);     
        }
        else{
           fprintf(fp,"\t\t\t or al,bl\n mov [rbp -%d],al\n",curr.result.arg.arg_var->offset);      
        }

       }

       else if(!strcasecmp(curr.op,"LT") ||!strcasecmp(curr.op,"<=") || !strcasecmp(curr.op,"LE") || !strcasecmp(curr.op,"GT") || !strcasecmp(curr.op,"GE") || !strcasecmp(curr.op,"NE") || !strcasecmp(curr.op,"EQ") || !strcasecmp(curr.op,"==")){
            if(curr.instruction!=NULL && !strcasecmp(curr.instruction,"IF")){
                qEn leftOp = curr.arg1;
                qEn rightOp = curr.arg2;
                int realFlag=0;
                
                if(curr.label!=NULL)
                    fprintf(fp,"\t\t\t %s:\n ",curr.label);

                if(leftOp.entry==0){
                    ST_ENTRY* t = leftOp.arg.arg_var;
                    if(t->is_array==0){
                        fprintf(fp,"\t\t\t mov ax,[rbp-%d] \n ",t->offset);
                    }
                }
                else if(leftOp.entry==1){
                    fprintf(fp,"\t\t\t mov ax,%d \n ",leftOp.arg.arg_num);
                }
                else if(leftOp.entry==2){
                    realFlag=1;
                    fprintf(fp,"\t\t\t mov xmm0,%lf \n ",leftOp.arg.arg_rnum);
                }


                if(rightOp.entry==0){
                    ST_ENTRY* t = rightOp.arg.arg_var;
                    if(t->is_array==0){
                        fprintf(fp,"\t\t\t mov bx,[rbp-%d] \n ",t->offset);
                    }
                }
                else if(rightOp.entry==1){       
                    fprintf(fp,"\t\t\t mov bx,%d \n ",rightOp.arg.arg_num);
                }
                else if(rightOp.entry==2){
                    realFlag=1;
                    fprintf(fp,"\t\t\t mov xmm1,%lf \n ",rightOp.arg.arg_rnum);
                }
                
                fprintf(fp,"\t\t\t cmp ax,bx \n");
                if(!strcasecmp(curr.op,"LT")){
                    fprintf(fp,"\t\t\t jl %s\n",curr.goTolabel);
                }
                else if(!strcasecmp(curr.op,"LE") || !strcasecmp(curr.op,"<=")){
                    fprintf(fp,"\t\t\t jle %s\n",curr.goTolabel);
                }
                else if(!strcasecmp(curr.op,"GT")){
                    fprintf(fp,"\t\t\t jg %s\n",curr.goTolabel);
                }
                else if(!strcasecmp(curr.op,"GE")){
                    fprintf(fp,"\t\t\t jge %s\n",curr.goTolabel);
                }
                else if(!strcasecmp(curr.op,"EQ") ||strcasecmp(curr.op,"==")){
                    fprintf(fp,"\t\t\t je %s\n",curr.goTolabel);
                }
                else if(!strcasecmp(curr.op,"NE")){
                    fprintf(fp,"\t\t\t jne %s\n",curr.goTolabel);
                }
            }
            else{
                qEn leftOp = curr.arg1;
                qEn rightOp = curr.arg2;
                int realFlag=0;

                if(leftOp.entry==0){
                    ST_ENTRY* t = leftOp.arg.arg_var;
                    if(t->is_array==0){
                        fprintf(fp,"\t\t\t mov ax,[rbp-%d] \n ",t->offset);
                    }
                }
                else if(leftOp.entry==1){
                    fprintf(fp,"\t\t\t mov ax,%d \n ",leftOp.arg.arg_num);
                }
                else if(leftOp.entry==2){
                    realFlag=1;
                    fprintf(fp,"\t\t\t mov xmm0,%lf \n ",leftOp.arg.arg_rnum);
                }

                if(rightOp.entry==0){
                    ST_ENTRY* t = rightOp.arg.arg_var;
                    if(t->is_array==0){
                        fprintf(fp,"\t\t\t mov bx,[rbp-%d] \n ",t->offset);
                    }
                }
                else if(rightOp.entry==1){       
                    fprintf(fp,"\t\t\t mov bx,%d \n ",rightOp.arg.arg_num);
                }
                else if(rightOp.entry==2){
                    realFlag=1;
                    fprintf(fp,"\t\t\t mov xmm1,%lf \n ",rightOp.arg.arg_rnum);
                }


                fprintf(fp,"\t\t\t cmp ax,bx \n");
                if(!strcasecmp(curr.op,"LT")){
                    fprintf(fp,"\t\t\t setl [rbp - %d]\n",curr.result.arg.arg_var->offset);
                }
                else if(!strcasecmp(curr.op,"LE") || !strcasecmp(curr.op,"<=")){
                    fprintf(fp,"\t\t\t setle [rbp - %d]\n",curr.result.arg.arg_var->offset);
                }
                else if(!strcasecmp(curr.op,"GT")){
                    fprintf(fp,"\t\t\t setg [rbp - %d]\n",curr.result.arg.arg_var->offset);
                }
                else if(!strcasecmp(curr.op,"GE")){
                    fprintf(fp,"\t\t\t setge [rbp - %d]\n",curr.result.arg.arg_var->offset);
                }
                else if(!strcasecmp(curr.op,"EQ") || !strcasecmp(curr.op,"==")){
                    fprintf(fp,"\t\t\t sete [rbp - %d]\n",curr.result.arg.arg_var->offset);
                }
                else if(!strcasecmp(curr.op,"NE") || !strcasecmp(curr.op,"!=")){
                   fprintf(fp,"\t\t\t setne [rbp - %d]\n",curr.result.arg.arg_var->offset);
                }
            }
       }

       else if(curr.instruction!=NULL && !strcasecmp(curr.instruction,"GOTO")){
            if(curr.label!=NULL)
                fprintf(fp,"\t\t\t %s:\n ",curr.label);
            
            fprintf(fp,"\t\t\t jmp %s\n",curr.goTolabel);
       }

       else if(curr.instruction!=NULL && !strcasecmp(curr.instruction,"INCREMENT")){
            if(curr.label!=NULL)
                fprintf(fp,"\t\t\t %s:\n ",curr.label);
            
            fprintf(fp,"\t\t\t mov cx,[rbp - %d]\n",curr.arg1.arg.arg_var->offset);
            fprintf(fp,"\t\t\t inc cx\n");
            fprintf(fp,"\t\t\t mov [rbp - %d],cx\n",curr.arg1.arg.arg_var->offset);
       }

       else if(!strcasecmp(curr.op,"GET")){
            qEn id = curr.arg1;
            
            if(curr.label!=NULL)
                fprintf(fp,"\t\t\t %s:\n ",curr.label);
            
            if(id.arg.arg_var->is_array == 0){
                //normal id hai
                
                if(!strcasecmp(id.arg.arg_var->type.id_type.id_dt,"INTEGER")){
                    fprintf(fp,"\t\t\t xor eax, eax\n");
                    fprintf(fp,"\t\t\t lea rdi, [inputMsgInteger]\n");
                    fprintf(fp,"\t\t\t call printf\n");
                    fprintf(fp,"\t\t\t xor eax,eax\n");
                    fprintf(fp,"\t\t\t lea rdi,[lineBreak]\n");
                    fprintf(fp,"\t\t\t call printf\n");
                    fprintf(fp,"\t\t\t mov eax,0\n");
                    fprintf(fp,"\t\t\t lea rdi, [format]\n");
                    fprintf(fp,"\t\t\t lea rsi, [rbp - %d]\n",id.arg.arg_var->offset);
                    fprintf(fp,"\t\t\t call scanf\n");
                }
                else if(!strcasecmp(id.arg.arg_var->type.id_type.id_dt,"BOOLEAN")){
                    fprintf(fp,"\t\t\t xor eax, eax\n");
                    fprintf(fp,"\t\t\t lea rdi, [inputMsgBoolean]\n");
                    fprintf(fp,"\t\t\t call printf\n");
                    fprintf(fp,"\t\t\t xor eax,eax\n");
                    fprintf(fp,"\t\t\t lea rdi,[lineBreak]\n");
                    fprintf(fp,"\t\t\t call printf\n");
                    fprintf(fp,"\t\t\t mov eax,0\n");
                    fprintf(fp,"\t\t\t lea rdi, [formatBool]\n");
                    fprintf(fp,"\t\t\t lea rsi, [rbp - %d]\n",id.arg.arg_var->offset);
                    fprintf(fp,"\t\t\t call scanf\n");
                }
            }

            else{
                    if(!strcasecmp(id.arg.arg_var->type.arr_type.arr_dt,"INTEGER")){
                        fprintf(fp,"\t\t\t xor eax, eax\n");
                        fprintf(fp,"\t\t\t lea rdi, [inputMsgInteger]\n");
                        fprintf(fp,"\t\t\t call printf\n");
                        fprintf(fp,"\t\t\t xor eax,eax\n");
                        fprintf(fp,"\t\t\t lea rdi,[lineBreak]\n");
                        fprintf(fp,"\t\t\t call printf\n");
                        fprintf(fp,"\t\t\t mov eax,0\n");
                        fprintf(fp,"\t\t\t lea rdi, [formatBool]\n");
                        fprintf(fp,"\t\t\t lea rsi, [rbp - %d]\n",curr.result.arg.arg_num);
                        fprintf(fp,"\t\t\t call scanf\n");   
                    }

                    else if(!strcasecmp(id.arg.arg_var->type.arr_type.arr_dt,"BOOLEAN")){
                        fprintf(fp,"\t\t\t xor eax, eax\n");
                        fprintf(fp,"\t\t\t lea rdi, [inputMsgBoolean]\n");
                        fprintf(fp,"\t\t\t call printf\n");
                        fprintf(fp,"\t\t\t xor eax,eax\n");
                        fprintf(fp,"\t\t\t lea rdi,[lineBreak]\n");
                        fprintf(fp,"\t\t\t call printf\n");
                        fprintf(fp,"\t\t\t mov eax,0\n");
                        fprintf(fp,"\t\t\t lea rdi, [formatBool]\n");
                        fprintf(fp,"\t\t\t lea rsi, [rbp - %d]\n",curr.result.arg.arg_num);
                        fprintf(fp,"\t\t\t call scanf\n");    
                    }
            }
       }
    }
    fprintf(fp,"\t\t\t return:\n");
    fprintf(fp,"\t\t\t add rsp,16\n");
    fprintf(fp,"\t\t\t leave\n");
    fprintf(fp,"\t\t\t ret\n");
    fclose(fp);
}
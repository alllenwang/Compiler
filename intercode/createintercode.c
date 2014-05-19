#include "createintercode.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../table/table.h"
#include "../tree/tree.h"
#include "intercode.h"
int temp_num = 0;
int label_num = 0;
Operand *args[10];
InterCode *translate_exp(struct Node *node,Operand *op)
{
	if(node->child[1] != NULL)
	{
		if(node->child[1]->type == eASSIGNOP)
		{
			struct Node *exp1 = node->child[0];
			struct Node *exp2 = node->child[2];
			if(exp1->child[0]->type == eID && exp1->child[1] == NULL)
			{
				temp_num ++;
				Operand t1;
				t1.kind = TEMP;
				t1.u.temp_no = temp_num;
				
				InterCode *code1 = translate_exp(exp2,&t1);
				InterCode *code2 = (InterCode *)malloc(sizeof(InterCode));
				code2->kind = ASSIGN;
				code2->u.assign.left.kind = VARIABLE;
				code2->u.assign.left.u.var_no = numberofvar(exp1->child[0]->ID);
				code2->u.assign.right = t1;
				code2->last = NULL;
				code2->next = NULL;
				InterCodeInsert(code2);
				
				if(op != NULL)
				{
					InterCode *code3 = (InterCode *)malloc(sizeof(InterCode));
					code3->kind = ASSIGN;
					code3->u.assign.left.kind = op->kind;
					code3->u.assign.left.u = op->u;
					code3->u.assign.right.kind = VARIABLE;
					code3->u.assign.right.u.var_no = numberofvar(exp1->child[0]->ID);
					code3->last = NULL;
					code3->next = NULL;
					InterCodeInsert(code3);
				}
			}
			else if(exp1->child[1] != NULL && exp1->child[1]->type == eLB)
			{
				int num = 0;
				struct Node *n1 = exp1->child[0];
				struct Node *n2 = exp1->child[2];
				struct Node *id;
				for(id = n1; id->type != eID; id = id->child[0]);
				struct table_node *array = table_check(id->ID,1);
				int size[10];
				int i = 0;
				for( ; i < 10 ; i ++)
					size[i] = 0;
				struct array_node * arraysize = array->array_next;
				for(i = 0; arraysize != NULL ; arraysize = arraysize->next)
				{
					num ++;
					size[i] = arraysize->size;
					i ++;
				//	printf("%d\n",size[i]);
				}
				for(i = 0; size[i] != 0 ; i ++)
				{
					int j = i + 1;
					size[i] = 1;
					for( ; size[j] != 0 ; j ++)
					{
						size[i] = size[i] * size[j];
					}
				}
				
				temp_num ++;
				Operand t1;
				t1.kind = TEMP;
				t1.u.temp_no = temp_num;
				InterCode *code1 = (InterCode *)malloc(sizeof(InterCode));
				code1->kind = ASSIGN;
				code1->u.assign.left = t1;
				code1->u.assign.right.kind = CONSTANT;
				code1->u.assign.right.u.value = 0;
				code1->last = NULL;
				code1->next = NULL;
				InterCodeInsert(code1);
				
				int pos = num;
				while(n1->type != eID)
				{
					pos --;
					temp_num ++;
					Operand t2;
					t2.kind = TEMP;
					t2.u.temp_no = temp_num;
			
					InterCode *code1 = translate_exp(n2,&t2);
					InterCode *code2 = (InterCode *)malloc(sizeof(InterCode));
					code2->kind = MUL;
					code2->u.binop.result = t2;
					code2->u.binop.op1 = t2;
					code2->u.binop.op2.kind = CONSTANT;
					code2->u.binop.op2.u.value = size[pos];
					//printf("%d\n",size[pos]);
					code2->last = NULL;
					code2->next = NULL;
					InterCodeInsert(code2);
			
					InterCode *code3 = (InterCode *)malloc(sizeof(InterCode));
					code3->kind = ADD;
					code3->u.binop.result = t1;
					code3->u.binop.op1 = t1;
					code3->u.binop.op2 = t2;
					code3->last = NULL;
					code3->next = NULL;
					InterCodeInsert(code3);
					if(n1->child[2] != NULL)
					{
						n2 = n1->child[2];
						n1 = n1->child[0];
					}
					else
					{
						n1 = n1->child[0];
					}
					
				}
		
				InterCode *code4 = (InterCode *)malloc(sizeof(InterCode));
				code4->kind = MUL;
				code4->u.binop.result = t1;
				code4->u.binop.op1 = t1;
				code4->u.binop.op2.kind = CONSTANT;
				code4->u.binop.op2.u.value = 4;
				code4->last = NULL;
				code4->next = NULL;
				InterCodeInsert(code4);
				
				temp_num ++;
				Operand t3;
				t3.kind = TEMP;
				t3.u.temp_no = temp_num;
				InterCode *code5 = (InterCode *)malloc(sizeof(InterCode));
				code5->kind = ADDR;
				code5->u.addr.result = t3;
				code5->u.addr.op1.kind = VARIABLE;
				code5->u.addr.op1.u.var_no = numberofvar(n1->ID);			
				code5->u.addr.op2 = t1;				
				code5->last = NULL;
				code5->next = NULL;
				InterCodeInsert(code5);
				
				temp_num ++;
				Operand t4;
				t4.kind = TEMP;
				t4.u.temp_no = temp_num;
				InterCode *code6 = translate_exp(exp2,&t4);
				InterCode *code7 = (InterCode *)malloc(sizeof(InterCode));
				code7->kind = MEMWRITE;
				code7->u.assign.left = t3;
				code7->u.assign.right = t4;
				code7->last = NULL;
				code7->next = NULL;
				InterCodeInsert(code7);
				if(op != NULL)
				{
					InterCode *code8 = (InterCode *)malloc(sizeof(InterCode));
					code8->kind = MEMREAD;
					code8->u.assign.left.kind = op->kind;
					code8->u.assign.left.u = op->u;
					code8->u.assign.right = t3;
					code8->last = NULL;
					code8->next = NULL;
					InterCodeInsert(code8);
				}
			}
			else 
			{
				printf("error!!\n");
				exit(0);
			}
		}
		else if((node->child[1]->type == eAND || node->child[1]->type == eOR 
				|| node->child[1]->type == eRELOP || node->child[0]->type == eNOT) && op != NULL)
		{
			label_num ++;
			int label1 = label_num;
			label_num ++;
			int label2 = label_num; 
			InterCode *code0 = (InterCode *)malloc(sizeof(InterCode));
			code0->kind = ASSIGN;
			code0->u.assign.left.kind = op->kind;
			code0->u.assign.left.u = op->u;
			code0->u.assign.right.kind = CONSTANT;
			code0->u.assign.right.u.value = 0;
			code0->last = NULL;
			code0->next = NULL;
			InterCodeInsert(code0);
			
			InterCode *code1 = translate_cond(node,label1,label2);
			InterCode *code2 = (InterCode *)malloc(sizeof(InterCode));
			code2->kind = LABEL;
			code2->u.label.label = label1;
			code2->last = NULL;
			code2->next = NULL;
			InterCodeInsert(code2);
		
			InterCode *code3 = (InterCode *)malloc(sizeof(InterCode));
			code3->kind = ASSIGN;
			code3->u.assign.left.kind = op->kind;
			code3->u.assign.left.u = op->u;
			code3->u.assign.right.kind = CONSTANT;
			code3->u.assign.right.u.value = 1;
			code3->last = NULL;
			code3->next = NULL;
			InterCodeInsert(code3);
		
			InterCode *code4 = (InterCode *)malloc(sizeof(InterCode));
			code4->kind = LABEL;
			code4->u.label.label = label2;
			code4->last = NULL;
			code4->next = NULL;
			InterCodeInsert(code4);
		}
		else if((node->child[1]->type == eADD || node->child[1]->type == eSUB ||
				node->child[1]->type == eMUL || node->child[1]->type == eDIV) && op != NULL)
		{
			temp_num ++;
			Operand t1;
			t1.kind = TEMP;
			t1.u.temp_no = temp_num;
			
			temp_num ++;
			Operand t2;
			t2.kind = TEMP;
			t2.u.temp_no = temp_num;
			InterCode *code1 = translate_exp(node->child[0],&t1);
			InterCode *code2 = translate_exp(node->child[2],&t2);

			InterCode *code3 = (InterCode *)malloc(sizeof(InterCode));
			switch(node->child[1]->type)
			{
				case eADD: 	code3->kind = ADD;	break;
				case eSUB:	code3->kind = SUB;	break;
				case eMUL:	code3->kind = MUL;	break;
				case eDIV:	code3->kind = DIV;	break;
				default:	printf("error!!\n");	break;
			}
			
			code3->u.binop.result.kind = op->kind;
			code3->u.binop.result.u = op->u;
			code3->u.binop.op1 = t1;
			code3->u.binop.op2 = t2;
			code3->last = NULL;
			code3->next = NULL;
			InterCodeInsert(code3);
		}
		else if(node->child[0]->type == eLP)
		{
			InterCode *node1 = translate_exp(node->child[1],op);
		}
		else if(node->child[0]->type == eSUB && op != NULL)
		{
			temp_num ++;
			Operand t1;
			t1.kind = TEMP;
			t1.u.temp_no = temp_num;
			
			InterCode *code1 = translate_exp(node->child[1],&t1);
			InterCode *code2 = (InterCode *)malloc(sizeof(InterCode));
			code2->kind = SUB;	
			code2->u.binop.result.kind = op->kind;
			code2->u.binop.result.u = op->u;
			code2->u.binop.op1.kind = CONSTANT;
			code2->u.binop.op1.u.value = 0;
			code2->u.binop.op2 = t1;
			code2->last = NULL;
			code2->next = NULL;
			InterCodeInsert(code2);
			
		}
		else if(node->child[0]->type == eID && node->child[3] != NULL)
		{
			int i = 0;
			for( ; i < 10 ; i ++)
				args[i] = NULL;
			InterCode *code1 = translate_args(node->child[2]);
			if(strcmp(node->child[0]->ID, "write") == 0)
			{
				InterCode *code2 = (InterCode *)malloc(sizeof(InterCode));
				code2->kind = WRITE;
				code2->u.arg.isarray = 0;
				code2->u.arg.result.kind = args[0]->kind;
				code2->u.arg.result.u = args[0]->u;
				code2->last = NULL;
				code2->next = NULL;
				InterCodeInsert(code2);
			}
			else
			{
				int i = 0;
				struct table_node *func = table_check(node->child[0]->ID,3);
				struct func_node * temp = func->func_next;
				int array[10];
				for(i = 0; i < 10 ; i ++)
				{
					if(temp != NULL)
					{
						if(temp->type == 2)
							array[i] = 1;
						else 
							array[i] = 0;
						temp = temp->next;
					}
					else
						array[i] = 0;
				}
				for(i = 9; i >= 0 ; i --)
				{
					if(args[i] != NULL)
					{
						
						InterCode *code = (InterCode *)malloc(sizeof(InterCode));
						code->kind = ARG;
						code->u.arg.isarray = 0;
						//code->u.arg.isarray =array[i];
						code->u.arg.result.kind = args[i]->kind;
						code->u.arg.result.u = args[i]->u;
						code->last = NULL;
						code->next = NULL;
						InterCodeInsert(code);
					}
				}
				for(i = 0 ; i < 10 ; i ++)
				{
					args[i] = NULL;
				}
				InterCode *code3 = (InterCode *)malloc(sizeof(InterCode));
				code3->kind = CALL;
				if(op != NULL)
				{	
					code3->u.callFunc.result.kind = op->kind;
					code3->u.callFunc.result.u = op->u;
				}
				else 
				{
					temp_num ++;
					Operand t;
					t.kind = TEMP;
					t.u.temp_no = temp_num;
					code3->u.callFunc.result = t;
				}
				strcpy(code3->u.callFunc.func,node->child[0]->ID);
				code3->last = NULL;
				code3->next = NULL;
				InterCodeInsert(code3);
			}
		}
		else if(node->child[0]->type == eID && node->child[3] == NULL)
		{
			if(strcmp(node->child[0]->ID,"read") == 0)
			{
				InterCode *code1 = (InterCode *)malloc(sizeof(InterCode));
				code1->kind = READ;
				code1->u.arg.isarray = 0;
				code1->u.arg.result.kind = op->kind;
				code1->u.arg.result.u = op->u;
				code1->last = NULL;
				code1->next = NULL;
				InterCodeInsert(code1);
			}
			else
			{
				InterCode *code1 = (InterCode *)malloc(sizeof(InterCode));
				code1->kind = CALL;
				if(op != NULL)
				{
					code1->u.callFunc.result.kind = op->kind;
					code1->u.callFunc.result.u = op->u;
				}
				else
				{
					temp_num ++;
					Operand t;
					t.kind = TEMP;
					t.u.temp_no = temp_num;
					code1->u.callFunc.result = t;
				}
				strcpy(code1->u.callFunc.func,node->child[0]->ID);
				code1->last = NULL;
				code1->next = NULL;
				InterCodeInsert(code1);
			}
		}
		else if(node->child[1]->type == eLB && op != NULL)
		{
			///////////////////////////////to finish
			int num = 0;
			int total = 0;
			struct Node *n1 = node->child[0];
			struct Node *n2 = node->child[2];
			struct Node *id;
			for(id = n1; id->type != eID; id = id->child[0])
				total ++;
			
			struct table_node *array = table_check(id->ID,1);
			int size[10];
			int i = 0;
			for( ; i < 10 ; i ++)
				size[i] = 0;
			struct array_node * arraysize = array->array_next;
			for(i = 0; arraysize != NULL ; arraysize = arraysize->next)
			{
				num ++;
				size[i] = arraysize->size;
				i ++;
			//	printf("%d\n",size[i]);
			}
			for(i = 0; size[i] != 0 ; i ++)
			{
				int j = i + 1;
				size[i] = 1;
				for( ; size[j] != 0 ; j ++)
				{
					size[i] = size[i] * size[j];
				}
			}
			
			temp_num ++;
			Operand t1;
			t1.kind = TEMP;
			t1.u.temp_no = temp_num;
			InterCode *code1 = (InterCode *)malloc(sizeof(InterCode));
			code1->kind = ASSIGN;
			code1->u.assign.left = t1;
			code1->u.assign.right.kind = CONSTANT;
			code1->u.assign.right.u.value = 0;
			code1->last = NULL;
			code1->next = NULL;
			InterCodeInsert(code1);
			
			int pos = total;
			while(n1->type != eID)
			{
				pos --;
				temp_num ++;
				Operand t2;
				t2.kind = TEMP;
				t2.u.temp_no = temp_num;
		
				InterCode *code1 = translate_exp(n2,&t2);
				InterCode *code2 = (InterCode *)malloc(sizeof(InterCode));
				code2->kind = MUL;
				code2->u.binop.result = t2;
				code2->u.binop.op1 = t2;
				code2->u.binop.op2.kind = CONSTANT;
				code2->u.binop.op2.u.value = size[pos];
				code2->last = NULL;
				code2->next = NULL;
				InterCodeInsert(code2);
		
				InterCode *code3 = (InterCode *)malloc(sizeof(InterCode));
				code3->kind = ADD;
				code3->u.binop.result = t1;
				code3->u.binop.op1 = t1;
				code3->u.binop.op2 = t2;
				code3->last = NULL;
				code3->next = NULL;
				InterCodeInsert(code3);
				if(n1->child[2] != NULL)
				{
					n2 = n1->child[2];
					n1 = n1->child[0];
				}
				else
				{
					n1 = n1->child[0];
				}
				
			}
			InterCode *code4 = (InterCode *)malloc(sizeof(InterCode));
			code4->kind = MUL;
			code4->u.binop.result = t1;
			code4->u.binop.op1 = t1;
			code4->u.binop.op2.kind = CONSTANT;
			code4->u.binop.op2.u.value = 4;
			code4->last = NULL;
			code4->next = NULL;
			InterCodeInsert(code4);
			/////
			temp_num ++;
			Operand t3;
			t3.kind = TEMP;
			t3.u.temp_no = temp_num;
			InterCode *code5 = (InterCode *)malloc(sizeof(InterCode));
			int number = numberofvar(n1->ID);
			//printf("%s %d\n",n1->ID,number);
			if(number > 0)
			{
				code5->kind = ADDR;
				code5->u.addr.op1.u.var_no = number;
			}
			else
			{
				code5->kind = ADD;
				code5->u.addr.op1.u.var_no = 0 - number;
			}
			code5->u.addr.result = t3;
			code5->u.addr.op1.kind = VARIABLE;	
			code5->u.addr.op2 = t1;				
			code5->last = NULL;
			code5->next = NULL;
			if(total != num)
			{
				code5->u.addr.result.kind = op->kind;
				code5->u.addr.result.u = op->u;
				InterCodeInsert(code5);
			}
			else
			{
				InterCodeInsert(code5);
				InterCode *code6 = (InterCode *)malloc(sizeof(InterCode));
				code6->kind = MEMREAD;
				code6->u.assign.left.kind = op->kind;
				code6->u.assign.left.u = op->u;
				code6->u.assign.right = t3;
				code6->last = NULL;
				code6->next = NULL;
				InterCodeInsert(code6);
			}
		}
		else
		{
			printf("error type!\n");
			exit(0);
		}
		
	}
	else if(node->child[0]->type == eID && op != NULL)
	{
		struct table_node * temp = table_check(node->child[0]->ID,1);
		InterCode *code1 = (InterCode *)malloc(sizeof(InterCode));
		int number = numberofvar(node->child[0]->ID);
		//printf("%s %d\n",node->child[0]->ID,number);
		if(temp->type == 2 && number > 0)
		{
			code1->kind = ADDR;
			code1->u.addr.result.kind = op->kind;
			code1->u.addr.result.u = op->u;
			code1->u.addr.op1.kind = VARIABLE;
			code1->u.addr.op1.u.var_no = number;		
			code1->u.addr.op2.kind = CONSTANT;
			code1->u.addr.op2.u.value = 0;	
		}
		else
		{
			code1->kind = ASSIGN;
			code1->u.assign.left.kind = op->kind;
			code1->u.assign.left.u = op->u;
			code1->u.assign.right.kind = VARIABLE;
			if(number > 0)
				code1->u.assign.right.u.var_no = number;
			else
				code1->u.assign.right.u.var_no = -number;
		}	
		code1->last = NULL;
		code1->next = NULL;
		InterCodeInsert(code1);
	}
	else if(node->child[0]->type == eINT && op != NULL)
	{
		InterCode *code1 = (InterCode *)malloc(sizeof(InterCode));
		code1->kind = ASSIGN;
		code1->u.assign.left.kind = op->kind;
		code1->u.assign.left.u = op->u;
		code1->u.assign.right.kind = CONSTANT;
		code1->u.assign.right.u.var_no = node->child[0]->intvalue;
		code1->last = NULL;
		code1->next = NULL;
		InterCodeInsert(code1);
	}
	else
	{
		printf("----error type!!!\n");
		exit(0);
	}
	return NULL;
}
InterCode *translate_args(struct Node *node)
{
	temp_num ++;
	Operand *t1 = (Operand *)malloc(sizeof(Operand));
	t1->kind = TEMP;
	t1->u.temp_no = temp_num;
	
	InterCode *code1 = translate_exp(node->child[0],t1);
	int i = 0;
	for(;i < 10 ; i ++)
	{
		if(args[i] == NULL)
		{
			args[i] = t1;
			break;
		}
	}
	if(node->child[2] != NULL)
	{
		InterCode *code2 = translate_args(node->child[2]);
	}
	return NULL;
}
InterCode *translate_stmt(struct Node *node)
{
	if(node->child[0]->type == Exp)
	{
		InterCode *n = translate_exp(node->child[0], NULL);
	}
	else if(node->child[0]->type == CompSt)
	{
		InterCode *n = translate_compst(node->child[0]);
	}
	else if(node->child[0]->type == eRETURN)
	{
		temp_num ++;
		Operand temp;
		temp.kind = TEMP;
		temp.u.temp_no = temp_num;
		InterCode *n1 = translate_exp(node->child[1], &temp);
		InterCode *n2 = (InterCode *)malloc(sizeof(InterCode));
		n2->kind = RETURN;
		n2->u.arg.isarray = 0;
		//n2->u.arg.result.kind = temp.kind;
		//n2->u.arg.result.u = temp.u;
		n2->u.arg.result = temp;		
		n2->last = NULL;
		n2->next = NULL;		
		InterCodeInsert(n2);
	}
	else if(node->child[0]->type == eIF && node->child[5] == NULL)
	{
		label_num ++;
		int label_true = label_num;
		label_num ++;
		int label_false = label_num; 
		InterCode *n = translate_cond(node->child[2], label_true, label_false);
		InterCode *n1 = (InterCode *)malloc(sizeof(InterCode));
		n1->kind = LABEL;
		n1->u.label.label = label_true;
		n1->last = NULL;
		n1->next = NULL;		
		InterCodeInsert(n1);
		n = translate_stmt(node->child[4]);
		InterCode *n2 = (InterCode *)malloc(sizeof(InterCode));
		n2->kind = LABEL;
		n2->u.label.label = label_false;
		n2->last = NULL;
		n2->next = NULL;		
		InterCodeInsert(n2);
		
	}
	else if(node->child[0]->type == eIF && node->child[5] != NULL)
	{
		label_num ++;
		int label1 = label_num;
		label_num ++;
		int label2 = label_num;
		label_num ++;
		int label3 = label_num;
		InterCode *code1 = translate_cond(node->child[2], label1, label2);
		InterCode *n1 = (InterCode *)malloc(sizeof(InterCode));
		n1->kind = LABEL;
		n1->u.label.label = label1;
		n1->last = NULL;
		n1->next = NULL;		
		InterCodeInsert(n1);
		
		InterCode *code2 = translate_stmt(node->child[4]);
		InterCode *n2 = (InterCode *)malloc(sizeof(InterCode));
		n2->kind = GOTO;
		n2->u.label.label = label3;
		n2->last = NULL;
		n2->next = NULL;		
		InterCodeInsert(n2);
	
		InterCode *n3 = (InterCode *)malloc(sizeof(InterCode));
		n3->kind = LABEL;
		n3->u.label.label = label2;
		n3->last = NULL;
		n3->next = NULL;		
		InterCodeInsert(n3);
		
		InterCode *code3 = translate_stmt(node->child[6]);
		InterCode *n4 = (InterCode *)malloc(sizeof(InterCode));
		n4->kind = LABEL;
		n4->u.label.label = label3;
		n4->last = NULL;
		n4->next = NULL;		
		InterCodeInsert(n4);
		
	}
	else if(node->child[0]->type == eWHILE)
	{
		label_num ++;
		int label1 = label_num;
		label_num ++;
		int label2 = label_num;
		label_num ++;
		int label3 = label_num;
		
		InterCode *n1 = (InterCode *)malloc(sizeof(InterCode));
		n1->kind = LABEL;
		n1->u.label.label = label1;
		n1->last = NULL;
		n1->next = NULL;		
		InterCodeInsert(n1);
		InterCode *code1 = translate_cond(node->child[2], label2, label3);
		
		InterCode *n2 = (InterCode *)malloc(sizeof(InterCode));
		n2->kind = LABEL;
		n2->u.label.label = label2;
		n2->last = NULL;
		n2->next = NULL;		
		InterCodeInsert(n2);
		
		InterCode *code2 = translate_stmt(node->child[4]);
		InterCode *n3 = (InterCode *)malloc(sizeof(InterCode));
		n3->kind = GOTO;
		n3->u.label.label = label1;
		n3->last = NULL;
		n3->next = NULL;		
		InterCodeInsert(n3);

		InterCode *n4 = (InterCode *)malloc(sizeof(InterCode));
		n4->kind = LABEL;
		n4->u.label.label = label3;
		n4->last = NULL;
		n4->next = NULL;		
		InterCodeInsert(n4);
	}
	else
	{
		printf("error!!\n");
		exit(0);
	}
	
	return NULL;
}
InterCode *translate_cond(struct Node *node, int label_true, int label_false)
{
	if(node->child[1] != NULL)
	{
		if(node->child[0]->type == Exp && node->child[1]->type == eRELOP)
		{
			temp_num ++;
			Operand t1;
			t1.kind = TEMP;
			t1.u.temp_no = temp_num;
			
			temp_num ++;
			Operand t2;
			t2.kind = TEMP;
			t2.u.temp_no = temp_num;
			
			InterCode *code1 = translate_exp(node->child[0], &t1);
			InterCode *code2 = translate_exp(node->child[2], &t2);
			
			InterCode *code3 = (InterCode *)malloc(sizeof(InterCode));
			code3->kind = IF;
			code3->u.ifstruct.left = t1;
			code3->u.ifstruct.right = t2;
			code3->u.ifstruct.relop = node->child[1]->iftype;
			code3->u.ifstruct.label = label_true;
			code3->last = NULL;
			code3->next = NULL;
			InterCodeInsert(code3);
			
			InterCode *n = (InterCode *)malloc(sizeof(InterCode));
			n->kind = GOTO;
			n->u.label.label = label_false;
			n->last = NULL;
			n->next = NULL;		
			InterCodeInsert(n);
			return NULL;
		}
		else if(node->child[0]->type == eNOT)
		{
			InterCode *code = translate_cond(node->child[1], label_false, label_true);
			return NULL;
		}
		else if(node->child[0]->type == Exp && node->child[1]->type == eAND)
		{
			label_num ++;
			int label1 = label_num;
			InterCode *code1 = translate_cond(node->child[0], label1, label_false);
			InterCode *n = (InterCode *)malloc(sizeof(InterCode));
			n->kind = LABEL;
			n->u.label.label = label1;
			n->last = NULL;
			n->next = NULL;		
			InterCodeInsert(n);
			InterCode *code2 = translate_cond(node->child[2], label_true, label_false);
			return NULL;
		}
		else if(node->child[0]->type == Exp && node->child[1]->type == eOR)
		{
			label_num ++;
			int label1 = label_num;
			InterCode *code1 = translate_cond(node->child[0], label_true, label1);
			InterCode *n = (InterCode *)malloc(sizeof(InterCode));
			n->kind = LABEL;
			n->u.label.label = label1;
			n->last = NULL;
			n->next = NULL;		
			InterCodeInsert(n);
			InterCode *code2 = translate_cond(node->child[2], label_true, label_false);
			return NULL;
		}
	}
	temp_num ++;
	Operand t1;
	t1.kind = TEMP;
	t1.u.temp_no = temp_num;
	
	InterCode *code1 = translate_exp(node,&t1);
	InterCode *code2 = (InterCode *)malloc(sizeof(InterCode));
	code2->kind = IF;
	code2->u.ifstruct.left = t1;
	code2->u.ifstruct.right.kind = CONSTANT;
	code2->u.ifstruct.right.u.value = 0;
	code2->u.ifstruct.relop = 3;
	code2->u.ifstruct.label = label_true;
	code2->last = NULL;
	code2->next = NULL;	
	InterCodeInsert(code2);

	InterCode *n = (InterCode *)malloc(sizeof(InterCode));
	n->kind = GOTO;
	n->u.label.label = label_false;
	n->last = NULL;
	n->next = NULL;		
	InterCodeInsert(n);
	return NULL;
}
InterCode *translate_compst(struct Node *node)
{
	struct Node *temp1 = node->child[1];
	struct Node *temp2 = node->child[2];
	for(; temp1->child[0] != NULL; temp1 = temp1->child[1])
	{
		InterCode *node = translate_def(temp1->child[0]);	
		//InterCodeInsert(node);
	}
	
	for(; temp2->child[0] != NULL; temp2 = temp2->child[1])
	{
		InterCode *node = translate_stmt(temp2->child[0]);
		//InterCodeInsert(node);
	}
	
	return NULL;
}
InterCode *translate_def(struct Node *node)
{
	if(node->child[0]->child[0]->type == StructSpecifier)
	{
		printf("Can not translate the code: Contain structure and function parameters of structure type!\n");
		exit(0);
	}
	
	struct Node * temp = node->child[1];
	
	for(; temp != NULL; temp = temp->child[2])	
	{
		struct Node *temp1 = temp->child[0];
		int size = 1;
		if(temp1->child[0]->child[0]->type == eID)
		{			
			if(temp1->child[1] != NULL)
			{
				Operand var;
				var.kind = VARIABLE;
				var.u.var_no = numberofvar(temp1->child[0]->child[0]->ID);
				InterCode *node = translate_exp(temp1->child[2], &var);
			}
			continue; 
		}
		for(temp1 = temp1->child[0]; temp1->child[0]->type != eID; temp1 = temp1->child[0])
		{
			size = size * temp1->child[2]->intvalue;
		}
		size = size * 4;
		InterCode *node = (InterCode *)malloc(sizeof(InterCode));
		node->kind = DEC;
		node->u.dec.size = size;
		node->u.dec.dec.kind = VARIABLE;
		node->u.dec.dec.u.var_no = numberofvar(temp1->child[0]->ID);
		node->last = NULL;
		node->next = NULL;
		InterCodeInsert(node);
	}
	return NULL;
}
void createintercode(struct Node *head)
{
	if(head != NULL)
	{
		
		if(head->type == ExtDef)
		{
			if(head->child[1]->type == ExtDecList)
			{
				printf("Can not translate the code: Contain structure and function parameters of structure type!\n");
				exit(0);
			}
			else if(head->child[1]->type == eSEMI)
			{
				printf("Can not translate the code: Contain structure and function parameters of structure type!\n");
				exit(0);
			}
			else if(head->child[1]->type == FunDec)
			{
				struct Node * func = head->child[1];
				struct Node * compst = head->child[2];
				InterCode *node = (InterCode *)malloc(sizeof(InterCode));
				node->kind = FUNCTION;
				strcpy(node->u.func.name,func->child[0]->ID);
				node->last = NULL;
				node->next = NULL;
				InterCodeInsert(node);
				if(func->child[2]->type == VarList)
				{
					struct Node *varlist = func->child[2];
					struct Node *temp;
					for(temp = varlist; temp != NULL ; temp = temp->child[2])
					{
						struct Node *temp1 = temp->child[0];
						temp1 = temp1->child[1];
						for(; temp1->type != eID ; temp1 = temp1->child[0]);
						InterCode *node = (InterCode *)malloc(sizeof(InterCode));
						node->kind = PARAM;
						node->u.arg.isarray = 0;
						node->u.arg.result.kind = VARIABLE;
						node->u.arg.result.u.var_no = numberofvar(temp1->ID);
						node->last = NULL;
						node->next = NULL;
						resetvar(temp1->ID);
						InterCodeInsert(node);
					}
				}
				InterCode *temp2 = translate_compst(compst);
				//InterCodeInsert(temp2);	
			}
			else
			{
				printf("syntax tree is error!!\n");
				exit(0);
			}
		}
		int i = 0;
		for(;i < CHILD_NUM ; i ++)
		{
			createintercode(head->child[i]);
		}
	}
}

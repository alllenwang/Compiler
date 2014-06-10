#include "intercode.h"
InterCode * InterCodeHead = NULL;
variable * variableHead = NULL;
void InterCodeInsert(InterCode * node)
{
	if(node == NULL)
		return;
	if(InterCodeHead == NULL)
	{
		InterCodeHead = node;
	}
	else
	{
		InterCode * temp = InterCodeHead;
		for(; temp->next != NULL; temp = temp->next);
		temp->next = node;
		node->last = temp;
	}
}
void InterCodePlus(InterCode * first , InterCode * node)
{
	InterCode * temp = first;
	if(first == NULL)
	{
		first = node;
	}
	else
	{
		for(; temp->next != NULL; temp = temp->next);
		temp->next = node;
		node->last = first;
	}
}
void deletenode(InterCode * node)
{
	if(node->next == NULL)
	{
		node->last->next = NULL;
		free(node);
	}
	else if(node->last == NULL)
	{
		node->next->last = NULL;
		free(node);
	}
	else
	{
		node->last->next = node->next;
		node->next->last = node->last;
		free(node);
	}
}
int numberofvar(char *name)
{
	variable * temp = variableHead;
	for(; temp!= NULL ; temp = temp->next)
	{
		if(strcmp(temp->name,name) == 0)
			return temp->number;
	}
	return -1;
}
void resetvar(char *name)
{
	variable * temp = variableHead;
	for(; temp!= NULL ; temp = temp->next)
	{
		if(strcmp(temp->name,name) == 0)
			temp->number = 0 - temp->number;
	}
}
void printoperand(FILE* wf, Operand a)
{
	switch(a.kind)
	{
		case VARIABLE:
			fprintf(wf,"v%d",a.u.var_no);
			break;
		case CONSTANT:
			fprintf(wf,"#%d",a.u.value);
			break;
		case TEMP:
			fprintf(wf,"t%d",a.u.temp_no);
			break;
		default:
			fprintf(wf,"error\n");
			break;
	}
}
void printcode(FILE* wf)
{
	InterCode * temp = InterCodeHead;
	for(; temp != NULL ; temp = temp->next)
	{
		switch(temp->kind)
		{
			case ASSIGN:
				printoperand(wf,temp->u.assign.left);
				fprintf(wf," := ");
				printoperand(wf,temp->u.assign.right);
				fprintf(wf,"\n");
				break;
			case ADDR:
				printoperand(wf,temp->u.addr.result);
				fprintf(wf," := ");
				fprintf(wf,"&");
				printoperand(wf,temp->u.addr.op1);
				fprintf(wf," + ");
				printoperand(wf,temp->u.addr.op2);
				fprintf(wf,"\n");
				break;
			case MEMREAD:
				printoperand(wf,temp->u.assign.left);
				fprintf(wf," := ");
				fprintf(wf,"*");
				printoperand(wf,temp->u.assign.right);
				fprintf(wf,"\n");
				break;
			case MEMWRITE:
				fprintf(wf,"*");
				printoperand(wf,temp->u.assign.left);
				fprintf(wf," := ");
				printoperand(wf,temp->u.assign.right);
				fprintf(wf,"\n");
				break;
			case ADD:
				printoperand(wf,temp->u.addr.result);
				fprintf(wf," := ");
				printoperand(wf,temp->u.addr.op1);
				fprintf(wf," + ");
				printoperand(wf,temp->u.addr.op2);
				fprintf(wf,"\n");
				break;
			case SUB:
				printoperand(wf,temp->u.addr.result);
				fprintf(wf," := ");
				printoperand(wf,temp->u.addr.op1);
				fprintf(wf," - ");
				printoperand(wf,temp->u.addr.op2);
				fprintf(wf,"\n");
				break;
			case MUL:
				printoperand(wf,temp->u.addr.result);
				fprintf(wf," := ");
				printoperand(wf,temp->u.addr.op1);
				fprintf(wf," * ");
				printoperand(wf,temp->u.addr.op2);
				fprintf(wf,"\n");
				break;
			case DIV:
				printoperand(wf,temp->u.addr.result);
				fprintf(wf," := ");
				printoperand(wf,temp->u.addr.op1);
				fprintf(wf," / ");
				printoperand(wf,temp->u.addr.op2);
				fprintf(wf,"\n");
				break;
			case LABEL:
				fprintf(wf,"LABEL label%d :\n",temp->u.label.label);
				break;
			case FUNCTION:
				fprintf(wf,"FUNCTION %s :\n",temp->u.func.name);
				break;
		 	case GOTO:
				fprintf(wf,"GOTO label%d\n",temp->u.label.label);
				break;
			case IF:
				fprintf(wf,"IF ");
				printoperand(wf,temp->u.ifstruct.left);
				switch(temp->u.ifstruct.relop)
				{
					case 3:		fprintf(wf," != ");		break;
					case 4:		fprintf(wf," == ");		break;
					case 5:		fprintf(wf," < ");		break;
					case 6:		fprintf(wf," > ");		break;
					case 7:		fprintf(wf," >= ");		break;
					case 8:		fprintf(wf," <= ");		break;
					default:	fprintf(wf,"error\n");	break;
				}
				printoperand(wf,temp->u.ifstruct.right);
				fprintf(wf," GOTO label%d\n",temp->u.ifstruct.label);
				break;
			case RETURN:
				fprintf(wf,"RETURN ");
				printoperand(wf,temp->u.arg.result);
				fprintf(wf,"\n");
				break;
			case DEC:
				fprintf(wf,"DEC ");
				printoperand(wf,temp->u.dec.dec);
				fprintf(wf," %d\n",temp->u.dec.size);
				break;
			case ARG:
				fprintf(wf,"ARG ");
				if(temp->u.arg.isarray == 1)
					fprintf(wf,"&");
				printoperand(wf,temp->u.arg.result);
				fprintf(wf,"\n");
				break;
			case CALL:
				printoperand(wf,temp->u.callFunc.result);
				fprintf(wf," := ");
				fprintf(wf,"CALL %s\n",temp->u.callFunc.func);
				
				break;
			case PARAM:
				fprintf(wf,"PARAM ");
				printoperand(wf,temp->u.arg.result);
				fprintf(wf,"\n");
				break;
			case READ:
				fprintf(wf,"READ ");
				printoperand(wf,temp->u.arg.result);
				fprintf(wf,"\n");
				break;
			case WRITE:
				fprintf(wf,"WRITE ");
				printoperand(wf,temp->u.arg.result);
				fprintf(wf,"\n");
				break;
			default:
				printf("error\n");
				break;
		}
		
	}
}
void optimize()
{
	InterCode * temp1 = InterCodeHead;
	InterCode * temp2 = temp1->next;
	int num = 0;
	while(temp1 != NULL && temp2 != NULL)
	{
		if((temp1->kind == ADD ||temp1->kind == SUB || temp1->kind == MUL || temp1->kind == DIV) && 
			 (temp2->kind == ADD ||temp2->kind == SUB || temp2->kind == MUL || temp2->kind == DIV || temp2->kind == ADDR))
		{
		
			if(temp1->u.binop.result.kind == temp2->u.binop.op1.kind && temp1->u.binop.result.u.value == temp2->u.binop.op1.u.value && temp2->kind != ADDR)
			{
				if(temp1->kind == ADD)
				{
					if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op2.kind == CONSTANT)
					{
						int result = temp1->u.binop.op1.u.value + temp1->u.binop.op2.u.value;
						temp2->u.binop.op1.kind = CONSTANT;
						temp2->u.binop.op1.u.value = result;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op1.u.value == 0 && temp1->u.binop.result.kind != VARIABLE)
					{
						temp2->u.binop.op1.kind = temp1->u.binop.op2.kind;
						temp2->u.binop.op1.u = temp1->u.binop.op2.u;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else if(temp1->u.binop.op2.kind == CONSTANT && temp1->u.binop.op2.u.value == 0 && temp1->u.binop.result.kind != VARIABLE)
					{
						temp2->u.binop.op1.kind = temp1->u.binop.op1.kind;
						temp2->u.binop.op1.u = temp1->u.binop.op1.u;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else
					{
						temp1 = temp1->next;
						temp2 = temp2->next;
					}
				}
				else if(temp1->kind == SUB)
				{
					if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op2.kind == CONSTANT)
					{
						int result = temp1->u.binop.op1.u.value - temp1->u.binop.op2.u.value;
						temp2->u.binop.op1.kind = CONSTANT;
						temp2->u.binop.op1.u.value = result;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else if(temp1->u.binop.op2.kind == CONSTANT && temp1->u.binop.op2.u.value == 0 && temp1->u.binop.result.kind != VARIABLE)
					{
						temp2->u.binop.op1.kind = temp1->u.binop.op1.kind;
						temp2->u.binop.op1.u = temp1->u.binop.op1.u;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else
					{
						temp1 = temp1->next;
						temp2 = temp2->next;
					}
				}
				else if(temp1->kind == MUL)
				{
					if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op2.kind == CONSTANT)
					{
						int result = temp1->u.binop.op1.u.value * temp1->u.binop.op2.u.value;
						temp2->u.binop.op1.kind = CONSTANT;
						temp2->u.binop.op1.u.value = result;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op1.u.value == 0 && temp1->u.binop.result.kind != VARIABLE)
					{
						temp2->u.binop.op1.kind = CONSTANT;
						temp2->u.binop.op1.u.value = 0;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else if(temp1->u.binop.op2.kind == CONSTANT && temp1->u.binop.op2.u.value == 0 && temp1->u.binop.result.kind != VARIABLE)
					{
						temp2->u.binop.op1.kind = CONSTANT;
						temp2->u.binop.op1.u.value = 0;
						InterCode * temp = temp1;

						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else
					{
						temp1 = temp1->next;
						temp2 = temp2->next;
					}
				}
				else if(temp1->kind == DIV)
				{
					if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op2.kind == CONSTANT)
					{
						int result = temp1->u.binop.op1.u.value / temp1->u.binop.op2.u.value;
						temp2->u.binop.op1.kind = CONSTANT;
						temp2->u.binop.op1.u.value = result;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op1.u.value == 0 && temp1->u.binop.result.kind != VARIABLE)
					{
						temp2->u.binop.op1.kind = CONSTANT;
						temp2->u.binop.op1.u.value = 0;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else
					{
						temp1 = temp1->next;
						temp2 = temp2->next;
					}
				}
				else 
				{
					temp1 = temp1->next;
					temp2 = temp2->next;
				}
			}
			else if(temp1->u.binop.result.kind == temp2->u.binop.op2.kind && temp1->u.binop.result.u.value == temp2->u.binop.op2.u.value )
			{
				if(temp1->kind == ADD)
				{
					if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op2.kind == CONSTANT)
					{
						int result = temp1->u.binop.op1.u.value + temp1->u.binop.op2.u.value;
						temp2->u.binop.op2.kind = CONSTANT;
						temp2->u.binop.op2.u.value = result;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op1.u.value == 0 && temp1->u.binop.result.kind != VARIABLE)
					{
						temp2->u.binop.op2.kind = temp1->u.binop.op2.kind;
						temp2->u.binop.op2.u = temp1->u.binop.op2.u;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else if(temp1->u.binop.op2.kind == CONSTANT && temp1->u.binop.op2.u.value == 0 && temp1->u.binop.result.kind != VARIABLE)
					{
						temp2->u.binop.op2.kind = temp1->u.binop.op1.kind;
						temp2->u.binop.op2.u = temp1->u.binop.op1.u;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else
					{
						temp1 = temp1->next;
						temp2 = temp2->next;
					}
				}
				else if(temp1->kind == SUB)
				{
					if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op2.kind == CONSTANT && temp1->u.binop.result.kind != VARIABLE)
					{
						int result = temp1->u.binop.op1.u.value - temp1->u.binop.op2.u.value;
						temp2->u.binop.op2.kind = CONSTANT;
						temp2->u.binop.op2.u.value = result;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else if(temp1->u.binop.op2.kind == CONSTANT && temp1->u.binop.op2.u.value == 0 && temp1->u.binop.result.kind != VARIABLE)
					{
						temp2->u.binop.op2.kind = temp1->u.binop.op1.kind;
						temp2->u.binop.op2.u = temp1->u.binop.op1.u;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else
					{
						temp1 = temp1->next;
						temp2 = temp2->next;
					}
				}
				else if(temp1->kind == MUL)
				{
					if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op2.kind == CONSTANT)
					{
						int result = temp1->u.binop.op1.u.value * temp1->u.binop.op2.u.value;
						temp2->u.binop.op2.kind = CONSTANT;
						temp2->u.binop.op2.u.value = result;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op1.u.value == 0 && temp1->u.binop.result.kind != VARIABLE)
					{
						temp2->u.binop.op2.kind = CONSTANT;
						temp2->u.binop.op2.u.value = 0;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else if(temp1->u.binop.op2.kind == CONSTANT && temp1->u.binop.op2.u.value == 0 && temp1->u.binop.result.kind != VARIABLE)
					{
						temp2->u.binop.op2.kind = CONSTANT;
						temp2->u.binop.op2.u.value = 0;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else
					{
						temp1 = temp1->next;
						temp2 = temp2->next;
					}
				}
				else if(temp1->kind == DIV)
				{
					if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op2.kind == CONSTANT)
					{
						int result = temp1->u.binop.op1.u.value / temp1->u.binop.op2.u.value;
						temp2->u.binop.op2.kind = CONSTANT;
						temp2->u.binop.op2.u.value = result;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else if(temp1->u.binop.op1.kind == CONSTANT && temp1->u.binop.op1.u.value == 0 && temp1->u.binop.result.kind != VARIABLE)
					{
						temp2->u.binop.op2.kind = CONSTANT;
						temp2->u.binop.op2.u.value = 0;
						InterCode * temp = temp1;
						temp1 = temp1->next;
						temp2 = temp2->next;
						deletenode(temp);
						num ++;
						continue;
					}
					else
					{
						temp1 = temp1->next;
						temp2 = temp2->next;
					}
				}
				else 
				{
					temp1 = temp1->next;
					temp2 = temp2->next;
				}
			}
		/*	else if(temp1->u.binop.result.kind == CONSTANT)
			{
				InterCode * temp = temp1;
				temp1 = temp1->next;
				temp2 = temp2->next;
				deletenode(temp);
				num ++;
				continue;
			}
		*/	else
			{
				temp1 = temp1->next;
				temp2 = temp2->next;
			}
		}
		else
		{
			temp1 = temp1->next;
			temp2 = temp2->next;
		}
	}
	if(num > 0)
		optimize();
}

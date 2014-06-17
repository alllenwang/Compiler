#include "assembler.h"
#include "../table/table.h"
#include "../tree/tree.h"
#include "../intercode/intercode.h"
#include "../intercode/createintercode.h"
Param * param_head = NULL;
Var * var_head = NULL;
int param_total_num = 0;
int var_total_num = 0;
void get_temp(int num,Operand a,FILE *wf)
{
	if(a.kind == CONSTANT)
	{
		fprintf(wf,"\tli $t%d, %d\n",num, a.u.value);
	}
	else
	{
		int op1 = param_num(a);
		if(op1 < 0)
		{
			op1 = var_num(a);
			if(op1 < 0 ) printf("error!!!\n");
			fprintf(wf,"\tlw $t%d, %d($fp)\n",num, op1);
		}
		else
		{
			fprintf(wf,"\tlw $t%d, %d($sp)\n",num, op1 * 4);
		}
	}
}
void send_temp(int num,Operand a,FILE *wf)
{
	int result = param_num(a);
	if(result < 0)
	{
		result = var_num(a);
		if(result < 0 ) 
		{
			//var_add(4);
			var_total_num = var_total_num + 4;
			insert_var(a,var_total_num);
			fprintf(wf,"\tsw $t%d, %d($fp)\n",num, var_total_num);
		}
		else						
			fprintf(wf,"\tsw $t%d, %d($fp)\n",num, result);
	}
	else
	{
		fprintf(wf,"\tlw $t%d, %d($sp)\n",num , result * 4);
	}
}
void insert_param(Operand a, int num)
{
	Param *temp = (Param *)malloc(sizeof(Param));
	temp->a = a;
	temp->num = num;
	temp->next = param_head;
	param_head = temp;
}
void insert_var(Operand a, int num)
{
	Var *temp = (Var *)malloc(sizeof(Var));
	temp->a = a;
	temp->num = num;
	temp->next = var_head;
	var_head = temp;
}
void var_add(int num)
{
	Var *temp = var_head;
	for(; temp != NULL; temp = temp->next)
	{
		temp->num = temp->num + num;
	}
}
void param_add(int num)
{
	Param *temp =  param_head;
	for(; temp != NULL; temp = temp->next)
	{
		temp->num = temp->num + num;
	}
}
int param_num(Operand a)
{
	Param *temp =  param_head;
	for(; temp != NULL; temp = temp->next)
	{
		if(temp->a.kind == a.kind && temp->a.u.value == a.u.value)
			return temp->num;
	}
	return -1;
}
int var_num(Operand a)
{
	Var *temp =  var_head;
	for(; temp != NULL; temp = temp->next)
	{
		if(temp->a.kind == a.kind && temp->a.u.value == a.u.value)
			return temp->num;
	}
	return -1;
}
void create_code(FILE *wf)
{
	fprintf(wf, ".data\n");
	fprintf(wf, "_prompt: .asciiz \"Enter an integer:\"\n");
	fprintf(wf, "_ret: .asciiz \"\\n\"\n");
	fprintf(wf, ".globl main\n");
	fprintf(wf, ".text\n");
	fprintf(wf, "read:\n");
	fprintf(wf, "\tli $v0, 4\n");
	fprintf(wf, "\tla $a0, _prompt\n");
	fprintf(wf, "\tsyscall\n");
	fprintf(wf, "\tli $v0, 5\n");
	fprintf(wf, "\tsyscall\n");
	fprintf(wf, "\tjr $ra\n");
	fprintf(wf, "\n");
	fprintf(wf, "write:\n");
	fprintf(wf, "\tli $v0, 1\n");
	fprintf(wf, "\tsyscall\n");
	fprintf(wf, "\tli $v0, 4\n");
	fprintf(wf, "\tla $a0, _ret\n");
	fprintf(wf, "\tsyscall\n");
	fprintf(wf, "\tmove $v0, $0\n");
	fprintf(wf, "\tjr $ra\n");

	InterCode * temp = InterCodeHead;
	int num = 1;
	int result = 0;
	int op1 = 0;
	int op2 = 0;
	int arg_num = 0;
	InterCode * argpos = NULL;
	int i = 0;
	for(; temp != NULL ; temp = temp->next)
	{
		switch(temp->kind)
		{
			case ASSIGN:	
				get_temp(0, temp->u.assign.right, wf);
				//fprintf(wf, "\tmove $t1, $t0");
				send_temp(0, temp->u.assign.left, wf);
				break;
			case ADDR:
				
				break;
			case MEMREAD:
				
				break;
			case MEMWRITE:
				
				break;
			case ADD:
				get_temp(0, temp->u.addr.op1, wf);
				get_temp(1, temp->u.addr.op2, wf);
				fprintf(wf, "\tadd $t2, $t1, $t0\n");
				send_temp(2, temp->u.addr.result, wf);
				break;
			case SUB:
				get_temp(0, temp->u.addr.op1, wf);
				get_temp(1, temp->u.addr.op2, wf);
				fprintf(wf, "\tsub $t2, $t1, $t0\n");
				send_temp(2, temp->u.addr.result, wf);
				break;
			case MUL:
				get_temp(0, temp->u.addr.op1, wf);
				get_temp(1, temp->u.addr.op2, wf);
				fprintf(wf, "\tmul $t2, $t1, $t0\n");
				send_temp(2, temp->u.addr.result, wf);
				break;
			case DIV:
				get_temp(0, temp->u.addr.op1, wf);
				get_temp(1, temp->u.addr.op2, wf);
				fprintf(wf, "\tdiv $t2, $t1, $t0\n");
				send_temp(2, temp->u.addr.result, wf);
				break;
			case LABEL:
				fprintf(wf,"label%d :\n",temp->u.label.label);
				break;
			case FUNCTION:
				fprintf(wf,"%s:\n",temp->u.func.name);
				if(strcmp(temp->u.func.name,"main") == 0)
				{
					fprintf(wf,"\tmove $fp, $sp\n");
					fprintf(wf,"\taddi $sp, $sp, -2048\n");					
					fprintf(wf, "\tli $t0, 8\n");
					fprintf(wf,"\tsw $t0, 4($sp)\n");
					
					param_total_num = 0;
				}	
				break;
		 	case GOTO:
				fprintf(wf,"\tj label%d\n",temp->u.label.label);
				break;
			case IF:
				get_temp(0, temp->u.ifstruct.left, wf);
				get_temp(1, temp->u.ifstruct.right, wf);
				switch(temp->u.ifstruct.relop)
				{
					
					case 3:		fprintf(wf,"\tbne ");		break;
					case 4:		fprintf(wf,"\tbeq ");		break;
					case 5:		fprintf(wf,"\tblt ");		break;
					case 6:		fprintf(wf,"\tbgt ");		break;
					case 7:		fprintf(wf,"\tbge ");		break;
					case 8:		fprintf(wf,"\tble ");		break;
					default:	fprintf(stdout,"error\n");		break;
				}
				fprintf(wf,"$t0, $t1, label%d\n", temp->u.ifstruct.label);
				break;
			case RETURN:
				get_temp(0, temp->u.arg.result, wf);
				fprintf(wf, "\tmove $v0, $t0\n");
				fprintf(wf, "\tjr $ra\n");
				break;
			case DEC:
				var_total_num = var_total_num + 4;
				insert_var(temp->u.dec.dec,var_total_num);				
				var_total_num = var_total_num + temp->u.dec.size;
				
				break;
			case ARG:
				fprintf(wf,"\tsw $ra, 0($sp)\n");
				fprintf(wf,"\taddi $sp, $sp, %d\n",(param_total_num + 2) * 4);
				arg_num = param_total_num;
				argpos = temp;
				param_total_num = 1;
				for( ; argpos->next->kind == ARG; argpos = argpos->next)
				{
					param_total_num ++;
				}
				i = param_total_num;
			
				
				for( ; temp->next->kind == ARG; temp = temp->next)
				{
					get_temp(0, temp->u.arg.result, wf);
					fprintf(wf,"\tsw $t0, %d($sp)\n", i * 4);
					i --;
				}
				get_temp(0, temp->u.arg.result, wf);
				fprintf(wf,"\tsw $t0, %d($sp)\n", i * 4);
				
				fprintf(wf,"\tli $t0, %d\n", (param_total_num + 2) * 4);
				fprintf(wf,"\tsw $t0, %d($sp)\n", (param_total_num + 1) * 4);
				temp = temp->next;
				
				fprintf(wf, "\tjal %s\n",temp->u.callFunc.func);
				fprintf(wf, "\tlw $t0, -4($sp)\n");
				fprintf(wf,"\tsub $sp, $fp, $t0\n");
				fprintf(wf, "\tlw $ra, 0($sp)\n");
				fprintf(wf, "\tmove $t0, $v0\n");
				send_temp(0, temp->u.callFunc.result,wf);
				arg_num = param_total_num;
				
				
				
				break;
			case CALL:
				//printf("%d",num);
				fprintf(wf,"\tsw $ra, 0($sp)\n");
				fprintf(wf, "\tli $t0, 8\n");
				fprintf(wf,"\tsw $t0, 4($sp)\n");
				fprintf(wf,"\tadd $sp, $sp, $t0\n");
			
				fprintf(wf, "\tjal %s\n",temp->u.callFunc.func);
				
				fprintf(wf, "\tlw $t0, -4($sp)\n");
				fprintf(wf,"\tsub $sp, $sp, $t0\n");
				fprintf(wf, "\tlw $ra, 0($sp)\n");
				fprintf(wf, "\tmove $t0, $v0\n");
				send_temp(0, temp->u.callFunc.result,wf);
				
				break;
			case PARAM:
				num = 1;
				for( ; temp->next->kind == PARAM; temp = temp->next)
				{
					insert_param(temp->u.arg.result, num);
					num ++;
				}
				insert_param(temp->u.arg.result, num);
				num ++;
				break;
			case READ:
				fprintf(wf,"\tsw $ra, 0($sp)\n");
				fprintf(wf,"\taddi $sp, $sp, 4\n");
				fprintf(wf, "\tjal read\n");
				
				fprintf(wf,"\tsub $sp, $sp, 4\n");
				fprintf(wf, "\tlw $ra, 0($sp)\n");
				fprintf(wf, "\tmove $t0, $v0\n");
				send_temp(0, temp->u.arg.result,wf);
				break;
			case WRITE:
				fprintf(wf,"\tsw $ra, 0($sp)\n");
				fprintf(wf,"\taddi $sp, $sp, 4\n");
				get_temp(0, temp->u.arg.result, wf);
				fprintf(wf,"\tmove $a0, $t0\n");
				fprintf(wf, "\tjal write\n");
				fprintf(wf,"\taddi $sp, $sp, -4\n");
				fprintf(wf, "\tlw $ra, 0($sp)\n");
				break;
			default:
				
				break;
		}
	}
}

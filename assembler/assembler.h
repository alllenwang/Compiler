#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../intercode/intercode.h"
typedef struct Param Param;
struct Param
{
	Operand a;
	int num;
	Param *next;
		
};
typedef struct Var Var;
struct Var
{
	Operand a;
	int num;
	Var *next;
};

extern int param_total_num;
extern Param * param_head;
extern Var * var_head;
void get_temp(int,Operand,FILE *wf);
void send_temp(int,Operand,FILE *wf);
void insert_param(Operand, int);
void insert_var(Operand, int);
void var_add(int);
void param_add(int);
int param_num(Operand);
int var_num(Operand);
void create_code(FILE *wf);


#endif

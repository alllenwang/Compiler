#ifndef __INTERCODE_H__
#define __INTERCODE_H__
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef struct Operand Operand;
struct Operand
{
	enum { VARIABLE, CONSTANT, TEMP} kind;
	union { int var_no; int value; int temp_no;} u;
}; 
typedef struct InterCode InterCode;
struct InterCode
{
	enum {ASSIGN, ADDR, MEMREAD , MEMWRITE, ADD, SUB, MUL, DIV, LABEL, FUNCTION,
		 GOTO, IF, RETURN, DEC, ARG, CALL, PARAM, READ, WRITE} kind;
	union 
	{
		struct { Operand dec; int size;} dec;			//DEC
		struct { char name[32];} func;				//FUNCTION
		struct { int label;} label;				//LABEL GOTO 
		struct { Operand result; char func[32];} callFunc;	//CALL
		struct { Operand result, op1, op2;} addr;		//ADDR
		struct { int isarray; Operand result;} arg;		//ARG PARAM READ WRITE RETURN
		struct { Operand left, right; int relop; int label;} ifstruct;//IF
		struct { Operand left, right;} assign;			//ASSIGN MEMREAD MEMWRITE
		struct { Operand result, op1, op2;} binop;		//ADD SUB MUL DIV
	} u;
	InterCode *last;
	InterCode *next;
}; 
typedef struct variable variable;
struct variable 
{
	char name[32];
	int number;
	variable *next;
};
extern variable * variableHead;
extern InterCode * InterCodeHead;
void InterCodeInsert(InterCode * node);
int numberofvar(char *name);
void InterCodePlus(InterCode * first , InterCode * node);
void printcode(FILE* wf);
void printoperand(FILE* wf, Operand a);
void resetvar(char *name);
void optimize();
void deletenode(InterCode * node);







#endif

#ifndef __TREE_H__
#define __TREE_H__
#include <stdbool.h>
#include <stdlib.h>
#define CHILD_NUM 10
enum Type
{ 	eINT = 1 , eFLOAT , eID , eADD, eSUB , 
	eMUL , eDIV , eAND , eOR , eASSIGNOP ,
	eCOMMA , eNE , eEQ , eLT , eGT ,
	eGE , eLE , eSEMI , eDOT , eNOT ,
	eLP , eRP , eLB , eRB , eLC ,
	eRC , eSTRUCT , eRETURN , eIF , eELSE , 
	eTHEN , eWHILE , eWHEN , eFOR ,	eTYPE , 
	eRELOP , Program , ExtDefList ,  ExtDef , Specifier ,
	ExtDecList , FunDec , CompSt , VarDec , StructSpecifier ,
	OptTag , Tag , VarList , ParamDec , StmtList ,
	Stmt , DefList , Def , DecList , Dec ,
	Exp , Args
};
struct Node
{
	bool isempty;// 
	enum Type type;
	int line;
	char name[20];
	int iftype;	// 1: type->int 2: type->float 
			// 3: !=	4: ==
			// 5: <		6: >
			// 7: >=	8: <=
			// 0: ternimal symbol
			//-1: nonternimal
	char ID[64];	
	int intvalue;
	float floatvalue;
	struct Node * father;
	struct Node * child[CHILD_NUM];	
};
extern struct Node * head;
extern bool print;
void insert(struct Node *father,struct Node *child);
void display(struct Node *node,int);
struct Node * build_node(enum Type, int, char *name, int, char *ID, int, float);

#endif









%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <stdbool.h>
	#include "lex.yy.c"
	
%}
%union{
	struct Node * type_node;
}

%token <type_node>INT
%token <type_node>FLOAT
%token <type_node>OCTONARY
%token <type_node>HEXADECIMAL
%token <type_node>ID
%type  <type_node>Program ExtDefList  ExtDef Specifier ExtDecList FunDec 
%type  <type_node>CompSt VarDec StructSpecifier OptTag Tag VarList ParamDec 
%type  <type_node>StmtList Stmt DefList Def DecList Dec Exp Args
%token <type_node>ADD SUB MUL DIV
%token <type_node>AND OR ASSIGNOP COMMA
%token <type_node>NE EQ LT GT GE LE 
%token <type_node>SEMI DOT NOT
%token <type_node>LP RP LB RB LC RC
%token <type_node>STRUCT RETURN IF ELSE THEN WHILE WHEN FOR
%token <type_node>TYPE 
%token <type_node>RELOP
%token <type_node>error

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%right	ASSIGNOP
%left	OR
%left	AND
%left	NE	EQ	GE	LE	LT	GT
%left	ADD	SUB
%left	MUL	DIV
%right	NOT	
%left 	LP	RP	DOT	LB	RB
%%

Program		:	ExtDefList	
{	$$ = build_node(Program,@$.first_line,"Program",-1,"",-1,-1.0);
	insert($$,$1); head = $$;}
		;
					
ExtDefList	:	ExtDef ExtDefList	
{	$$ = build_node(ExtDefList,@$.first_line,"ExtDefList",-1,"",-1,-1.0);
	insert($$,$1);	insert($$,$2);}	
		|	/*empty*/	
{$$ = build_node(ExtDefList,@$.first_line,"ExtDefList",-1,"",-1,-1.0);
	$$->isempty = true;}
		;
ExtDef		:	Specifier ExtDecList SEMI
{	$$ = build_node(ExtDef,@$.first_line,"ExtDef",-1,"",-1,-1.0);
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	Specifier SEMI	
{	$$ = build_node(ExtDef,@$.first_line,"ExtDef",-1,"",-1,-1.0);
	insert($$,$1);	insert($$,$2);	}
		|	Specifier FunDec CompSt	
{	$$ = build_node(ExtDef,@$.first_line,"ExtDef",-1,"",-1,-1.0);
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	error SEMI
		;	
ExtDecList	:	VarDec		
{	$$ = build_node(ExtDecList,@$.first_line,"ExtDecList",-1,"",-1,-1.0);	
	insert($$,$1);}
		|	VarDec COMMA ExtDecList	
{	$$ = build_node(ExtDecList,@$.first_line,"ExtDecList",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		;
Specifier	:	TYPE
{	$$ = build_node(Specifier,@$.first_line,"Specifier",-1,"",-1,-1.0);	
	insert($$,$1);}
		|	StructSpecifier
{	$$ = build_node(Specifier,@$.first_line,"Specifier",-1,"",-1,-1.0);	
	insert($$,$1);}
		;
StructSpecifier	:	STRUCT OptTag LC DefList RC
{$$ = build_node(StructSpecifier,@$.first_line,"StructSpecifier",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);	insert($$,$4);
	insert($$,$5);}
		|	STRUCT Tag
{$$ = build_node(StructSpecifier,@$.first_line,"StructSpecifier",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);}
		;
OptTag		:	ID	
{	$$ = build_node(OptTag,@$.first_line,"OptTag",-1,"",-1,-1.0);	
	insert($$,$1);}
		|	/*empty*/	
{	$$ = build_node(OptTag,@$.first_line,"OptTag",-1,"",-1,-1.0);
	$$->isempty = true;}
		;
Tag		:	ID 
{	$$ = build_node(Tag,@$.first_line,"Tag",-1,"",-1,-1.0);	
	insert($$,$1);}
		;
VarDec		:	ID	
{	$$ = build_node(VarDec,@$.first_line,"VarDec",-1,"",-1,-1.0);	
	insert($$,$1);}
		|	VarDec LB INT RB
{	$$ = build_node(VarDec,@$.first_line,"VarDec",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);	insert($$,$4);}
		|	error SEMI
		;
FunDec		:	ID LP VarList RP
{	$$ = build_node(FunDec,@$.first_line,"FunDec",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);	insert($$,$4);}
		|	ID LP RP
{	$$ = build_node(FunDec,@$.first_line,"FunDec",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	error RP
		;
VarList		:	ParamDec COMMA VarList
{	$$ = build_node(VarList,@$.first_line,"VarList",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	ParamDec
{	$$ = build_node(VarList,@$.first_line,"VarList",-1,"",-1,-1.0);	
	insert($$,$1);}
		;
ParamDec	:	Specifier VarDec
{	$$ = build_node(ParamDec,@$.first_line,"ParamDec",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);}
		;
CompSt		:	LC DefList StmtList RC
{	$$ = build_node(CompSt,@$.first_line,"CompSt",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);	insert($$,$4);}
		|	error RC
		;
StmtList	:	Stmt StmtList
{	$$ = build_node(StmtList,@$.first_line,"StmtList",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);}
		|	/*empty*/	
{	$$ = build_node(StmtList,@$.first_line,"StmtList",-1,"",-1,-1.0);
	$$->isempty = true;}
		;
Stmt		:	Exp SEMI
{	$$ = build_node(Stmt,@$.first_line,"Stmt",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);}
		|	CompSt
{	$$ = build_node(Stmt,@$.first_line,"Stmt",-1,"",-1,-1.0);	
	insert($$,$1);}
		|	RETURN Exp SEMI
{	$$ = build_node(Stmt,@$.first_line,"Stmt",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	IF LP Exp RP Stmt	%prec LOWER_THAN_ELSE
{	$$ = build_node(Stmt,@$.first_line,"Stmt",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3); insert($$,$4);
	insert($$,$5);}
		|	IF LP Exp RP Stmt ELSE Stmt
{	$$ = build_node(Stmt,@$.first_line,"Stmt",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3); insert($$,$4);
	insert($$,$5);	insert($$,$6);	insert($$,$7);}
		|	WHILE LP Exp RP Stmt	
{	$$ = build_node(Stmt,@$.first_line,"Stmt",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3); insert($$,$4);
	insert($$,$5);}
		|	error SEMI
		;

DefList		:	Def DefList
{	$$ = build_node(DefList,@$.first_line,"DefList",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);}
		|	/*empty*/	
{	$$ = build_node(DefList,@$.first_line,"DefList",-1,"",-1,-1.0);
	$$->isempty = true;}
		;
Def		:	Specifier DecList SEMI
{	$$ = build_node(Def,@$.first_line,"Def",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	error SEMI
		;
DecList		:	Dec
{	$$ = build_node(DecList,@$.first_line,"DecList",-1,"",-1,-1.0);	
	insert($$,$1);}
		|	Dec COMMA DecList
{	$$ = build_node(DecList,@$.first_line,"DecList",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		;
Dec		:	VarDec
{	$$ = build_node(Dec,@$.first_line,"Dec",-1,"",-1,-1.0);	
	insert($$,$1);}
		|	VarDec ASSIGNOP Exp
{	$$ = build_node(Dec,@$.first_line,"Dec",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		;
Exp		:	Exp ASSIGNOP Exp
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	Exp AND Exp
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	Exp OR Exp
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	Exp RELOP Exp
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	Exp ADD Exp
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}	
		|	Exp SUB Exp
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}	
		|	Exp MUL Exp
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	Exp DIV Exp
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	LP Exp RP
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	SUB Exp
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);}
		|	NOT Exp
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);}
		|	ID LP Args RP
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);	insert($$,$4);}
		|	ID LP RP
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	Exp LB Exp RB
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);	insert($$,$4);}
		|	Exp DOT ID
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	ID
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);}		
		|	INT
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);}		
		|	FLOAT
{	$$ = build_node(Exp,@$.first_line,"Exp",-1,"",-1,-1.0);	
	insert($$,$1);}	
		|	LB error RB
		|	error RP
		|	LP error RP
		;
Args		:	Exp COMMA Args
{	$$ = build_node(Args,@$.first_line,"Args",-1,"",-1,-1.0);	
	insert($$,$1);	insert($$,$2);	insert($$,$3);}
		|	Exp
{	$$ = build_node(Args,@$.first_line,"Args",-1,"",-1,-1.0);	
	insert($$,$1);}
		;


%%

yyerror(char* msg)
{	
	print = false;
	fprintf(stderr,"error type B at line %d: %s\n",yylineno,msg);	
}

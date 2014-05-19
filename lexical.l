%{
	#include "syntax.tab.h"
	#include "tree/tree.h"
	#include <string.h>
	int yycolumn = 1;

	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;\
	yylloc.first_column=yycolumn;yylloc.last_column = yycolumn + yyleng-1;\
	yycolumn+=yyleng;
	int lines = 1;
	
%}


octonary	0[1-7][0-7]*
hexadeciaml	0(X|x)([1-9]|[A-F]|[a-f])([0-9]|[A-F]|[a-f])*
letter		[a-zA-Z]
digit		[0-9]
int		(0|([1-9]){digit}*)
float		(0|([1-9]){digit}*)?(\.({digit}*))?(E[+-]?([1-9]){digit}*)?
id		(_|{letter})(_|{letter}|{digit})*
delim 		[ \t\n]
ws		{delim}+
type		"int"|"float"
%%

"\n"		{lines ++; yycolumn = 1;yylineno++;}
" "		{			}
"\t"		{			}
"+"		{	yylval.type_node = 
			build_node(eADD,lines,"ADD",0,"",-1,-1.0);
			return ADD;	}
"-"		{	yylval.type_node = 
			build_node(eSUB,lines,"SUB",0,"",-1,-1.0);
			return SUB;	}
"*"		{	yylval.type_node = 
			build_node(eMUL,lines,"MUL",0,"",-1,-1.0);
			return MUL;	}
"/"		{ 	yylval.type_node = 
			build_node(eDIV,lines,"DIV",0,"",-1,-1.0);
			return DIV;	}
"&&" 		{	yylval.type_node = 
			build_node(eAND,lines,"AND",0,"",-1,-1.0);
			return AND;	}
"||"		{	yylval.type_node = 
			build_node(eOR,lines,"OR",0,"",-1,-1.0);
			return OR;	}
"="		{	yylval.type_node = 
			build_node(eASSIGNOP,lines,"ASSIGNOP",0,"",-1,-1.0);
			return ASSIGNOP;}
","		{ 	yylval.type_node = 
			build_node(eCOMMA,lines,"COMMA",0,"",-1,-1.0);
			return COMMA;	}
"!="		{	yylval.type_node = 
			build_node(eRELOP,lines,"RELOP",3,"",-1,-1.0);
			return RELOP;	}
"==" 		{	yylval.type_node = 
			build_node(eRELOP,lines,"RELOP",4,"",-1,-1.0);
			return RELOP;	}
"<"		{ 	yylval.type_node = 
			build_node(eRELOP,lines,"RELOP",5,"",-1,-1.0);
			return RELOP;	}
">"		{ 	yylval.type_node = 
			build_node(eRELOP,lines,"RELOP",6,"",-1,-1.0);
			return RELOP;	}
">="		{	yylval.type_node = 
			build_node(eRELOP,lines,"RELOP",7,"",-1,-1.0);
			return RELOP;	}
"<="		{	yylval.type_node = 
			build_node(eRELOP,lines,"RELOP",8,"",-1,-1.0);
			return RELOP;	}
";"		{ 	yylval.type_node = 
			build_node(eSEMI,lines,"SEMI",0,"",-1,-1.0);
			return SEMI;	}
"."		{ 	yylval.type_node = 
			build_node(eDOT,lines,"DOT",0,"",-1,-1.0);
			return DOT;	}
"!"		{ 	yylval.type_node = 
			build_node(eNOT,lines,"NOT",0,"",-1,-1.0);
			return NOT;	}
"("		{	yylval.type_node = 
			build_node(eLP,lines,"LP",0,"",-1,-1.0);
			return LP;	}
")"		{	yylval.type_node = 
			build_node(eRP,lines,"RP",0,"",-1,-1.0);
			return RP;	}
"["		{ 	yylval.type_node = 
			build_node(eLB,lines,"LB",0,"",-1,-1.0);
			return LB;	}
"]"		{ 	yylval.type_node = 
			build_node(eRB,lines,"RB",0,"",-1,-1.0);
			return RB;	}
"{"		{ 	yylval.type_node = 
			build_node(eLC,lines,"LC",0,"",-1,-1.0);
			return LC;	}
"}"		{ 	yylval.type_node = 
			build_node(eRC,lines,"RC",0,"",-1,-1.0);
			return RC;	}
"struct"	{ 	yylval.type_node = 
			build_node(eSTRUCT,lines,"STRUCT",0,"",-1,-1.0);
			return STRUCT;	}
"return"	{ 	yylval.type_node = 
			build_node(eRETURN,lines,"RETURN",0,"",-1,-1.0);
			return RETURN;	}
"if"		{ 	yylval.type_node = 
			build_node(eIF,lines,"IF",0,"",-1,-1.0);
			return IF;	}
"then"		{ 	yylval.type_node = 
			build_node(eTHEN,lines,"THEN",0,"",-1,-1.0);
			return THEN;	}
"else"		{ 	yylval.type_node = 
			build_node(eELSE,lines,"ELSE",0,"",-1,-1.0);
			return ELSE;	}
"while"		{ 	yylval.type_node = 
			build_node(eWHILE,lines,"WHILE",0,"",-1,-1.0);
			return WHILE;	}
"when"		{	yylval.type_node = 
			build_node(eWHEN,lines,"WHEN",0,"",-1,-1.0);
			return WHEN;	}
"for"		{ 	yylval.type_node = 
			build_node(eFOR,lines,"FOR",0,"",-1,-1.0);
			return FOR;	}	
{int}		{	int a = atoi(yytext);
			yylval.type_node = 
			build_node(eINT,lines,"INT",0,"",a,-1.0);
			return INT;	}
{float}		{	float a = atof(yytext);
			yylval.type_node = 
			build_node(eFLOAT,lines,"FLOAT",0,"",-1,a);
			return FLOAT;	}
{type}		{	int a;	
			if(strcmp(yytext,"int") == 0)
				a = 1;
			else
				a = 2;
			yylval.type_node = 
			build_node(eTYPE,lines,"TYPE",a,"",-1,-1.0);
			return TYPE;	}
{id}		{ 	yylval.type_node = 
			build_node(eID,lines,"ID",0,yytext,-1,-1.0);
			return ID;	}
{octonary}	{	int num = strlen(yytext);
			int i;
			int a = 0;
			for(i = 1 ; i < num ; i ++)	
			 a = a * 8 + yytext[i] - '0'; 
			yylval.type_node = 
			build_node(eINT,lines,"INT",0,"",a,-1.0);
			return INT;	}
{hexadeciaml}	{		
			int i;
			int a = 0;
			int b;
			int num = strlen(yytext);
			for(i = 2 ; i < num ; i ++)
			{
				if(yytext[i] >= 'a' && yytext[i] <= 'f')
					b = yytext[i] - 'a' + 10;
				else if(yytext[i] >= 'A'&&yytext[i] <= 'F')
					b = yytext[i] - 'A' + 10;
				else
					b = yytext[i] - '0';
				a = a * 16 + b;
			}
			yylval.type_node = 
			build_node(eINT,lines,"INT",0,"",a,-1.0);
			return INT;
		}
.       	{	print = false;
			printf("ERROR type A at line %d:Mysterious character \'%s\'\n",lines,yytext);}

%%


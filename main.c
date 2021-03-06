#include <stdio.h>
#include <stdlib.h>
#include "tree/tree.h"
#include "table/table.h"
#include "semantics/semantics.h"
#include "intercode/intercode.h"
#include "intercode/createintercode.h"
#include "assembler/assembler.h"
int main(int argc, char** argv)
{
	if(argc <= 2) 
	{
		printf("error Please input test file and the output file!\n");
		return 1;
	}
	FILE* f = fopen(argv[1],"r");
	if(!f)
	{
		perror(argv[1]);
		return 1;
	}
	yyrestart(f);
	yyparse();
	if(print)
	{
		//display(head,0);
		table_insert("read",0,3,1,-1);
		table_insert("write",0,3,1,-1);
		tablehead->func_next = add_func(1,1);
		analysis(head);
		if(synerror == 0)
		{
			struct table_node * temp = tablehead;
			int num = 1;
			for(; temp != NULL; temp = temp->next)
			{
				if(temp->type != 3 && temp->basic_type == 1)
				{
					variable * node = (variable *)malloc(sizeof(variable));
					strcpy(node->name,temp->name);
					node->number = num;
					num ++;
					node->next = variableHead;
					variableHead = node;
				}
			}
			variable * t = variableHead;  
			for(;t != NULL ; t = t->next)
			{
				t->number =num - t->number;
			}
			createintercode(head);
		/*	FILE* wf = fopen(argv[2],"w+");
			if(!wf)
			{
				perror(argv[2]);
				return 1;
			}
		*/
			FILE* wf1 = fopen(argv[2],"w+");
			if(!wf1)
			{
				perror(argv[2]);
				return 1;
			}
			optimize();
			//printcode(wf);
			create_code(wf1);
			//fclose(wf);
			fclose(wf1); 
		}
		else
		{

			printf("semantics error!!!!\n");
		}
	}
	else
		printf("Syntax ERROR !!!!\n");
	
	return 0;

}

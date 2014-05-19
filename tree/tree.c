#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"
struct Node * head = NULL;
bool print = true;
void insert(struct Node *father , struct Node *child)
{
	int i = 0;
	for(; i < CHILD_NUM ; i ++)
	{
		if(father->child[i] == NULL)
		{
			child->father = father;
			father->child[i] = child;
			return;
		}
	}
}
void display(struct Node *node,int num)
{
	
	if(node != NULL && node->isempty == false)
	{
		int i;	
		for( i = 0 ; i < num ; i ++)
			printf("  ");
		if(node->iftype == -1)	
		{
			printf("%s(%d)\n",node->name,node->line);
		}
		else if(node->iftype == 0)
		{
			if(strcmp(node->name,"INT") == 0)
				printf("INT: %d\n",node->intvalue);		
			else if(strcmp(node->name,"FLOAT") == 0)
				printf("FLOAT: %f\n",node->floatvalue);
			else if(strcmp(node->name,"ID") == 0)
				printf("ID: %s\n",node->ID);
			else
				printf("%s\n",node->name);
		}
		else if(node->iftype == 1)
		{
			printf("TYPE: int\n");
		}
		else if(node->iftype == 2)
		{
			printf("TYPE: float\n");
		}
		else
		{
			printf("%s\n",node->name);
		}
		for(i = 0 ; i < CHILD_NUM ; i ++)
		{
			display(node->child[i],num + 1);
		}
	}
	
}
struct Node * build_node(enum Type type, int line, char *name, int iftype, 
				char *ID, int intvalue, float floatvalue)
{
	struct Node *node = malloc(sizeof(struct Node));
	node->isempty = false;
	node->type = type;
	node->line = line;
	strcpy(node->name , name);
	node->iftype = iftype;
	strcpy(node->ID , ID);
	node->intvalue = intvalue;
	node->floatvalue = floatvalue;
	int i;
	for(i = 0 ; i < CHILD_NUM ; i ++)
	{
		node->child[i] = NULL;
	}
	node->father = NULL;	
//	printf("%s  %d\n",node->name,node->line);
	return node;
}


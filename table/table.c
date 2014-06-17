#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "../semantics/semantics.h"
struct table_node *tablehead = NULL;
void table_insert(char *name,int line,int type,int basic_type,int instruct)
{
	int i = 1;
	if(type == 3)
		i = 3;
	struct table_node *p = table_check(name,i);
	struct struct_node *q = check(name);
	if(p != NULL || q != NULL)
	{
		if(type == 3)
		{
			synerror = 1;
			printf("Error type 4 at line %d:Redefined function \"%s\"\n",line,name);
			return;		
		}
		if(instruct < 0)
		{
			synerror = 1;
			printf("Error type 3 at line %d:Redefined variable \"%s\"\n",line,name);
			return;
		}
		if(instruct > 0)
		{
			synerror = 1;
			printf("Error type 15 at line %d:Redefined field\'%s\'\n",line,name);
			return;
		}
		return;
	}
	struct table_node *node = (struct table_node *)malloc(sizeof(struct table_node));
	strcpy(node->name,name);
	node->line = line;
	node->type = type;
	node->basic_type = basic_type;
	node->next = tablehead;
	node->func_next = NULL;
	node->array_next = NULL;
	node->struct_next = NULL;
	tablehead = node;
}
struct func_node* add_func(int basic_type, int type)
{
	struct func_node *temp = (struct func_node *)malloc(sizeof(struct func_node));
	temp->type = type;
	temp->basic_type = basic_type;
	temp->next = NULL;
	temp->struct_next = NULL;
	temp->array_next = NULL;
	return temp;
	
}
struct array_node* add_array(int type ,int size)
{
	struct array_node *temp = (struct array_node *)malloc(sizeof(struct array_node));
	temp->type = type;
	temp->size = size;
	temp->next = NULL;
	return temp;
}
struct struct_node* add_struct(char *name ,int line,int type,int basic_type)
{
	struct struct_node *temp = (struct struct_node *)malloc(sizeof(struct struct_node));
	strcpy(temp->name,name);
	temp->line = line;
	temp->type = type;
	temp->basic_type = basic_type;
	temp->array_next = NULL;
	temp->struct_next = NULL;
	temp->next = NULL;
	return temp;
}
void table_delete(struct table_node *node)
{

}

struct table_node *table_check(char *name,int type)//type 1:basic 3:func
{
	struct table_node *temp;
	for(temp = tablehead ; temp != NULL ; temp = temp->next)
	{
		int t = temp->type;
		if(t != 3)
			t = 1;
		if(strcmp(temp->name,name) == 0 && t == type)
			return temp;
	}
	return NULL;
}
int struct_equal(struct struct_node *node1,struct struct_node *node2)
{
	struct struct_node *t1 = node1;
	struct struct_node *t2 = node2;
	while(1)
	{
		if(t1 == NULL && t2 == NULL)
			return 1;
		else if(t1 == NULL && t2 != NULL)
			return -1;
		else if(t1 != NULL && t2 == NULL)
			return -1;
		else
		{
			if(t1->type == t2->type && t1->basic_type == t2->basic_type)
			{
				if(t1->basic_type == 3)
				{
					struct struct_node *a1 = t1->struct_next;
					struct struct_node *a2 = t2->struct_next;
					int a = struct_equal(a1,a2);
					if(a == -1)
						return -1;
				}
				if(t1->type == 2)
				{
					struct array_node *a1 = t1->array_next;
					struct array_node *a2 = t2->array_next;
					while(1)
					{
						if(a1 != NULL && a2 != NULL)
						{
							a1 = a1->next;
							a2 = a2->next;
						}
						else if(a1 == NULL && a2 != NULL)
							return -1;
						else if(a1 != NULL && a2 == NULL)
							return -1;
						else
							break;
					}
				}
				t1 = t1->next;
				t2 = t2->next;
			}
			else
				return -1;
		}
	}
}
int exist(char *name)
{
	struct table_node *temp = tablehead;
	for( ; temp != NULL ; temp = temp->next)
	{
		if(strcmp(name,temp->name) == 0)
			return 1;
	}
	return -1;
}	











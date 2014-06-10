#include "semantics.h"
#include "../tree/tree.h"
#include "../table/table.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
int synerror = 0;
struct struct_list *struct_list_head = NULL;
int isfunc = 0;
void list_insert(char *name,int line)
{
	struct struct_node *t = check(name);
	if(t != NULL)
	{
		synerror = 1;
		printf("Error type 16 at line %d: Duplicated name \'%s\'\n",line,name);
	}
	struct struct_list *node = (struct struct_list *)malloc(sizeof(struct struct_list));
	memset(node->name,0,sizeof(node->name));
	strcpy(node->name,name);
	node->info_next = NULL;
	node->next = struct_list_head;
	struct_list_head = node;
}
struct struct_node *check(char *name)
{
	struct struct_list *temp = struct_list_head;
	for( ; temp != NULL ; temp = temp->next )
	{
		if(strcmp(name,temp->name) == 0)
		{
			return temp->info_next;
		}
	}
	return NULL;
}
struct table_node *analysis_exp(struct Node *node)
{
	
	struct Node *temp = node;
	if(temp->child[0] != NULL && temp->child[2] != NULL && temp->child[3] == NULL)
	{
		if(temp->child[0]->type == Exp && temp->child[2]->type == Exp)
		{
			if(temp->child[1]->type == eADD || temp->child[1]->type == eSUB
				|| temp->child[1]->type == eMUL || temp->child[1]->type == eDIV)
			{
				struct table_node *p1 = analysis_exp(temp->child[0]);
				struct table_node *p2 = analysis_exp(temp->child[2]);
				if(p1 == NULL || p2 == NULL)
					return NULL;
				if(p1->type != 2 && p2->type != 2 && p1->basic_type != 3 && p2->basic_type != 3)
				{
					if(p1->basic_type == p2->basic_type)
						return p1;
					else
					{
						synerror = 1;
						printf("Error type 7 at line %d: Operands type mismatched\n",temp->child[1]->line);
						return NULL;
					}
				}
				else
				{
					synerror = 1;
					printf("Error type 7 at line %d: Operands type mismatched\n",temp->child[1]->line);
						return NULL;										
					
				}
			}
			if(temp->child[1]->type == eAND || temp->child[1]->type == eOR)
			{
				struct table_node *p1 = analysis_exp(temp->child[0]);
				struct table_node *p2 = analysis_exp(temp->child[2]);
				if(p1 == NULL || p2 == NULL)
					return NULL;
				if(p1->type != 2 && p2->type != 2 && p1->basic_type == 1 && p2->basic_type == 1)
				{
					return p1;
				}
				else
				{
					synerror = 1;
					printf("Error type 7 at line %d: Type of AND operation and OR operation mismatched\n",temp->child[1]->line);
					return NULL;
				}
			}
			if(temp->child[1]->type == eRELOP)
			{
			
				struct table_node *p1 = analysis_exp(temp->child[0]);
				struct table_node *p2 = analysis_exp(temp->child[2]);
				if(p1 == NULL || p2 == NULL)
					return NULL;
				if(p1->type != 2 && p2->type != 2 && p1->basic_type != 3 && p2->basic_type != 3
					 && p1->basic_type == p2->basic_type) 
				{
					struct table_node *p = (struct table_node *)malloc(sizeof(struct table_node));
					memset(p->name,0,sizeof(p->name));
					p->type = 1;
					p->basic_type = 1;
					p->line = temp->child[0]->line;
					p->next = NULL;
					p->array_next = NULL;
					p->struct_next = NULL;
					p->func_next = NULL;
					return p;
				}
				else
				{
					synerror = 1;
					printf("Error type 7 at line %d: Type of RELOP operation mismatched\n",temp->child[1]->line);
					return NULL;
					
				}

			}
			if(temp->child[1]->type == eASSIGNOP)
			{
				
				struct Node *p = temp->child[0];
				if((p->child[0]->type == eID && p->child[1] == NULL) ||
					(p->child[1] != NULL && p->child[1]->type == eLB) ||
					(p->child[1] != NULL && p->child[1]->type == eDOT))
				{
					struct table_node *p1 = analysis_exp(temp->child[0]);
					struct table_node *p2 = analysis_exp(temp->child[2]);
					if(p1 == NULL || p2 == NULL)
						return NULL;
					if(p1->type != 2 && p2->type != 2 && p1->basic_type == p2->basic_type)
					{
						if(p1->basic_type != 3)
							return p1;
						else
						{
							int a = struct_equal(p1->struct_next,p2->struct_next);
							if(a == -1)
							{
								synerror = 1;
								printf("Error type 5 at line %d: Type mismatched\n",temp->child[1]->line);//struct not equal
								return NULL;
							}
							else
							{
								return p1;
							}
						}
					}
					else if(p1->type == 2 && p2->type == 2 && p1->basic_type == p2->basic_type)
					{
						if(p1->basic_type == 3)
						{
							int a = struct_equal(p1->struct_next,p2->struct_next);
							if(a == -1)
							{
								synerror = 1;
								printf("Error type 5 at line %d: Type mismatched\n",temp->child[1]->line);//struct not equal
								return NULL;
							}
						}
						struct array_node *t1 = p1->array_next;
						struct array_node *t2 = p2->array_next;
						while(1)
						{
							if(t1 != NULL && t2 != NULL)
							{
								t1 = t1->next;
								t2 = t2->next;
							}
							else if(t1 == NULL && t2 == NULL)
							{
								return p1;
							}
							else
							{
								synerror = 1;
								printf("Error type 5 at line %d: Type mismatched\n",temp->child[1]->line);//struct not equal
								return NULL;
							}
						}
					}
					else
					{
						synerror = 1;
						printf("Error type 5 at line %d: Type mismatched\n",temp->child[1]->line);
						return NULL;
					}
				}
				else
				{
					synerror = 1;
					printf("Error type 6 at line %d: The left-hand side of an assignment must be a variable\n",temp->child[1]->line);
					return NULL;
				}	
			}			
		}
	}
	if(temp->child[1] != NULL)
	{
		if(temp->child[1]->type == Exp)
		{
			if(temp->child[0]->type == eLP)
			{
				struct table_node *p = analysis_exp(temp->child[1]);
				return p;
			}
			else if(temp->child[0]->type == eSUB)	
			{
				struct table_node *p = analysis_exp(temp->child[1]);
				if(p->type != 2 &&p->basic_type != 3)
					return p;
				else 
				{
					synerror = 1;
					printf("Error type 7 at line %d: Type mismatched(In MINUS operation variable must be int or float)\n",temp->child[1]->line);
					return NULL;
				}
			}
			else if(temp->child[0]->type == eNOT)
			{
				struct table_node *p = analysis_exp(temp->child[1]);
				if(p->type != 2 &&p->basic_type == 1)
					return p;
				else 
				{
					synerror = 1;
					printf("Error type 5 at line %d: Type mismatched(In NOT operation variable must be int)\n",temp->child[1]->line);
					return NULL;
				}
			}	
		}
	}
	if(temp->child[0]->type == eID && temp->child[1] != NULL)
	{
		
		struct Node *p = temp->child[0];
		char a[64];
		memset(a,0,sizeof(a));
		strcpy(a,p->ID);
		struct table_node * q = table_check(a,3);
		int e = exist(a);
		
		if(q == NULL && e == -1)
		{
			synerror = 1;
			printf("Error type 2 at line %d:Undefined function \"%s\"\n",temp->child[0]->line,a);
			return NULL;
		}
		else if(q == NULL && e == 1)
		{
			synerror = 1;
			printf("Error type 11 at line %d: \"%s\" must be a function\n",temp->child[0]->line,a);
			return NULL;
		}
		else
		{
			struct func_node *func = q->func_next;
			struct func_node *f = func;
			if(func == NULL && temp->child[2]->type == eRP)
			{
				return q;
			}
			if(func != NULL && temp->child[2]->type == eRP)
			{
				synerror = 1;
				printf("Error type 9 at line %d: The method \"%s() is not applicable for the arguments \"(",temp->child[0]->line,q->name);
				for( ; f->next != NULL ;f = f->next)
				{
					if(f->basic_type == 1)
						printf("int");
					else if(f->basic_type == 2)
						printf("float");
					else if(f->basic_type == 3)
						printf("struct");
					if(f->type == 2)
						printf(" array");
					printf(",");	
				}
				if(f->basic_type == 1)
					printf("int");
				else if(f->basic_type == 2)
					printf("float");
				else if(f->basic_type == 3)
					printf("struct");
				if(f->type == 2)
					printf(" array");
				printf(")\"\n");	
				return q;
			}
			p = temp->child[2];
			struct table_node *variable[20];
			int i = 0;
			for( ; i < 20 ; i ++)
			{
				variable[i] = NULL;
			}
			for(i = 0 ; p != NULL ; p = p->child[2])
			{
				struct Node *p1 = p->child[0];
				//printf("%s\n",p1->name);
				struct table_node *q1 = analysis_exp(p1);
				variable[i] = q1;
				i ++;
			}
			
			int error = 0;
			
			for(i = 0; variable[i] != NULL ; i ++)
			{
				struct table_node *q1 = variable[i];
				if(func == NULL)
				{
					error = 1;
					break;
				}
				if((q1->type == func->type || q1->type == 3)  && q1->basic_type == func->basic_type)
				{
					if(q1->type == 2)
					{
						struct array_node *array = q1->array_next;
						int num1 = 0;
						int num2 = 0;
						for(; array != NULL ; array = array->next)
							num1 ++;
						array = func->array_next;
						for(; array != NULL ; array = array->next)
							num2 ++;
						if(num1 != num2)
						{
							error = 1;
							
						}
					}
					if(q1->basic_type == 3)
					{
						int a = struct_equal(q1->struct_next,func->struct_next);
						if(a == -1)
						{
							error = 1;
							
						}
						//struct equal
					}
				}
				else
				{
					error = 1;
				}
				func = func->next;
			}
			if(func != NULL)
				error = 1;
			
			if(error == 1)
			{
				synerror = 1;
				printf("Error type 9 at line %d: The method \"%s(",temp->child[0]->line,temp->child[0]->ID);
				
				for(i = 0 ; variable[i] != NULL ; i ++)
				{
					if(variable[i+1] == NULL)
						break;
					if(variable[i]->basic_type == 1)
						printf("int");
					else if(variable[i]->basic_type == 2)
						printf("float");
					else if(variable[i]->basic_type == 3)
						printf("struct");
					if(variable[i]->type == 2)
						printf(" array");
					printf(",");	
				}
				if(variable[i]!=NULL)
				{
					if(variable[i]->basic_type == 1)
						printf("int");
					else if(variable[i]->basic_type == 2)
						printf("float");
					else if(variable[i]->basic_type == 3)
						printf("struct");
					if(variable[i]->type == 2)
						printf(" array");
				}
				printf(")\" is not applicable for arguments \"(");
				
				if(f == NULL)
				{
					printf(")\"\n");
					return q;
				}
				for( ; f->next != NULL ;f = f->next)
				{
					if(f->basic_type == 1)
						printf("int");
					else if(f->basic_type == 2)
						printf("float");
					else if(f->basic_type == 3)
						printf("struct");
					if(f->type == 2)
						printf(" array");
					printf(",");
				}
				if(f->basic_type == 1)
					printf("int");
				else if(f->basic_type == 2)
					printf("float");
				else if(f->basic_type == 3)
					printf("struct");
				if(f->type == 2)
					printf(" array");
				printf(")\"\n");	
				
				return q;

			}
		}
		
		return q;
		
	}
	if(temp->child[0]->type == Exp && temp->child[3] != NULL)
	{
		
		struct Node *p = temp->child[0];
		int num = 1;
		struct table_node *p2 = analysis_exp(temp->child[2]);
		if(p2->type == 1 && p2->basic_type == 1)
		{
			while(1)
			{
				if(p->child[0]->type == Exp && p->child[1]->type == eLB)
				{
					p2 = analysis_exp(p->child[2]);
					if(p2->type == 1 && p2->basic_type == 1);
					else
					{
						synerror = 1;
						printf("Error type 12 at line %d: Operands type mistaken\n",temp->child[1]->line);
						return NULL;
					}
					p = p->child[0];
					num ++;
				}
				else if(p->child[0]->type == eID)
				{
					struct table_node *t = table_check(p->child[0]->ID,1);
					if(t->array_next == NULL)
					{
						synerror = 1;
						printf("Error type 10 at line %d: \"%s\" must be an array\n",temp->child[1]->line,p->child[0]->ID);
						return NULL;
					}
					struct table_node * t1 = (struct table_node *)malloc(sizeof(struct table_node));
					memset(t1->name,0,sizeof(t1->name));
					t1->type = t->type;
					t1->basic_type = t->basic_type;
					t1->line = t->line;
					t1->next = NULL;
					t1->func_next = NULL;
					t1->struct_next = t->struct_next;
					struct array_node *t2 = t->array_next;
					int i = 0;
					for(; i < num ; i ++)
					{
						if(t2 == NULL)
						{
							synerror = 1;
							printf("Error type 10 at line %d: \"%s",temp->child[1]->line,p->child[0]->ID);
							int j = 0;
							for( ;j < i ; j ++)
								printf("[]");
							printf("\" must be an array\n");
							return NULL;
						}
						t2 = t2->next;
					}
					t1->array_next = t2;
					if(t2 == NULL)
						t1->type = 1;
					//printf("t1->type %d\n",t1->type);
					return t1;
				}
				else if(p->child[0]->type == Exp && p->child[1]->type == eDOT)
				{
					struct table_node * t = analysis_exp(p);
					if(t != NULL)
					{
						
						struct table_node * t1 = (struct table_node *)malloc(sizeof(struct table_node));
						memset(t1->name,0,sizeof(t1->name));
						t1->type = t->type;
						t1->basic_type = t->basic_type;
						t1->line = t->line;
						t1->next = NULL;
						t1->func_next = NULL;
						t1->struct_next = t->struct_next;
						struct array_node *t2 = t->array_next;
						int i = 0;
						for(; i < num ; i ++)
						{
							if(t2 == NULL)
							{
								synerror = 1;
								printf("Error type 10 at line %d: \"%s",temp->child[1]->line,t->name);
								int j = 0;
								for( ;j < i ; j ++)
									printf("[]");
								printf("\" must be an array\n");
								return NULL;
							}
							t2 = t2->next;
						}
						t1->array_next = t2;
						if(t2 == NULL)
							t1->type = 1;
						//printf("t1->type %d\n",t1->type);
						return t1;
					}
					return NULL;
				}
				else
				{
					synerror = 1;
					printf("Error type array\n");
					return NULL;
				}
			}
		}
		else
		{
			synerror = 1;
			printf("Error type 12 at line %d: Operands type mistaken\n",temp->child[1]->line);
			return NULL;
		}
	}
	if(temp->child[0]->type == Exp && temp->child[1]->type == eDOT)
	{
		//struct Node *temp = node;
		int i = 0;
		char name[10][64];
		for( ; i < 10 ; i ++)
		{
			memset(name,0,sizeof(name[i]));
		}
		i = 0;
		while(1)
		{
			if(temp->child[0]->type == Exp && temp->child[1]->type == eDOT)
			{
				strcpy(name[i],temp->child[2]->ID);
				i ++;
				temp = temp->child[0];
				
			}
			else if(temp->child[0]->type == eID && temp->child[1] == NULL)
			{
				char a[64];
				memset(a,0,sizeof(a));
				
				strcpy(a,temp->child[0]->ID);
				
				struct table_node *t =table_check(a,1);
				if(t == NULL)
				{
					synerror = 1;
					printf("Error type 1 at line %d: Undefined variable\"%s\" \n",temp->child[0]->line,a);
					return NULL;
				}
				struct struct_node *t1 = t->struct_next;
				
				for(i-- ; i >= 0 ; i--)
				{
					if(t1 != NULL)
					{
						for(;t1 != NULL; t1 = t1->next)
						{
							//printf("%d   %s    %s\n",i ,name[i],t1->name);
							if(strcmp(name[i],t1->name) == 0)
							{
								if(i == 0)
								{
									struct table_node *t2 = (struct table_node *)malloc(sizeof(struct table_node));
									memset(t2->name,0,sizeof(t2->name));
									strcpy(t2->name,name[i]);
									t2->line = temp->line;
									t2->type = t1->type;
									t2->basic_type = t1->basic_type;
									t2->next = NULL;
									t2->func_next = NULL;
									t2->struct_next = t1->struct_next;
									t2->array_next = t1->array_next;
									return t2;
								}
								else
								{
								//	printf("==%s %s\n",t1->name,t1->struct_next->name);
									t1 = t1->struct_next;
								}
								break;
							}
						}
						if(t1 == NULL)
						{
							synerror = 1;
							printf("Error type 14 at line %d: Un-existed field \"%s\"\n",temp->child[0]->line,name[i]);
							return NULL;
						}
						
					}
					else
					{
						synerror = 1;
						printf("Error type 13 at line %d: Illegal use of \".\"\n",temp->child[0]->line);
						return NULL;
					}
				}
			}
			else if(temp->child[0]->type == Exp && temp->child[3] != NULL)
			{
				struct table_node *t = analysis_exp(temp);
				if(t != NULL && t->basic_type == 3 && t->type == 1)
				{
					struct struct_node *t1 = t->struct_next;
					for(i-- ; i >= 0 ; i--)
					{
						if(t1 != NULL)
						{
							for(;t1 != NULL; t1 = t1->next)
							{
								if(strcmp(name[i],t1->name) == 0)
								{
									if(i == 0)
									{
										struct table_node *t2 = (struct table_node *)malloc(sizeof(struct table_node));
										memset(t2->name,0,sizeof(t2->name));
										strcpy(t2->name,name[i]);
										t2->line = temp->line;
										t2->type = t1->type;
										t2->basic_type = t1->basic_type;
										t2->next = NULL;
										t2->func_next = NULL;
										t2->struct_next = t1->struct_next;
										t2->array_next = t1->array_next;
										return t2;
									}
									else
									{
										t1 = t1->struct_next;
									}
									break;
								}
							}
							synerror = 1;
							printf("Error type 14 at line %d: Un-existed field \"%s\"\n",temp->child[1]->line,t1->name);
							return NULL;	
						}

					} 
				}
				else
				{
					synerror = 1;
					printf("Error type 13 at line %d: Illegal use of \".\"\n",temp->child[0]->line);
					return NULL;
				}
			}
			else
			{
				synerror = 1;
				printf("Error type 13 at line %d: Illegal use of \".\"\n",temp->child[0]->line);
				return NULL;
			}
		}
	}
	if(temp->child[0]->type == eID && temp->child[1] == NULL)
	{
		struct table_node *p = table_check(temp->child[0]->ID,1);
		if(p == NULL)
		{
			synerror = 1;
			printf("Error type 1 at line %d: Undefined variable \"%s\"\n",temp->child[0]->line,temp->child[0]->ID);
		}
		return p;
	}
	if(temp->child[0]->type == eINT)
	{
		struct table_node *p = (struct table_node *)malloc(sizeof(struct table_node));
		memset(p->name,0,sizeof(p->name));
		p->type = 1;
		p->basic_type = 1;
		p->line = temp->child[0]->line;
		p->next = NULL;
		p->array_next = NULL;
		p->struct_next = NULL;
		p->func_next = NULL;
		return p;
	}
	if(temp->child[0]->type == eFLOAT)
	{
		struct table_node *p = (struct table_node *)malloc(sizeof(struct table_node));
		memset(p->name,0,sizeof(p->name));
		p->type = 1;
		p->basic_type = 2;
		p->line = temp->child[0]->line;
		p->next = NULL;
		p->array_next = NULL;
		p->struct_next = NULL;
		p->func_next = NULL;
		return p;
	}
	return NULL;
}
void analysis_stmt(struct Node *node, struct table_node *func)
{
	struct Node *temp = node;
	if(temp->child[0]->type == CompSt)
	{
		temp = temp->child[0];
		add_deflist(temp->child[1],-1);
		analysis_stmtlist(temp->child[2],func);
	}
	int i;
	for(i = 1 ; i < CHILD_NUM ; i ++)
	{
		if(temp->child[i] != NULL)
		{
			if(temp->child[i]->type == Stmt)
				analysis_stmt(temp->child[i],func);
		}
	}
	
	if(temp->child[0]->type == Exp)
	{
		struct table_node *p = analysis_exp(temp->child[0]);
		if( p == NULL)
		{

		}//Exp SEMI
	}
	//printf("1111\n");
	if(temp->child[1] != NULL)
	{
		if(temp->child[1]->type == Exp)
		{
			struct table_node *p = analysis_exp(temp->child[1]);
			if(p != NULL)
			{
				if(p->type != 2)
				{
					if(p->basic_type == func->basic_type)
					{
						if(p->basic_type == 3)
						{
							int a = struct_equal(p->struct_next,func->struct_next);
							if(a == -1)
							{
								synerror = 1;
								printf("Error type 8 at line %d: The return type mismatched\n",temp->child[0]->line);
							}
						}					
					}
					else 
					{
						synerror = 1;
						printf("Error type 8 at line %d: The return type mismatched\n",temp->child[0]->line);
					}
				}
				else
				{
					synerror = 1;
					printf("Error type 8 at line %d: The return type mismatched\n",temp->child[0]->line);
				}
			}
			//return EXP
		}
	}
	if(temp->child[2] != NULL)
	{
		if(temp->child[2]->type == Exp)
		{
			struct table_node *p = analysis_exp(temp->child[2]);
			if(p != NULL)
			{
				if(p->type != 2 && p->basic_type == 1);
				else 
				{
					synerror = 1;
					printf("Error type: It must be int type in if() or while()\n");
				}
				
			}
		}
	}
}
void analysis_stmtlist(struct Node *node, struct table_node *func)
{
	
	struct Node *temp = node;
	for(; temp->child[1] != NULL ; temp = temp->child[1])
	{
		//printf("%s %s\n", temp->name,temp->child[1]->name);
		struct Node *temp1 = temp->child[0];
		analysis_stmt(temp1,func);
	}
	
}
struct struct_node *set_structinfo(struct Node *node)//StructSpecifier
{
	
	struct struct_node* head = NULL;
	struct struct_node* tail = NULL;
	struct Node *temp = node;
	if(temp->child[3] == NULL)
	{
		temp = temp->child[1]->child[0];
		char a[64];
		memset(a,0,sizeof(a));
		strcpy(a,temp->ID);
		return check(a);	
	}
	temp = temp->child[3];
	//printf("---\n");
	
	//printf("asdasdasd\n");
	//printf("@@%s\n",temp->name);
	
	for(;temp->child[1] != NULL;temp = temp->child[1])
	{
		
		struct Node *temp1 = temp->child[0]->child[1];
		struct Node *type = temp->child[0]->child[0]->child[0];
		//printf("%s\n",temp1->name);
		for(;temp1 != NULL ; temp1 = temp1->child[2])
		{	
			
			//printf("222\n");
			struct Node *temp2 = temp1->child[0];
			
			if(temp2->child[1] != NULL && isfunc < 0)
			{
				synerror = 1;
				printf("Error type 15 at line %d: Can\'t assign a variable in struct\n",temp2->child[1]->line);
			}
			
			//printf("--%s\n",temp2->name);
			temp2 = temp2->child[0];
			//printf("---%s\n",temp2->name);
			int basic_type;	
			struct struct_node *p = NULL;		
			if(type->type == eTYPE)
			{
				//temp2 VarDec
				//printf("--%s\n",temp2->name);
				basic_type = type->iftype;
			}
			else
			{
				
				basic_type = 3; 
			}
			if(temp2->type == VarDec)
			{
				
				//printf("--%s\n",temp2->child[0]->name);
				if(temp2->child[0]->type == eID)
				{
					
					struct Node *t = temp2->child[0];
					char a[64];
					memset(a,0,sizeof(a));
					strcpy(a,t->ID);
					//printf("%s,%s\n",a,t->name);
					int line = t->line;
					
					p = add_struct(a,line,1,basic_type);
					if(head == NULL)
					{
						
						head = p;
						tail = p;
					}
					else
					{
						tail->next = p;
						tail = p;
					}
					
				}
				else if(temp2->child[0]->type == VarDec)
				{
					//printf("111\n");
					struct Node *t = temp2;
					//printf("111\n");
					while(t->child[0]->type != eID)
						t = t->child[0];
					char a[64];
					
					memset(a,0,sizeof(a));
					strcpy(a,t->child[0]->ID);
					int line = t->child[0]->line;
					
					p = add_struct(a ,line,2,basic_type);
					//table_insert(a,line,2,basic_type);
					t = t->father;
					
					//printf("-%d \n",temp->child[2]->intvalue);
					int s;
					s = t->child[2]->intvalue;
					
					//printf("----%d\n",s);
					p->array_next = add_array(basic_type,s);
					
						
					struct array_node *t1 = p->array_next;			
					for(t = t->father;t->child[2] != NULL;t = t->father)
					{
						if(t->child[2]->type == eINT)
						{
							//printf("111\n");
							int size = t->child[2]->intvalue;
							t1->next = add_array(basic_type,size);
							t1 = t1->next;
						}
					}
					
					if(head == NULL)
					{
					
						head = p;
						tail = p;
					}
					else
					{
						
						tail->next = p;
						tail = p;
					}
				}
			}
			if(basic_type == 3)
			{
				
				struct Node * q = type;
				//printf("&&%s\n",q->name);
				//printf("%s %d %d\n",p->name,p->type,p->basic_type);
				
				p->struct_next = set_structinfo(q);				
			}			
		}
		
	}	
	return head;
}
void add_id(int basic_type, struct Node *node,int instruct)//VarDec
{
	if(node != NULL)
	{
		if(node->type == VarDec)
		{
			if(node->child[0]->type == eID)
			{
				struct Node *temp = node->child[0];
				char a[64];
				memset(a,0,sizeof(a));
				strcpy(a,temp->ID);
				int line = temp->line;
				table_insert(a, line, 1,basic_type,instruct);
			}
			else if(node->child[0]->type == VarDec)
			{
				struct Node *temp = node;
				while(temp->child[0]->type != eID)
					temp = temp->child[0];
				char a[64];
				memset(a,0,sizeof(a));
				strcpy(a,temp->child[0]->ID);
				int line = temp->child[0]->line;
				table_insert(a,line,2,basic_type,instruct);
	
				temp = temp->father;
				//printf("-%d \n",temp->child[2]->intvalue);
				int s;
				s = temp->child[2]->intvalue;
				//printf("----%d\n",s);
				
				tablehead->array_next = add_array(basic_type,s);
				
				
				struct array_node *temp1 = tablehead->array_next;
				
				
				for(temp = temp->father;temp->child[2] != NULL;temp = temp->father)
				{
					if(temp->child[2]->type == eINT)
					{
						int size = temp->child[2]->intvalue;
						temp1->next = add_array(basic_type,size);
						temp1 = temp1->next;
					}
				}
				
			}
		}
		else
		{
			int i = 0;
			for(; i < CHILD_NUM ; i ++)
			{
				add_id(basic_type,node->child[i],instruct);
			}
		}
	}
}
void add_deflist(struct Node *node,int instruct)//Deflist add to table
{
	struct Node* temp = node;
	//printf("%s\n",temp->name);
	for(; temp->child[1] != NULL; temp = temp->child[1])
	{
		
		struct Node *temp1 = temp->child[0]->child[1];
		struct Node *type = temp->child[0]->child[0]->child[0];
		//printf("%s\n",temp1->name);
		for(;temp1 != NULL ; temp1 = temp1->child[2])
		{
			
			struct Node *temp2 = temp1->child[0];
			
			if(temp2->child[1] != NULL && isfunc < 0)
			{
				synerror = 1;
				printf("Error type 15 at line %d: Can\'t assign a variable in struct\n",temp2->child[1]->line);
			}
			struct Node *t = temp2;
			temp2 = temp2->child[0];
			if(type->type == eTYPE)
			{
				//printf("%s\n",temp2->name);
				int basic_type = type->iftype;
				add_id(basic_type,temp2,instruct);				
			}
			else
			{
				//printf("%s\n",temp2->child[0]->ID);
				add_id(3,temp2,instruct);
				tablehead->struct_next = set_structinfo(type);
				
				//printf("===\n");
			}
			
			if(t->child[1] != NULL && isfunc >= 0)
			{
				struct table_node *t1 = tablehead;
				struct table_node *t2 = analysis_exp(t->child[2]);
				if(t1->type != 2 && t2->type != 2 && t1->basic_type == t2->basic_type)
				{
					int a = struct_equal(t1->struct_next,t2->struct_next);
					if(a != 1)
					{
						synerror = 1;
						printf("Error type 5 at line %d: Type mismatched\n",t->child[1]->line);
					}				
				}
				else
				{
					synerror = 1;
					printf("Error type 5 at line %d: Type mismatched\n",t->child[1]->line);
				}
			}
			
		}
		if(type->type == StructSpecifier)
			add_struct_type(type);
	}
}
void add_struct_type(struct Node *node)//StructSpecifier
{
	//printf("--%s\n",node->name);
	if(node->child[1]->type == Tag)
	{
		
		char a[64];
		memset(a,0,sizeof(a));
		strcpy(a,node->child[1]->child[0]->ID);
		struct struct_node *t = check(a);
		if(t == NULL)
		{
			synerror = 1;
			printf("Error type 17 at line %d: Undefined struct \'%s\'\n",node->child[1]->line,a);
		}
		//printf("asdasdasd\n");
		//list_insert(a);
	}
	else if(node->child[1]->type == OptTag)
	{
		struct Node *temp = node;
		if(node->child[1]->child[0] != NULL)
		{
			//printf("111\n");	
			char a[64];
			memset(a,0,sizeof(a));
			strcpy(a,node->child[1]->child[0]->ID);
			list_insert(a,temp->line);
			struct_list_head->info_next = set_structinfo(temp);
		}
		temp = temp->child[3];
		isfunc --;
		add_deflist(temp,1);
		isfunc ++;		
	}
}
void analysis(struct Node *head)
{
	if(head != NULL)
	{
		
		if(head->type == ExtDef)
		{
			if(head->child[1]->type == ExtDecList)
			{
				struct Node *temp = head;
				if(temp->child[0]->child[0]->type == eTYPE)
				{
					int basic_type = temp->child[0]->child[0]->iftype;
					add_id(basic_type, temp,-1);
					
				}
				else if(temp->child[0]->child[0]->type == StructSpecifier)
				{
					temp = temp->child[1];
					struct table_node *p = tablehead;
					int num1 = 0;
					for(;p != NULL ; p = p->next)
						num1 ++;
					add_id(3, temp,-1);
					temp = head->child[0]->child[0];
					//printf("%s",temp->name);
					
					p = tablehead;
					int num2 = 0;
					for(;p != NULL ; p = p->next)
						num2 ++;
					p = tablehead;
					int i;
					for(i = 0; i < num2 - num1; i ++)
					{
						if(p->basic_type == 3 && p->struct_next == NULL)
							p->struct_next = set_structinfo(temp);
						p = p->next;
					}
					add_struct_type(temp);
				}
			//	printf("1\n");
			}
			else if(head->child[1]->type == eSEMI)
			{
				struct Node *temp = head->child[0];
				if(temp->child[0]->type == StructSpecifier)
				{
					temp = temp->child[0];
					add_struct_type(temp);
				}
			//	printf("2\n");
			}
			else if(head->child[1]->type == FunDec)
			{
				struct Node *temp0 = head->child[0];
				struct Node *temp1 = head->child[1];
				struct Node *temp2 = head->child[2];
				int basic_type;
				if(temp0->child[0]->type == eTYPE)
					basic_type = temp0->child[0]->iftype;
				else
					basic_type = 3;
				char a[64];
				memset(a,0,sizeof(a));
				strcpy(a,temp1->child[0]->ID);
				int line = head->line;
				table_insert(a,line,3,basic_type,-1);
				temp0 = temp0->child[0];
				if(basic_type == 3)
				{					
					tablehead->struct_next = set_structinfo(temp0);
					add_struct_type(temp0);
				}
				struct table_node *func = tablehead;
				struct func_node *head = NULL;
				struct func_node *tail = NULL;
				struct func_node *p = NULL;
				if(temp1->child[2]->type == VarList)
				{
					for(temp1 = temp1->child[2]; temp1 != NULL ; temp1 = temp1->child[2])
					{
						
						struct Node *node = temp1->child[0];
						int basic_type;
						//printf("%s\n",node->name);
						if(node->child[0]->child[0]->type == eTYPE)
							basic_type = node->child[0]->child[0]->iftype;
						else
							basic_type = 3;
						//printf("%d  %d\n",basic_type,node->child[0]->child[0]->iftype);
						add_id(basic_type,node->child[1],-1);
						if(basic_type == 3)
							tablehead->struct_next = set_structinfo(node->child[0]->child[0]);
						
						int type = 1;
						if(tablehead->array_next != NULL)
							type = 2;
						p =  add_func(basic_type,type);
						if(basic_type == 3)
							p->struct_next = tablehead->struct_next;
						if(type = 2)
							p->array_next = tablehead->array_next;
						if(head == NULL)
						{
							head = p;
							tail = p;
						}
						else
						{
							tail->next = p;
							tail = p;
						}
					}
				}
				func->func_next = head;
				add_deflist(temp2->child[1],-1);
				temp2 = temp2->child[2];
				func = table_check(a,3);
				//printf("%s\n",func->name);
				analysis_stmtlist(temp2 , func);
			}
			else
			{
				printf("syntax tree is error!!\n");
				return;
			}
		}
		int i = 0;
		for(;i < CHILD_NUM ; i ++)
		{
			analysis(head->child[i]);
		}
	}
}
	

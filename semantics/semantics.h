#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../tree/tree.h"
#include "../table/table.h"
struct struct_list
{
	char name[32];
	struct struct_node *info_next;
	struct struct_list *next;
};
extern int synerror;
extern int isfunc;//to judge func or struct
extern struct struct_list *struct_list_head;
struct struct_node *set_structinfo(struct Node *node);
void analysis_stmtlist(struct Node *node, struct table_node *func);
void analysis_stmt(struct Node *node, struct table_node *func);
struct table_node *analysis_exp(struct Node *node);
void list_insert(char *name,int);
void add_deflist(struct Node *node,int);
void add_id(int,struct Node *node,int);
void analysis(struct Node *head);
void add_struct_type(struct Node *node);
struct struct_node *check(char *name);

#endif

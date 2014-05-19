#ifndef __TABLE_H__
#define __TABLE_H__
#include <stdbool.h>
#include <stdlib.h>
#include "../tree/tree.h"
struct table_node
{
	char name[32];
	int line;
	int type;		// 1: basictype(int float struct)
				// 2: array
				// 3: function
	int basic_type; 	// 1:int	2: float	3:struct
	struct table_node *next;
	struct func_node *func_next;
	struct array_node *array_next;
	struct struct_node *struct_next;
};
struct func_node
{
	int type;		// 1: basictype(int float struct)
				// 2: array
	int basic_type; 	// 1:int	2: float	3:struct
	struct array_node *array_next;
	struct struct_node *struct_next;
	struct func_node *next;
};
struct array_node
{
	int type; 	// 1:int	2: float	3:struct
	int size;
	struct array_node *next;
};
struct struct_node 
{
	char name[32];
	int line;
	int type;		// 1: basictype(int float struct)
				// 2: array
	int basic_type; 	// 1:int	2: float	3:struct
	struct array_node *array_next;
	struct struct_node *struct_next;
	struct struct_node *next;	
};
int exist(char *name);
extern struct table_node *tablehead;
void table_insert(char *name, int, int, int,int);
struct func_node* add_func(int,int);
struct array_node* add_array(int,int);
struct struct_node* add_struct(char *name ,int ,int ,int);
void table_delete(struct table_node *node);
struct table_node *table_check(char *name,int type);
int struct_equal(struct struct_node *node1,struct struct_node *node2);




#endif

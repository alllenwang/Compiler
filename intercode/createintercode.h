#ifndef __CREATEINTERCODE_H__
#define __CREATEINTERCODE_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../table/table.h"
#include "../tree/tree.h"
#include "intercode.h"
void createintercode(struct Node *head);
InterCode *translate_exp(struct Node *node,Operand *op);
InterCode *translate_stmt(struct Node *node);
InterCode *translate_cond(struct Node *node, int, int);
InterCode *translate_compst(struct Node *node);
InterCode *translate_def(struct Node *node);
InterCode *translate_args(struct Node *node);
#endif

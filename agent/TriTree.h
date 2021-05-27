#include <cstdio>
#include <stdlib.h>
#include <cstring>
#include <iostream>
 
using namespace std;

//三叉树节点类型定义
typedef struct node
{
    char tag[10];//标签 表示的内容
    int id;     //编号 用于构建OID
    char datatype[10];     //数据类型
	struct node *lchild;
	struct node *mchild;
	struct node *rchild;
}TriTreeNode, *TriTree;

void PreOrder(TriTreeNode* node) ;

void InsertTree(TriTree &parent,int location,char tag[],int id,char datatype[]);

char *OID2TAG(TriTreeNode* node,char oid[]);

char *search(TriTreeNode* node,int OID[]);

void createTree();
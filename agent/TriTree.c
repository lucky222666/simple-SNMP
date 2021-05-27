#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TriTree.h"
#define MAX_LINE 1024

//一维数组，用于存放请求的OID
int OID[3];
TriTree root = new TriTreeNode;

//为创建树设置的变量
char tag[10];           //标签 表示的内容
int id;                 //编号 用于构建OID
char datatype[10];      //数据类型

int start=0;    //用于记录是否是树根
int count=0;    //用于记录分割一句话的字符串下标
int yitiao=0;   //组成一条完整的数据，插入树中
char parent_name[10];

//先序遍历 
void PreOrder(TriTreeNode* node)   
{
	cout << node->tag << " "<<node->id<<" "<<node->datatype<<endl;
	if (node->lchild != NULL) PreOrder(node->lchild);
	if (node->mchild != NULL) PreOrder(node->mchild);
	if (node->rchild != NULL) PreOrder(node->rchild);
	return;
}


//往树形结构中插入节点
void InsertTree(TriTree &parent,int location,char tag[],int id,char datatype[])
{
    TriTree node = (TriTreeNode *) calloc(1, sizeof(TriTreeNode));

    if(node == NULL) {
        printf("calloc error\n");
        return;
    }

	strcpy(node->tag,tag);
    node->id=id;
    strcpy(node->datatype,datatype);
    node->lchild=NULL;
    node->mchild=NULL;
    node->rchild=NULL;
 
    switch(location)
    {
        case 0:
            parent=node;
            break;
        case 1:
            parent->lchild=node;
            break;
        case 2:
            parent->mchild=node;
            break;
        case 3:
            parent->rchild=node;
            break;
    }
}

//根据OID获取想要获得的数据的关键字 叶子节点级别
char* OID2TAG(TriTreeNode* node,char oid[])
{
    char *s = oid;
    char *delim = ".";
    char *p;
    int i=0;
    OID[i]=atoi(strtok(s,delim));
    while((p = strtok(NULL, delim)))
    {
        i++;
        OID[i]=atoi(p);
    }
    //根据OID得到
    return search(node,OID);   
}

char *search(TriTreeNode* node,int OID[])
{
    
    if(OID[0]==1)
    {
        switch(OID[1])
        {
            case 1:
                node=node->lchild;
                if(OID[2]==1)
                    node=node->lchild;
                else
                    return NULL;
                break;
            case 2:
                node=node->mchild;
                if(OID[2]==1)
                    node=node->lchild;
                else if(OID[2]==2)
                    node=node->mchild;
                else if(OID[2]==3)
                    node=node->rchild;
                else    
                    return NULL;
                break;
            case 3:
                node=node->rchild;
                if(OID[2]==1)
                    node=node->lchild;
                else if(OID[2]==2)
                    node=node->mchild;
                else    
                    return NULL;
                break;
        }
        return node->tag;
    }
    else
        return NULL;
    
}

void createTree()
{
    char buf[MAX_LINE];  /*缓冲区*/
    FILE *fp;            /*文件指针*/
    int len;             /*行字符个数*/
    if((fp = fopen("ASN1.txt","r")) == NULL)
    {
        perror("fail to read");
        exit (1) ;
    }
    
    //按行读取文件内容
    while(fgets(buf,MAX_LINE,fp) != NULL)
    {
        len = strlen(buf);
        buf[len-1] = '\0';  /*去掉换行符*/

        //匹配关键字
        char *c1=strstr(buf,"OBJECT IDENTIFIER");
        char *c2=strstr(buf,"OBJECT-TYPE");
        char *c3=strstr(buf, "SYNTAX");
        char *c4=strstr(buf,"::= {");

        char *p=NULL;   //用于分割字符串的全局指针

        if(c1)//说明是一级节点或二级节点
        {
            //说明是一级节点
            if(!start)
            {
                start++;
                p = strtok(buf, " ");

                if(p){
                    memset(tag,0,sizeof(tag));
                    strcpy(tag,p);
                }

                count=1;
                while(p=strtok(NULL, " ")){//使用第一个参数为NULL来提取子串
                    count++;
                    if(count==7)
                    { 
                        id=atoi(p);
                        break;
                    }
                }
                InsertTree(root,0,tag,id,"NONE");
            }
            //说明是二级节点
            else
            {
                p = strtok(buf, " ");

                if(p){
                    memset(tag,0,sizeof(tag));
                    strcpy(tag,p);
                }

                count=1;
                while(p=strtok(NULL, " ")){//使用第一个参数为NULL来提取子串
                    count++;
                    if(count==7)
                    { 
                        id=atoi(p);
                        break;
                    }
                }
                InsertTree(root,id,tag,id,"NONE");
            }
        }

        //创建树的二级节点,以便三级节点插入
        TriTree root_lchild = root->lchild;
        TriTree root_mchild = root->mchild;
        TriTree root_rchild = root->rchild;

        if(c2 || c3 || c4)//说明是三级节点
        {
            //获取叶子节点的标签 即名称
            if(c2)
            {
                yitiao=0;
                p = strtok(buf, " ");

                if(p){
                    memset(tag,0,sizeof(tag));
                    strcpy(tag,p);
                    yitiao++;
                }
            }
            //获取叶子节点的ID 即编号
            else if(c3)
            {
                p = strtok(buf, " ");

                if(p);//跳过SYNTAX

                count=1;
                while(p=strtok(NULL, " ")){//使用第一个参数为NULL来提取子串
                    count++;
                    if(count==2)
                    { 
                        strcpy(datatype,p);
                    }
                }
                yitiao++;
            }
            //获取叶子节点的类型 用于BER编码
            else if(c4)
            {
                //printf("%s\n",buf);
                p = strtok(buf, " ");

                if(p)
                    ;

                count=1;
                while(p=strtok(NULL, " ")){//使用第一个参数为NULL来提取子串
                    count++;
                    if(count==3)
                        strcpy(parent_name,p);
                    if(count==4)
                    {
                        char tmp=*p;
                        if(tmp=='}');
                        else   id=atoi(p);
                        break; 
                    }
                } 
                yitiao++;
            }
        }
        //插入叶子节点
        if(yitiao==3 && datatype)
        {
            if(strcmp(tag,"sysname")==0)
                InsertTree(root_lchild,1,tag,id,datatype);
            else if(strcmp(tag,"cpu_5")==0)
                InsertTree(root_mchild,1,tag,id,datatype);
            else if(strcmp(tag,"cpu_10")==0)     
                InsertTree(root_mchild,2,tag,id,datatype);
            else if(strcmp(tag,"cpu_30")==0)
                InsertTree(root_mchild,3,tag,id,datatype);
            else if(strcmp(tag,"memtotal")==0)
                InsertTree(root_rchild,1,tag,id,datatype);
            else if(strcmp(tag,"memfree")==0)
                InsertTree(root_rchild,2,tag,id,datatype);
        }
    }

    //PreOrder(root);
}


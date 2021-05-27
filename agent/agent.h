#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include"TriTree.h"

extern TriTree root ;

typedef struct MEM_PACKED         //定义一个mem occupy的结构体
{
    char name[20];      //定义一个char类型的数组名name有20个元素
    int data;
    char name2[20];
}MEM_OCCUPY;
 
 
typedef struct MEM_PACK         //定义一个mem occupy的结构体
{
    int memtotal;
    int memfree;
}MEM_PACK;

void get_hostname();
void get_cpuinfo();
void get_meminfo();
int  analysis_request(char recv_buf[],char send_buf[]);
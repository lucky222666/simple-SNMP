#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<libmemcached/memcached.h>

int memcached_cy(char *keys_argv[],char *values_argv[]);
void Mysql_Insert(char recv_buf_[],int count,int choose);
int Myfile_Write(char recv_buf_[]);
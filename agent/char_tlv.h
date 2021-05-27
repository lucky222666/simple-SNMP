#include<stdio.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#define MAX 1024


void itoa (int n,char s[]);
void encode(int fileno,char char_buf[],char tlv_buf[]);
void decode(int fileno,char tlv_buf[],char char_buf[]);

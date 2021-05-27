#include"char_tlv.h"


//unsigned char transform to TLV
void encode(int fileno,char char_buf[],char tlv_buf[])
{
    char tlv_type[MAX]={0};
    char tlv_len[MAX]={0};
    // char tlv_buf[MAX] 是TLV的集合

    //获取sysname : IA5String  22 十六进制转换无法解析，于是考虑十进制
    if(fileno==1)
    {
        //tlv_buf[0]=0x00&0x16;
        itoa(22,tlv_type);
        strcat(tlv_buf,tlv_type);
        strcat(tlv_buf," ");
        itoa(strlen(char_buf),tlv_len);
        strcat(tlv_buf,tlv_len);
        strcat(tlv_buf," ");
        strcat(tlv_buf,char_buf);
    }
    //获取cpu_5 cpu_10 cpu_30 : REAL
    else if(fileno==2)
    {
        char *a,*b,*c,*p;
        p=NULL;
        int len=0;
        int i=1;
        //只取前三项
        p = strtok(char_buf, " ");   
        while(p)
        {
            if(i==1)    a = p;
            else if(i==2) b=p;
            else if(i==3) c=p;

            ++i;
            // 再次调用分割时指针要变为NULL, 也就是这里的第一个参数，分割的字符串还是str
            // 第二个参数要和第一次调用时的分割符保持一致
            p = strtok(NULL, " ");
        }            
        itoa(9,tlv_type);
        strcat(tlv_buf,tlv_type);
        strcat(tlv_buf," ");
        itoa(strlen(a),tlv_len);
        strcat(tlv_buf,tlv_len);
        strcat(tlv_buf," ");
        strcat(tlv_buf,a);
        strcat(tlv_buf," ");

        itoa(9,tlv_type);
        strcat(tlv_buf,tlv_type);
        strcat(tlv_buf," ");
        itoa(strlen(b),tlv_len);
        strcat(tlv_buf,tlv_len);
        strcat(tlv_buf," ");
        strcat(tlv_buf,b);
        strcat(tlv_buf," ");

        itoa(9,tlv_type);
        strcat(tlv_buf,tlv_type);
        strcat(tlv_buf," ");
        itoa(strlen(c),tlv_len);
        strcat(tlv_buf,tlv_len);
        strcat(tlv_buf," ");
        strcat(tlv_buf,c);

        //printf("%s\n",tlv_buf);
    }
    //获取memtotal memfree : INTEGER
    else if(fileno==3)
    {
        itoa(2,tlv_type);
        strcat(tlv_buf,tlv_type);
        strcat(tlv_buf," ");
        itoa(strlen(char_buf),tlv_len);
        strcat(tlv_buf,tlv_len);
        strcat(tlv_buf," ");
        strcat(tlv_buf,char_buf);
    }
}

//TLV transform to unsigned char
void decode(int fileno,char tlv_buf[],char char_buf[])
{
    char *p;
    char unit[50];
    p=NULL;
    int i=1;

    //拆分出sysname 信息
    if(fileno==1)
    {
        p = strtok(tlv_buf, " ");   
        while(p)
        {
            if(i%3==1)
            {
                sprintf(unit,"T:%s ",p);
                strcat(char_buf,unit);
                memset(unit,0,50);
            }
            else if(i%3==2)
            {
                sprintf(unit,"L:%s ",p);
                strcat(char_buf,unit);
                memset(unit,0,50);
            }
            else if(i%3==0)
            {
                sprintf(unit,"V:%s ",p);
                strcat(char_buf,unit);
                memset(unit,0,50);
            }
            ++i;
            p = strtok(NULL, " ");
        } 
        //printf("%s\n",char_buf);     
    }
    else if(fileno==2)
    {
        p = strtok(tlv_buf, " ");   
        while(p)
        {
            if(i%3==1)
            {
                sprintf(unit,"T:%s ",p);
                strcat(char_buf,unit);
                memset(unit,0,50);
            }
            else if(i%3==2)
            {
                sprintf(unit,"L:%s ",p);
                strcat(char_buf,unit);
                memset(unit,0,50);
            }
            else if(i%3==0)
            {
                sprintf(unit,"V:%s ",p);
                strcat(char_buf,unit);
                memset(unit,0,50);
            }
            ++i;
            p = strtok(NULL, " ");
        }  
    }
    else if(fileno==3)
    {
        p = strtok(tlv_buf, " ");   
        while(p)
        {
            if(i==1)
            {
                sprintf(unit,"T:%s ",p);
                strcat(char_buf,unit);
                memset(unit,0,50);
            }
            else if(i==2)
            {
                sprintf(unit,"L:%s V:",p);
                strcat(char_buf,unit);
                memset(unit,0,50);
            }
            else 
            {
                sprintf(unit,"%s ",p);
                strcat(char_buf,unit);
                memset(unit,0,50);
            }
            ++i;
            p = strtok(NULL, " ");
        }  
    }


}


//辅助函数
void itoa (int n,char s[])
{
    int i,j,sign;
    if((sign=n)<0)//记录符号
    n=-n;//使n成为正数
    i=0;
    do{
        s[i++]=n%10+'0';//取下一个数字
    }
    while ((n/=10)>0);//删除该数字
    if(sign<0)
    s[i++]='-';
    s[i]='\0';
} 
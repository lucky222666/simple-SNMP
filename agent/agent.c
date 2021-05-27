#include"agent.h"

void get_hostname()
{
    system("uname -n > data1.txt");
}


void get_cpuinfo()
{
    system("head -1 /proc/loadavg > data2.txt");
}


void get_meminfo ()    // get RAM message
{
    system("head -2 /proc/meminfo > data3.txt");
}

int analysis_request(char recv_buf[],char send_buf[])
{
    createTree();
    //确定采集的数据对象
    strcpy(send_buf,OID2TAG(root,recv_buf));

    if(strcmp(send_buf,"sysname")==0)
    {
        get_hostname();
        return 1;
    }
    else if(!strcmp(send_buf,"cpu_5") || !strcmp(send_buf,"cpu_10") || !strcmp(send_buf,"cpu_30"))
    {
        get_cpuinfo();
        return 2;
    }
    else if(!strcmp(send_buf,"memtotal") || !strcmp(send_buf,"memfree"))
    {
        get_meminfo();
        return 3;
    }
    //开始采集数据到对应文本
}



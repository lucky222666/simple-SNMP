#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "char_tlv.h"
#include "memcache.h"


#define DEST_PORT 8000
#define DSET_IP_ADDRESS  "127.0.0.1"
#define MAX 1024

int requestno=0;
int j=0;    //用于计数位置 
char *p;    //用于分割字符串
char *keys[]={"cpu_5","cpu_10","cpu_30"}; //用于存储到memcache中的全局变量
char *values[]={"V1","V2","V3"};
int count;  //  用于记录插入数据库的主键

int main()
{
  /* socket文件描述符 */
  int sock_fd;

  /* 建立udp socket */
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock_fd < 0)
  {
    perror("socket");
    exit(1);
  }

  /* 设置address */
  struct sockaddr_in addr_serv;
  int len;
  memset(&addr_serv, 0, sizeof(addr_serv));
  addr_serv.sin_family = AF_INET;
  addr_serv.sin_addr.s_addr = inet_addr(DSET_IP_ADDRESS);
  addr_serv.sin_port = htons(DEST_PORT);
  len = sizeof(addr_serv);


  int send_num;
  int recv_num;
  char send_buf[MAX] ;
  char recv_buf[MAX];
  char recv_buf_[MAX];

  printf("Please input the OID of object you want to get:");
  scanf("%s",send_buf);

  if(!strcmp(send_buf,"1.1.1"))
    requestno=1;
  else if(!strcmp(send_buf,"1.2.1") || !strcmp(send_buf,"1.2.2") || !strcmp(send_buf,"1.2.3"))
    requestno=2;
  else if(!strcmp(send_buf,"1.3.1") || !strcmp(send_buf,"1.3.2"))
    requestno=3;

  send_num = sendto(sock_fd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&addr_serv, len);

  if(send_num < 0)
  {
    perror("sendto error:");
    exit(1);
  }


  switch(requestno)
  {
    case 1:
        //读取sysinfo
        recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_serv, (socklen_t *)&len);
        if(recv_num < 0)
        {
          perror("recvfrom error:");
          exit(1);
        }

        recv_buf[recv_num] = '\0';
        decode(1,recv_buf,recv_buf_);
        printf("%s\n",recv_buf_);
        //sysinfo 写入文件
        p=NULL;
        p = strtok(recv_buf_, " ");   
        while(p)
        {
            if(j==2)
            {
              FILE *fp;
 
              fp = fopen( "sysinfo.txt" , "w" );
              fwrite(p, 10 , 1, fp );
      
              fclose(fp);
            }
            ++j;
            p = strtok(NULL, " ");
        }             
        memset(recv_buf_,0,sizeof(recv_buf_));
        break;
    case 2:
        //读取cpuinfo
        recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_serv, (socklen_t *)&len);
        if(recv_num < 0)
        {
          perror("recvfrom error:");
          exit(1);
        }

        recv_buf[recv_num] = '\0';
        decode(1,recv_buf,recv_buf_);
        printf("%s\n",recv_buf_);
        printf("\n");
        //cpuinfo写入libmemcached
        p=NULL;
        j=1;
        p = strtok(recv_buf_, " ");   
        while(p)
        {
            if(j==3) values[0]=p;
            else if(j==6) values[1]=p;
            else if(j==9) values[2]=p;
            ++j;
            p = strtok(NULL, " ");
        }        
        //printf("%s %s %s\n",values[0],values[1],values[2]);
        memcached_cy(keys,values);
        memset(recv_buf_,0,sizeof(recv_buf_));
        break;
    case 3:
        //读取memtotal 并入库
        recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_serv, (socklen_t *)&len);
        if(recv_num < 0)
        {
          perror("recvfrom error:");
          exit(1);
        }

        recv_buf[recv_num] = '\0';
        decode(3,recv_buf,recv_buf_);
        printf("%s\n",recv_buf_);
        //Mysql_Insert(recv_buf_,count,1);
        Myfile_Write(recv_buf_);
        memset(recv_buf_,0,sizeof(recv_buf_));
        //读取memfree 并入库
        recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_serv, (socklen_t *)&len);
        if(recv_num < 0)
        {
          perror("recvfrom error:");
          exit(1);
        }
          
        recv_buf[recv_num] = '\0';
        decode(3,recv_buf,recv_buf_);
        printf("%s\n",recv_buf_);
        //Mysql_Insert(recv_buf_,count,2);
        Myfile_Write(recv_buf_);
        memset(recv_buf_,0,sizeof(recv_buf_));
        count++;
        break;
    default:
        break;
  }


  close(sock_fd);

  return 0;
}
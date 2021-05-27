#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "agent.h"
#include "char_tlv.h"

#define SERV_PORT   8000
#define MAX 1024


int main()
{
  /* sock_fd --- socket文件描述符 创建udp套接字*/
  int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock_fd < 0)
  {
    perror("socket");
    exit(1);
  }

  /* 将套接字和IP、端口绑定 */
  struct sockaddr_in addr_serv;
  int len;
  bzero(&addr_serv,sizeof(addr_serv));  //每个字节都用0填充
  
  addr_serv.sin_family      = AF_INET;
  addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);
  addr_serv.sin_port        = htons(SERV_PORT);
  
  len = sizeof(addr_serv);

  /* 绑定socket */
  if(bind(sock_fd, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0)
  {
    perror("bind error:");
    exit(1);
  }


  int recv_num;
  int send_num;
  FILE *fp;
  int fileno;
  char send_buf[MAX];
  char send_buf_[MAX];
  char recv_buf[MAX];
  struct sockaddr_in addr_client;


  while(1)      //为了提交效率，可以进一步使用多进程 或者 多线程
  {
    printf("server wait:\n");

    recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_client, (socklen_t *)&len);

    if(recv_num < 0)
    {
      perror("recvfrom error:");
      exit(1);
    }

    recv_buf[recv_num] = '\0';
    printf("server receive request: %s\n",recv_buf);

    //收到Manager请求，执行响应函数 采集数据&返回
    fileno=analysis_request(recv_buf,send_buf);
    switch(fileno)
    {
        case 1:
            fp = fopen("data1.txt", "r"); // 创建文件指针及打开文本文件

            if (fp == NULL)
            {
                printf("文件打开时发生错误");
                exit(1);
            }

            while(fgets(send_buf,MAX,fp) != NULL){
                encode(1,send_buf,send_buf_);   //TLV编码之后放在send_buf_缓冲区中
                //printf("%s\n",send_buf_);
                memset(send_buf,0,sizeof(send_buf));

                send_num = sendto(sock_fd, send_buf_, MAX, 0, (struct sockaddr *)&addr_client, len);

                if(send_num < 0)
                {
                    perror("sendto error:");
                    exit(1);
                }
                memset(send_buf_,0,sizeof(send_buf_));
            }
            break;
        case 2:
            fp = fopen("data2.txt", "r"); // 创建文件指针及打开文本文件

            if (fp == NULL)
            {
                printf("文件打开时发生错误");
                exit(1);
            }

            while(fgets(send_buf,MAX,fp) != NULL){
                encode(2,send_buf,send_buf_);   //TLV编码之后放在send_buf_缓冲区中
                //printf("%s\n",send_buf_);
                memset(send_buf,0,sizeof(send_buf));

                send_num = sendto(sock_fd, send_buf_, MAX, 0, (struct sockaddr *)&addr_client, len);

                if(send_num < 0)
                {
                    perror("sendto error:");
                    exit(1);
                }
                memset(send_buf_,0,sizeof(send_buf_));
            }
            break;    
        case 3:
            fp = fopen("data3.txt", "r"); // 创建文件指针及打开文本文件

            if (fp == NULL)
            {
                printf("文件打开时发生错误");
                exit(1);
            }

            while(fgets(send_buf,MAX,fp) != NULL){
                encode(3,send_buf,send_buf_);   //TLV编码之后放在send_buf_缓冲区中
                //printf("%s\n",send_buf_);
                memset(send_buf,0,sizeof(send_buf));

                send_num = sendto(sock_fd, send_buf_, MAX, 0, (struct sockaddr *)&addr_client, len);

                if(send_num < 0)
                {
                    perror("sendto error:");
                    exit(1);
                }
                memset(send_buf_,0,sizeof(send_buf_));
            }
            break;    
        default:
            fclose(fp); // 关闭文件指针
            break;
    }

    
  }

  close(sock_fd);

  return 0;
}
#include"memcache.h"
#include <mysql/mysql.h>
#include<string.h>


int memcached_cy(char *keys_argv[],char *values_argv[])
{
	memcached_server_st *servers=NULL;
	memcached_st *memc;
	memcached_return rc;
	char *keys[]={"huey","dewey","louie"};
	keys[0]=keys_argv[0];
	keys[1]=keys_argv[1];
	keys[2]=keys_argv[2];

	size_t key_length[3];
	char *values[]={"red","blue","green"};
	values[0]=values_argv[0];
	values[1]=values_argv[1];
	values[2]=values_argv[2];

	size_t value_length[3];
	unsigned int x;
	uint32_t flags;

	char return_key[MEMCACHED_MAX_KEY];
	size_t return_key_length;
	char *return_value;
	size_t return_value_length;

	memc=memcached_create(NULL);

	servers=memcached_server_list_append(servers,"localhost",113113,&rc);
	rc=memcached_server_push(memc,servers);

	if(rc==MEMCACHED_SUCCESS)
		fprintf(stderr,"Added server successfully\n");
	else
		fprintf(stderr,"Counld nor add server:%s\n",memcached_strerror(memc,rc));

	for(x=0;x<3;x++)
	{
		key_length[x]=strlen(keys[x]);
		value_length[x]=strlen(values[x]);

		rc=memcached_set(memc,keys[x],key_length[x],values[x],value_length[x],(time_t)0,(uint32_t)0);
		if(rc==MEMCACHED_SUCCESS)
			fprintf(stderr,"Key %s store successfully\n",keys[x]);
		else
			fprintf(stderr,"Could not store key:%s\n",memcached_strerror(memc,rc));
	}

	rc=memcached_mget(memc,keys,key_length,3);

	if(rc==MEMCACHED_SUCCESS)
	{
		while((return_value=memcached_fetch(memc,return_key,&return_key_length,&return_value_length,&flags,&rc))!=NULL)
		{
			if(rc==MEMCACHED_SUCCESS)
			{
				fprintf(stderr,"Key %s returned %s\n",return_key,return_value);
			}
		}
	}
	return 0;
}


void Mysql_Insert(char recv_buf_[],int count,int choose)
{
	MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *query;  
    int flag, t,id=0;
    mysql_init(&mysql);
    //连接数据库
    if(!mysql_real_connect(&mysql, "localhost", "root", "", "n181002520", 0, NULL, 0)) {
        printf("Failed to connect to Mysql!\n");
        return ;
	}
    // }else {  
    //     printf("Connected to Mysql successfully!\n");  
    // }  

	char *p=NULL;
    int j=1;
    p = strtok(recv_buf_, " ");   
    while(p)
    {
        if(j==4) {break;}   
    	++j;
        p = strtok(NULL, " ");
    }   

	memset(query, 0, 1024);		
	if(choose==1)
	{
		sprintf(query,"insert into meminfo(id,memtotal) values ('%d','%s')",count,p);
		query[strlen(query)]='\0';
		printf("%s\n",query);
	}
	else if(choose==2)
	{
		sprintf(query,"insert into meminfo(id,memfree) values ('%d','%s')",count,p);
		query[strlen(query)]='\0';
		printf("%s\n",query);
	}
    /*插入，成功则返回0*/  
    flag = mysql_real_query(&mysql, query, (unsigned int)strlen(query));  
    if(flag) {  
        printf("Insert data failure!\n");  
        return ;
    }
      
    mysql_close(&mysql);      
}

int Myfile_Write(char recv_buf_[])
{
	char *p=NULL;
    int j=1;
    p = strtok(recv_buf_, " ");   
    while(p)
    {
        if(j==3 || j==4)
        {
            FILE *fp;
 
            fp = fopen( "meminfo.txt" , "a+" );
            fwrite(p, 10 , 1, fp );
      
            fclose(fp);
       }
    	++j;
        p = strtok(NULL, " ");
    }   

}
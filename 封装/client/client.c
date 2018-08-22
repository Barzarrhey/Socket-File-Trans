#include "client.h"

#define BUFLEN 500
#define DATALEN 200

unsigned long get_file_size(const char *path)  
{  
    unsigned long filesize = -1;      
    struct stat statbuff;  
    if(stat(path, &statbuff) < 0){  
        return filesize;  
    }else{  
        filesize = statbuff.st_size;  
    }  
    return filesize;  
} 

void itoa(long i,char*string)
{
	long power,j;
	j=i;
	for(power=1;j>=10;j/=10)
		power*=10;
	for(;power>0;power/=10)
	{
		*string++='0'+i/power;
		i%=power;
	}
	*string='\0';
}

int client_trans_file(char IP[], char port[], char filename[])
{

	//1建立socket套接字
	// int socket(int domain, int type, int protocol);
	int socket_fd=socket(AF_INET, SOCK_STREAM ,0);
	if (socket_fd != -1)
	{
		printf("create sokcet success\n");
	}
	else
		printf("create sokcet failed\n");

	struct sockaddr_in  server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port =  htons(atoi(port));//htons(8888);
	server_addr.sin_addr.s_addr =inet_addr(IP);//inet_addr("192.168.1.212");

	int  crtv = connect(socket_fd, (struct sockaddr *)&server_addr,sizeof(server_addr));
	if (crtv == 0)
	{
		printf("connect server success\n");
	}
	else
	{
		printf("connect server failed\n");
		exit(0);
	}
	/**********************文件数据的发送***************************/
	//1、打开要发送的文件名称
	char file_name[30]={0};
	strcpy(file_name,filename);
	FILE * fp = fopen(file_name,"r");
	if (fp == NULL)
	{
		printf("fopen %s file failed\n",file_name);
		exit(1);
	}
	else
	{
		printf("fopen %s file successed\n",file_name);
	}
	//发送文件名称
	send(socket_fd, file_name, sizeof(file_name), 0);

	//2计算文件大小
	long file_size=get_file_size(file_name); 
	printf("file_size=%ld\n",file_size);
	//发送文件长度
	char file_len[10]={0};
	itoa(file_size,file_len);
	printf("file_len:%s\n",file_len);
	send(socket_fd, file_len, sizeof(file_len),0);

	char date_buf[BUFLEN]={0};
	size_t frtv;
	//分批次读取数据，且每读一次就发送一次
	while(1)
	{
		memset(date_buf, 0, sizeof(date_buf));
		frtv=fread(date_buf,1,BUFLEN,fp);
		if (frtv == 0)
		{
			break;
		}
		send(socket_fd, date_buf,frtv,0);

	}
	

	//关闭
	close(socket_fd);

	return 0;
}

//自己手动按照流程图+man手册形式进行代码的编写
//写一个一对一聊天  线程
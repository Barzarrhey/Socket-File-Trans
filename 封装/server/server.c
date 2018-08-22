#include "server.h"

#define BUFLEN 500
#define DATALEN 200

int server_recv_file(char port[])
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
	server_addr.sin_port =  htons(atoi(port));
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);// 任何可达的地址

	//反复绑定IP地址及端口号
	int reuse = -1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

	//3绑定IP到对应的端口号中
	int brtv = bind(socket_fd, (struct sockaddr *)&server_addr,sizeof(server_addr));
	if (brtv ==0)
	{
		printf("bind ip prot success\n");

	}
	else
		printf("bind ip prot failed\n");
	//4设置 客户端连接最大请求数
	int  lrtv = listen(socket_fd,5);
	if(lrtv ==0)
	{
		printf("lrtv client number success\n");
		printf("/********wait client connect**********/\n");
	}
	else
		printf("lrtv client number failed\n");
	while(1){
		//5等待客户端连接请求  会产生一个阻塞
		// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		struct sockaddr_in  client_addr;
		memset(&client_addr, 0, sizeof(client_addr));
		socklen_t client_addrlen = sizeof(client_addr);
		int new_socketfd = accept(socket_fd,(struct sockaddr *)&client_addr,&client_addrlen);
		if (new_socketfd != -1)
		{
			printf("accpet client success\n");
			printf("client ip: %s\n",inet_ntoa(client_addr.sin_addr));
		}
		else
			printf("accpet client failed\n");

		/**********************文件数据的接受**************************************/
		//接受文件名称，且创建空白的文件名
		char file_name[30]={};
		memset(file_name,'\0',sizeof(30*sizeof(char)));
		while(strlen(file_name) == 0){
			recv(new_socketfd, file_name,sizeof(file_name),0);
		}
		printf("receive filename success\n");
		FILE * fp = fopen(file_name,"w+");
		
		if (fp == NULL)
		{
			printf("fopen %s file failed\n",file_name);
			exit(0);
		}
	
		//接受文件长度，且把字符串转换为整形
		char file_len[10]={0};
		recv(new_socketfd, file_len,sizeof(file_len),0);
		long file_size=atoi(file_len); 
		printf("file_size = %ld\n",file_size);

		//分批次接受数据，每接受一次就写入一次
		char date_buf[BUFLEN]={0};
		size_t fwtv,rrtv,len=0;
		//分批次读取数据，且每读一次就发送一次
		while(1)
		{
			memset(date_buf, 0, sizeof(date_buf));
			rrtv = recv(new_socketfd, date_buf,sizeof(date_buf),0);
			fwtv = fwrite(date_buf, 1, rrtv, fp);
			fflush(fp);// 坑：如果不刷新会导致末尾几个包未写到文件中
			len+=fwtv;
			if (len == file_size)
			{
				break;
			}

		}
		close(new_socketfd);
		printf("file recieve success!\n");
	}
	close(socket_fd);
	return 0;
}


/********************
*坑：
*fwrite[1]函数写到用户空间缓冲区，并未同步到文件中，
*所以修改后要将内存与文件同步可以用fflush（FILE*fp）函数同步。
*
*
*注意：假如定义buffer缓冲区的大小为1024且第二个参数为1时，
*那么fread的第三个参数一定要小于等于1024，否则会出现错误。
*或许大家在学的时候记得比较清楚，实际编程中有可能会忘记。再次特别说明。
*
********************/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>          /* See NOTES */
#include <netinet/in.h>
#include <netinet/ip.h>       
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>



#define BUFLEN 128
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


int main(int argc, char const *argv[])
{
	if(argc < 2)
	{
		printf("Usag: <port> \n");
		exit(1);
	}
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
	server_addr.sin_port =  htons(atoi(argv[1]));//htons(8888);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("192.168.1.212");

	//反复绑定IP地址及端口号
	int reuse = -1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

	//3绑定IP到对应的端口号中
	//int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
	int brtv = bind(socket_fd, (struct sockaddr *)&server_addr,sizeof(server_addr));
	if (brtv ==0)
	{
		printf("bind ip prot success\n");

	}
	else
		printf("bind ip prot failed\n");
	//4设置 客户端连接最大请求数
	//   int listen(int sockfd, int backlog);
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
			len+=fwtv;
			if (len == file_size)
			{
				break;
			}

		}
		close(new_socketfd);
	}
	close(socket_fd);
	return 0;
}
	/*//建立发送信息线程
	pthread_t tid;
	pthread_create(&tid, NULL, sendmessage, &new_socketfd);

	//建立接受信息
	int n;
	char buf[BUFLEN];
	memset(buf, 0, BUFLEN);

	char recvtitle[DATALEN] = "received from client: ";
	sprintf(recvtitle, "%s%s:%d ", recvtitle,inet_ntoa(client_addr.sin_addr) , client_addr.sin_port);
	while(1)
	{
 
		while((n = recv(new_socketfd, buf, BUFLEN, 0)) > 0)
		{
 
			write(STDOUT_FILENO, recvtitle, strlen(recvtitle));
 
			write(STDOUT_FILENO, buf, n);
 	

			memset(buf, 0, BUFLEN);
 
		}
 
		if(n < 0)
 
			printf("recv error\n");
 
	}
 */
	//关闭
//自己手动按照流程图+man手册形式进行代码的编写
//写一个一对一聊天  线程
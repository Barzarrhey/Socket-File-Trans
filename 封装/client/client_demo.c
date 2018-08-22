#include "client.h"
// A demo to use client

int main(){
	char *find;
	int client = -1;
	char IP[20];
	memset(IP,'\0',sizeof(20*sizeof(char)));
	char port[6];
	memset(port,'\0',sizeof(6*sizeof(char)));
	char filename[30]; 
	memset(filename,'\0',sizeof(30*sizeof(char)));
	printf("please input the IP of server:\n");
	fgets(IP,sizeof(IP),stdin);
	find = strchr(IP, '\n');
	if(find != NULL)
		*find = '\0';
	printf("please input the port of server:\n");
	fgets(port,sizeof(port),stdin);
	find = strchr(port, '\n');
	if(find != NULL)
		*find = '\0';
	printf("please input the file name:\n");
	fgets(filename,sizeof(filename),stdin);
	find = strchr(filename, '\n');
	if(find != NULL)
		*find = '\0';
	client = client_trans_file(IP, port, filename);
	if(client == 0){
		printf("File trans success!\n");
		return 0;
	}
	else{
		printf("File trans failed!\n");
		return -1;
	}
}
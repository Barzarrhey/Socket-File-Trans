#include "server.h"
// A demo to use server
int main(){
	char *find;
	int server = -1;
	char port[6];
	memset(port,'\0',sizeof(6*sizeof(char)));
	printf("please input the port of server you want to use:\n");
	fgets(port,6,stdin);
	find = strchr(port, '\n');
	if(find != NULL)
		*find = '\0';
	printf("the server is starting\n");
	server = server_recv_file(port);
	if(server == 0){
		printf("Server close success!\n");
		return 0;
	}
	else{
		printf("Server exit with some error!\n");
		return -1;
	}
	return 0;
}
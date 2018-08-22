#ifndef _SERVER_H
#define _SERVER_H

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

int server_recv_file(char port[]);


#endif
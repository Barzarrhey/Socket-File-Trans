#ifndef _CLIENT_H
#define _CLIENT_H

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


unsigned long get_file_size(const char *path);
void itoa(long i,char*string);
int client_trans_file(char IP[], char port[], char filename[]);

#endif
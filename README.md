#**运行在Linux下的文件传输**

**C语言编写的Socket小程序，客户端传给服务器，服务器可以一直跑，客户端传完自动关闭**

#编译命令

 gcc server.c -o server -lpthread -Wall

 gcc client.c -o client -lpthread -Wall

#运行命令

client  "IP"  "port"  "filename"

server  "port"

**先运行服务器，再运行客户端**



# DEMO

##在封装目录下，里面是造好的轮子

## 编译命令

 gcc client_demo.c client.c -o  client -Wall



 gcc server_demo.c server.c -o  server -Wall

## 运行

./server

./client


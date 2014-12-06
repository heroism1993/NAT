#Makefile 
#
#Copyright (C) Beihang University, 2014
#
#Writen by gaochao(heroism.1993@gmail.com)
#

inc := common/error.h common/common.h common/config.h

server:server/server.o common/error.o server/register.o
	gcc -o serv $(inc) 

server/server.o: server/server.c server/server.h $(inc)

server/register.o: server/register.c server/server.h $(inc)

common/error.o:common/error.c common/error.h

client/client.o:$(inc) client/client.c

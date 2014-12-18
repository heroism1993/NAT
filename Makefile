#Makefile 
#
#Copyright (C) Beihang University, 2014
#
#Writen by gaochao(heroism.1993@gmail.com)
#

all:server client
inc := common/error.h common/common.h common/config.h

FLAG = -pthread
server_obj = server/server.o server/register.o
common_obj = common/error.o common/trie.o
client_obj = client/register.o client/client.o

server/server.o:server/server.c server/server.h
server/register.o:server/register.c
common/error.o:common/error.c common/error.h
common/trie.o:common/trie.c common/trie.h
client/register.o:client/register.c
client/client.o:client/client.h client/client.c

server:$(server_obj) $(common_obj)
	gcc $(FLAG) -o serv $(server_obj) $(common_obj)

client:$(client_obj) $(common_obj)
	gcc $(FLAG) -o cli $(client_obj) $(common_obj)
#server/server.o: server/server.c server/server.h $(inc)

#server/register.o: server/register.c server/server.h $(inc)

#common/error.o:common/error.c common/error.h

#client/client.o:$(inc) client/client.c

clean:
	rm -f common/*.o server/*.o client/*.o cli serv

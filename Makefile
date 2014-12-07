#Makefile 
#
#Copyright (C) Beihang University, 2014
#
#Writen by gaochao(heroism.1993@gmail.com)
#

inc := common/error.h common/common.h common/config.h

server:
	gcc -o serv server/*.h server/*.c common/*.h common/*.c 

client:
	gcc -o cli client/*.h client/*.c common/*.h common/*.c
#server/server.o: server/server.c server/server.h $(inc)

#server/register.o: server/register.c server/server.h $(inc)

#common/error.o:common/error.c common/error.h

#client/client.o:$(inc) client/client.c

clean:
	rm -f common/*.o server/*.o client/*.o

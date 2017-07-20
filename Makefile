#CFLAGS=-g -Wall
CFLAGS=-g
#AIX
#CC=xlc
#LINUX
CC=gcc
LIBS=-Lcgic -lcgic

all: verifica_versao.exe

verifica_versao.exe:
	$(CC) verifica_versao.c -c -o verifica_versao.o
	$(CC) verifica_versao.o cgic/libcgic.a -o verifica_versao.exe

clean:
	rm -f *.o *.a *.exe


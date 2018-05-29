CC=g++
LK_REDIS=-lhiredis
LK_THREAD=-lpthread
CC_EXT=cc
H_EXT=h

KV_D=KV/*.cc
IPC_D=ipc/TCP*.cc
SERIAL_D=serial/*.cc

DEPS=$(KV_D) $(IPC_D) $(SERIAL_D)

client: KV/* ipc/* serial/* 
	$(CC) -o run_client run_client.cc $(DEPS) $(LK_REDIS) $(LK_THREAD)

server:
	$(CC) -o run_server run_server.cc $(DEPS) $(LK_REDIS) $(LK_THREAD)

clean:
	-rm run_client run_server




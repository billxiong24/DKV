CC=g++
SERVER_EX=KVServer
PART_EX=KVPart
RING_EX=KVRing
LK_REDIS=-lhiredis
CC_EXT=cc
H_EXT=h

server: ../ipc/TCP* $(SERVER_EX).$(CC_EXT) $(SERVER_EX).$(H_EXT)
	 g++ -o $(SERVER_EX) $(SERVER_EX).$(CC_EXT) ../ipc/TCP* $(LK_REDIS) -lpthread
ring:
	 g++ -o $(RING_EX) $(RING_EX).$(CC_EXT) $(LK_REDIS)

part: $(SERVER_EX).$(CC_EXT) $(SERVER_EX).$(H_EXT) $(PART_EX).$(CC_EXT) $(PART_EX).$(H_EXT)
	 g++ -o $(PART_EX) $(SERVER_EX).$(CC_EXT) $(PART_EX).$(CC_EXT) $(LK_REDIS)

clean:
	-rm $(PART_EX) $(SERVER_EX)




CC = gcc
MAKE_STATIC_LIB = ar rv
ALLEGRO_FLAGS = -lallegro
JSON_FLAGS = -ljson-c
LIB_FLAGS = -llpthread -lqueue
LIB = cd ./lib &&
RM_O = cd ./lib && rm *.o

.PHONY: all

all: allegro json main
	$(RM_O)

main: liblpthread.a libqueue.a 
	$(CC) -o ./bin/main ./src/main.c -I./include -L./lib $(LIB_FLAGS) $(ALLEGRO_FLAGS) $(JSON_FLAGS)
	
liblpthread.a: lpthread.o
	$(LIB) $(MAKE_STATIC_LIB) liblpthread.a lpthread.o 

lpthread.o: ./lib/lpthread.c
	$(LIB) $(CC) -c lpthread.c -I../include

libqueue.a: queue.o
	$(LIB) $(MAKE_STATIC_LIB) libqueue.a queue.o

queue.o: ./lib/queue.c
	$(LIB) $(CC) -c queue.c

allegro:
	sudo apt-get install liballegro5-dev

json:
	sudo apt-get install libjson-c-dev

#producer: libshmhandler.a
#	$(CC) -o ./bin/producer ./src/producer.c -I./include ./lib/libshmhandler.a -lm -lrt -pthread

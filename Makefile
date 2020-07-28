CC = gcc
MAKE_STATIC_LIB = ar rv
ALLEGRO_FLAGS = -lallegro -lallegro_image -lallegro_primitives
JSON_FLAGS = -ljson-c
LIB_FLAGS = -llpthread -lqueue -llinked_list -lsteque
LIB = cd ./lib &&
RM_O = cd ./lib && rm *.o

.PHONY: all

all: allegro json main main_lthread
	$(RM_O)

main: liblpthread.a libqueue.a liblinked_list.a libsteque.a
	$(CC) -o ./bin/main ./src/main.c ./src/alienLogic.c ./src/bridge.c -I./include -L./lib $(LIB_FLAGS) $(ALLEGRO_FLAGS) $(JSON_FLAGS) -lpthread -lm
	
main_lthread: liblpthread.a libqueue.a liblinked_list.a libsteque.a
	$(CC) -o ./bin/main_lthread ./src/main_lthread.c ./src/alienLogic_lthread.c ./src/bridge.c -I./include -L./lib $(LIB_FLAGS) $(ALLEGRO_FLAGS) $(JSON_FLAGS)
	
liblpthread.a: lpthread.o
	$(LIB) $(MAKE_STATIC_LIB) liblpthread.a lpthread.o 

lpthread.o: ./lib/lpthread.c
	$(LIB) $(CC) -c lpthread.c -I../include

liblinked_list.a: linked_list.o
	$(LIB) $(MAKE_STATIC_LIB) liblinked_list.a linked_list.o

linked_list.o: ./lib/linked_list.c
	$(LIB) $(CC) -c linked_list.c

libqueue.a: queue.o
	$(LIB) $(MAKE_STATIC_LIB) libqueue.a queue.o

queue.o: ./lib/queue.c
	$(LIB) $(CC) -c queue.c

libsteque.a: steque.o
	$(LIB) $(MAKE_STATIC_LIB) libsteque.a steque.o

steque.o: ./lib/steque.c
	$(LIB) $(CC) -c steque.c

allegro:
	sudo apt-get install liballegro5-dev

json:
	sudo apt-get install libjson-c-dev

#producer: libshmhandler.a
#	$(CC) -o ./bin/producer ./src/producer.c -I./include ./lib/libshmhandler.a -lm -lrt -pthread

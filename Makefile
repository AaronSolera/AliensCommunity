CC = gcc
MAKE_STATIC_LIB = ar rv
ALLEGRO_FLAGS = -lallegro
JSON_FLAGS = -ljson-c
LIB = cd ./lib &&
RM_O = cd ./lib && rm *.o

.PHONY: main
	$(RM_O)

main: libqueue.a allegro json 
	$(CC) ./src/main.c $(ALLEGRO_FLAGS) $(JSON_FLAGS) -o ./bin/main -I./include ./lib/libqueue.a

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

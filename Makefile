CC = gcc
MAKE_STATIC_LIB = ar rv
ALLEGRO_FLAGS = -lallegro
LIB = cd ./lib &&
RM_O = cd ./lib && rm *.o

.PHONY: main
#	$(RM_O)

main: allegro
	$(CC) ./src/main.c $(ALLEGRO_FLAGS) -o ./bin/main -I./include

allegro:
	sudo apt-get install liballegro5-dev

#producer: libshmhandler.a
#	$(CC) -o ./bin/producer ./src/producer.c -I./include ./lib/libshmhandler.a -lm -lrt -pthread
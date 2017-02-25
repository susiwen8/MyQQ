all: server client client_1

.PHONY : all

CC = gcc

SOURCE_SERVER = server.c
SOURCE_CLIENT = client.c
OBJECT_SERVER = $(SOURCE_SERVER:.c = .o)
OBJECT_CLIENT = $(SOURCE_CLIENT:.c = .o)

%.o : %.c
	$(CC) -c $^ -o $@

server: $(OBJECT_SERVER) $(SOURCE_SERVER)
	$(CC) $(OBJECT_SERVER) -o server

client: $(OBJECT_CLIENT) $(SOURCE_CLIENT)
	$(CC) $(OBJECT_CLIENT) -o client

client_1: $(OBJECT_CLIENT) $(SOURCE_CLIENT)
	$(CC) $(OBJECT_CLIENT) -o client_1

.PHONY: clean
clean:
	rm server client client_1


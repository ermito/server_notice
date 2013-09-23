CC=gcc
SOURCES=server_notice.c
EXECUTABLE=server_notice
all:
	$(CC)  $(SOURCES) -o $(EXECUTABLE)
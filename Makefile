
CFLAGS=-g -Wall
LIBS=-lm

include Makefile.d

all::

all:: adventure

OBJS=adventure.o area.o commands.o event.o player.o util.o

adventure: $(OBJS)
	$(CC) $(CFLAGS) -o adventure $(OBJS) $(LIBS)

SRCS=adventure.c area.c commands.c event.c player.c util.c
depend Makefile.d:
	$(CC) -MM $(SRCS) |grep : >Makefile.d



adventure.o: adventure.c area.h commands.h player.h util.h
area.o: area.c area.h util.h
commands.o: commands.c commands.h player.h area.h util.h
player.o: player.c player.h area.h util.h
util.o: util.c util.h

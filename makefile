all	: tetris.o
	gcc tetris.c main_screen.c -lncurses

clean:
	rm a.out *.o

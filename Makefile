PacmanF: pacman.o  Main.o  
	gcc pacman.o Main.o -o PacmanF -lncurses -lpthread

pacman.o: pacman.c pacman.h
	gcc -c pacman.c -o pacman.o -lncurses -lpthread

Main.o : Main.c Main.h
	gcc -c Main.c -o Main.o -lncurses -lpthread

Clean:
	rm PacmanF
	rm *.o

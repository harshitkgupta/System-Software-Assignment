tree: main.o node.o
	gcc -o tree main.o node.o
node.o: node.c node.h
	gcc -c node.c
main.o: main.c node.h
	gcc -c main.c		 	
clean:
	rm -rf *.o tree	
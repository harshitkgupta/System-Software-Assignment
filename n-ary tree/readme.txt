Name--Harshit Kumar Gupta
Reg No-EET132329

In my project there are 3 files..
node.h  ->>> Header file for all declarations
node.c  ->>> C file for all function Definitions
main.c  ->>> C file for N ary Tree implementation
Makefile.mak  ->>> makefile for automated process to build excecutable

Compilation Procedure-1 ---
1-  gcc -c node.c
2-  gcc -c main.c
3-  gcc -o tree main.o node.o

or simply we can combine these steps into one step as--
	
	gcc -o tree main.c node.c
		
	
Compilation Procedure-2 ----
 we can automate build process by using make tool.
	
	make -f Makefile.mak
	
for Running the Program type---
	tree	
 
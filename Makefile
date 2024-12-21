all: sed
.PHONY: test clean

prog: main.o lab.o
	gcc -o sed main.o lab.o

main.o: main.c
	gcc -c main.c

lab.o: lab.c
	gcc -c lab.c

test: sed
	./sed input.txt -r "nigger" "white"
	

clean:
	rm -f sed *.o output.txt

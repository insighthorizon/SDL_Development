# Makefile format (targets are just various tasks we call then)
#<target1>:
#<TAB! Important to TAB!>command1
#<TAB>command2
#<TAB>command3
#...
#
#<target2>:
#<TAB>command1
#<TAB>command2
#<TAB>command3
#...

test:
	make build;
	make run;

build:
#compiling into .o file 
	gcc -I"/usr/include/SDL" -c sdltest.c;
#linking into ELF executable binary 
	gcc sdltest.o -lSDL -o sdltest;

run:
	./sdltest;

clean:
	-rm sdltest;
	-rm sdltest.o;

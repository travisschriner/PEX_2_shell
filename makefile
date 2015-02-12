#comment
shell: shell.c shelly.c list.c makefile
	gcc -o shell shell.c shelly.c list.c -Wall 

#shell.o: shell.c list.h shelly.h makefile
#	gcc -c shell.c

#list.o: list.c list.h makefile
#	gcc -c list.c
#shelly.o: shelly.c shelly.h makefile
#	gcc -c shelly.c

clean:
	-rm *.o 
	-rm *~


#PROGRAM = skakutavaLoptica
CC      = gcc
CFLAGS  = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL -lm -lSOIL

skakutavaLoptica: main.o 
	$(CC) $(LDFLAGS) -o skakutavaLoptica.out main.o $(LDLIBS)

$(PROGRAM): main.o
	$(CC) $(LDFLAGS) -c $(PROGRAM) main.c $(LDLIBS)

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)

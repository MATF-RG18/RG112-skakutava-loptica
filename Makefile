#PROGRAM = skakutavaLoptica
CC      = gcc
CFLAGS  = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL -lm 

skakutavaLoptica: main.o image.o
	$(CC) $(LDFLAGS) -o skakutavaLoptica.out main.o image.o $(LDLIBS)

$(PROGRAM): main.o
	$(CC) $(LDFLAGS) -c $(PROGRAM) main.c $(LDLIBS)

$(PROGRAM): image.o
	$(CC) $(LDFLAGS) -c $(PROGRAM) image.c image.h $(LDLIBS)

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)

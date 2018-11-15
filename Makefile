#PROGRAM = skakutavaLoptica
CC      = gcc
CFLAGS  = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL -lm

skakutavaLoptica: main.o loptica.o ostrvo.o
	$(CC) $(LDFLAGS) -o skakutavaLoptica.out main.o loptica.o ostrvo.o $(LDLIBS)

$(PROGRAM): main.o
	$(CC) $(LDFLAGS) -c $(PROGRAM) main.c $(LDLIBS)
$(PROGRAM): loptica.o
	$(CC) $(LDFLAGS) -c $(PROGRAM) loptica.c $(LDLIBS)
$(PROGRAM): ostrvo.o
	$(CC) $(LDFLAGS) -c $(PROGRAM) ostrvo.c $(LDLIBS)


.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)

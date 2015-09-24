CC=gcc
CFLAGS=-g -Wall -Wshadow
LDFLAGS=-lm -lglut -lGLEW -lGLU -lGL
SOURCES=button_0.c
HEADERS=
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=but

#  Idea of a makefile is to create object files of your compiled code	
#	and link those object files to make the program. By creating
#	dependencies for each of the desired files to be compiled, the
#	minimum ammount of recompiling can be done
#
# item : dependency
# $ : use a substitution 
# @ : reference name of item
# ^ : first depency
# < : all dependencies
# -c: compile



all: $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

button_0.o: button_0.c
	$(CC) $(CFLAGS) -c $< -o $@

hexagon.o : hexagon.c
	$(CC) $(CFLAGS) -c $< -o $@

triangle.o : triangle.c 
	$(CC) $(CFLAGS) -c $< -o $@


#.c.o:
#	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -f *.o
	ls


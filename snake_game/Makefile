IDIR =../include
CC=g++
CFLAGS= -I$(IDIR) -g -O0 -std=c++11

ODIR=.

LIBS=-lncurses

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Ant.o DoodleBug.o main.o Male.o Organism.o Queen.o Worker.o World.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: snake_game

snake_game: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *~ core $(INCDIR)/*~ 
	rm -f  snake_game
	rm -f *.o

etags: 
	find . -type f -iname "*.[ch]" | xargs etags --append  

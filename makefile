BIN=world

CC=g++
CFLAGS=-std=c++2a -Wall -Wextra -pedantic -Werror -g -O3
LIBS=-lX11 -lGLX -lGL

CPPFILES=$(wildcard *.cpp)
OFILES=$(patsubst %.cpp,%.o,$(CPPFILES))

$(BIN): $(OFILES)
	$(CC) $(LIBS) -o $(BIN) $(OFILES)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(BIN) $(OFILES)

OUTPUT=ps4
CC=gcc
CFLAGS=-std=c11 -g -Wall -Werror
LDLIBS=-lm
SOURCES=main.c bmp.c transformations.c

all: ps4

codeanalysis: $(SOURCES)
	cppcheck --enable=unusedFunction --error-exitcode=1 $^

$(OUTPUT): main.o bmp.o transformations.o
	echo "Starting compilation"
	#cppcheck --enable=unusedFunction --error-exitcode=1 $(SOURCES)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $(OUTPUT)

main.o: main.c
	echo "Making file .o - main.o"
	$(CC) $(CFLAGS) -c $^ $(LDLIBS) -o $@

bmp.o: bmp.c
	echo "Making file .o - bmp.o"
	$(CC) $(CFLAGS) -c $^ $(LDLIBS) -o $@

transformations.o: transformations.c
	echo "Making file .o - transformations.o"
	$(CC) $(CFLAGS) -c $^ $(LDLIBS) -o $@

clean:
	rm *.o $(OUTPUT)

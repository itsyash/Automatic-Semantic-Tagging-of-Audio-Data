###########################
# Make file
###########################

CC = g++
CPP = g++
CXX = g++

TARGET = pa

DEFS = 
OPTIMIZATION = -O5
FLAGS = -Wall -Werror -g -I. -L. -I/usr/local/include -L/usr/local/lib $(DEFS) ${OPTIMIZATION}
CFLAGS = $(FLAGS)
CPPFLAGS = $(FLAGS)
#CXXFLAGS = $(FLAGS)

OBJECTS = pa.o util.o

SWITCHES = $(LIBS) ${CFLAGS}

$(TARGET): lib main.o TAGS
#	$(CPP) $(SWITCHES) -o $@ main.o ${OBJECTS} -lboost_thread -lboost_iostreams #-lutil
	$(CPP) $(SWITCHES) -o $@ main.o ${OBJECTS} -lboost_iostreams #-lutil

build: lib main.o TAGS
#	$(CPP) $(SWITCHES) -o $(TARGET) main.o ${OBJECTS} -lboost_thread -lboost_iostreams #-lutil
	$(CPP) $(SWITCHES) -o $(TARGET) main.o ${OBJECTS} -lboost_iostreams #-lutil

lib: $(OBJECTS)
	ar rcs libpa.a $(OBJECTS)
	@if [ ! -d ../lib ]; then \
		mkdir -p ../lib; \
	fi
	cp libpa.a ../lib

main.o: main.cpp pa.hpp util.hpp

pa.o: pa.cpp pa.hpp global.hpp param.hpp util.hpp my_math.hpp

util.o: util.cpp util.hpp

clean:
	rm -rf *.o

TAGS: *.[ch]pp
	etags *.[ch]pp

install: build
	install -d ../bin
	install $(TARGET) ../bin

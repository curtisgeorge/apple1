CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)
LD_FLAGS := -lncurses
CC_FLAGS := -O3 -std=c++11

all: apple1

apple1: ${OBJ_FILES}
	g++ ${LD_FLAGS} -o $@ $^

%.o: %.cpp
	g++ ${CC_FLAGS} -c -o $@ $<

clean:
	-rm -f *.o
	-rm -f apple1

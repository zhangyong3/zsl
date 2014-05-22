#CC=g++ -g -std=c++11
CC=g++ -g
LD=g++
OBJ=mempool.o test.o string.o strtok.o link.o rbtree.o binbuf.o \
	file.o variant.o

test: $(OBJ)
	$(LD) -o $@ $^

%.o:%.cpp
	$(CC) -c $<


clean:
	rm -rf $(OBJ)

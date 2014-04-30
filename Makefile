#CC=g++ -std=c++11
CC=g++ -g
LD=g++
OBJ=mempool.o test.o string.o strtok.o link.o

test: $(OBJ)
	$(LD) -o $@ $^

%.o:%.cpp
	$(CC) -c $<


clean:
	rm -rf $(OBJ)

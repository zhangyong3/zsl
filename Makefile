CC=g++ -std=c++11
LD=g++
OBJ=mempool.o test.o

test: $(OBJ)
	$(LD) -o $@ $^

*.o:*.cpp
	$(CC) -c $<


clean:
	rm -rf $(OBJ)

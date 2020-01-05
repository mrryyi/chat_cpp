output: main.o
	g++ -o output main.o

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm output *.o
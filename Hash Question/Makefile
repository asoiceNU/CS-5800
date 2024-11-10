all: main.o hash_table.o
	g++ -std=c++11 main.o hash_table.o -o run

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

hash_table.o: hash_table.cpp
	g++ -std=c++11 -c hash_table.cpp

clean:
	rm hash_table.o main.o run.exe

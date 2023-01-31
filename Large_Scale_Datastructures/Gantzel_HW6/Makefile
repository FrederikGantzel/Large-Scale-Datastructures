homework : homework.o appendixes.o
	g++ -o homework homework.o appendixes.o -I.

homework.o : homework.cpp
	g++ -c homework.cpp

appendixes.o : appendixes.cpp
	g++ -c appendixes.cpp

clean:
	rm homework homework.o appendixes.o

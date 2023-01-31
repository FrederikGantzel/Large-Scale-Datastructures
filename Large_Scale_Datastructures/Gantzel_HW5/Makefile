homework : homework.o covidgenome.o
	g++ -o homework homework.o covidgenome.o -I.

homework.o : homework.cpp
	g++ -c homework.cpp

covidgenome.o : covidgenome.cpp
	g++ -c covidgenome.cpp

clean:
	rm homework homework.o covidgenome.o

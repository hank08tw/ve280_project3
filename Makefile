all:p3

p3: p3.o simulation.o
	g++ -o p3 p3.o simulation.o
p3.o: p3.cpp
	g++ -c p3.cpp
simulation.o: simulation.cpp
	g++ -c simulation.cpp
clean:
	rm -f p3 *.o 

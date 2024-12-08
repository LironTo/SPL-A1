# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

all: clean Link

compile: bin/Auxiliary.o bin/Simulation.o bin/Facility.o bin/Settlement.o bin/SelectionPolicy.o bin/Plan.o bin/Action.o bin/main.o

bin/Auxiliary.o: src/Auxiliary.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp

bin/Simulation.o: src/Simulation.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Simulation.o src/Simulation.cpp

bin/Facility.o: src/Facility.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Facility.o src/Facility.cpp

bin/Settlement.o: src/Settlement.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Settlement.o src/Settlement.cpp

bin/SelectionPolicy.o: src/SelectionPolicy.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp

bin/Plan.o: src/Plan.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Plan.o src/Plan.cpp
	
bin/Action.o: src/Action.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Action.o src/Action.cpp

bin/main.o: src/main.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/main.o src/main.cpp

Link: compile
	g++ -g -o bin/simulation bin/Auxiliary.o bin/Simulation.o bin/Facility.o bin/Settlement.o bin/SelectionPolicy.o bin/Plan.o bin/Action.o bin/main.o

Run: bin/simulation 
	./bin/simulation config_file.txt

clean:
	rm -rf bin/*

Leaks:
	valgrind --leak-check=full --show-reachable=yes bin/simulation config_file.txt 

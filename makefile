# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

compile: bin/Action.o bin/Auxiliary.o bin/Facility.o bin/Plan.o bin/SelectionPolicy.o bin/Settlement.o bin/Simulation.o bin/main.o

bin/Action.o: src/Action.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Action.o src/Action.cpp

bin/Auxiliary.o: src/Auxiliary.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp

bin/Facility.o: src/Facility.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Facility.o src/Facility.cpp

bin/Plan.o: src/Plan.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Plan.o src/Plan.cpp

bin/SelectionPolicy.o: src/SelectionPolicy.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp

bin/Settlement.o: src/Settlement.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Settlement.o src/Settlement.cpp

bin/Simulation.o: src/Simulation.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Simulation.o src/Simulation.cpp

bin/main.o: src/main.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -o bin/main.o src/main.cpp

Link: compile
	g++ -g -o bin/main bin/Action.o bin/Auxiliary.o bin/Facility.o bin/Plan.o bin/SelectionPolicy.o bin/Settlement.o bin/Simulation.o bin/main.o

Run: bin/main
	./bin/main

all: link

clean:
	rm -rf bin/*


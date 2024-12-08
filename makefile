all: clean compile link run

link: compile
	g++ -o bin/main bin/main.o bin/Action.o bin/Auxiliary.o bin/Facility.o bin/Plan.o bin/SelectionPolicy.o bin/Settlement.o bin/Simulation.o

	
compile: src/main.cpp src/Action.cpp src/Auxiliary.cpp src/Facility.cpp src/Plan.cpp src/SelectionPolicy.cpp src/Settlement.cpp src/Simulation.cpp 
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -c  -o bin/main.o src/main.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Action.o src/Action.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Auxiliary.o src/Auxiliary.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Facility.o src/Facility.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Plan.o src/Plan.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/SelectionPolicy.o src/SelectionPolicy.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Settlement.o src/Settlement.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Simulation.o src/Simulation.cpp


clean:  
	rm -f bin/*

run:
	./bin/main ./config_file.txt
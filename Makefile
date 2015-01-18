main: src/main.cpp
	clang++ -std=c++11 -O2 -Wall -Iinclude/protocol/include src/*.cpp -o main -lboost_system

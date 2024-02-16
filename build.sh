mkdir -p bin/test
g++ -std=c++17 -g -ftrapv -o bin/test/solvetest test/solvetest.cpp
g++ -std=c++17 -g -ftrapv -o bin/test/gmptest test/gmptest.cpp -lgmpxx -lgmp

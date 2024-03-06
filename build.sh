mkdir -p bin/test
g++ -std=c++17 -g -ftrapv -o bin/test/solvetest test/solvetest.cpp
g++ -std=c++17 -g -o bin/test/gmptest test/gmptest.cpp -lgmpxx -lgmp
g++ -std=c++17 -g -ftrapv -o bin/test/mpstest test/mpstest.cpp
g++ -std=c++17 -g -ftrapv -o bin/test/gcdtest test/gcdtest.cpp
g++ -std=c++17 -g -o bin/solve src/solve.cpp -lgmpxx -lgmp


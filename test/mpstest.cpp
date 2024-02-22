#include<iostream>
#include<cassert>
#include<numeric>
#include<gmpxx.h>


using std::gcd;

#include"../src/problem.hpp"
#include"../src/formats.hpp"

void test_ej(void) {
	Problem<long long> problem = read_mps<long long>("test/ej.mps");

	SolutionStats res = problem.solve();

	std::cout << *(problem.tab) << '\n';
}

int main(void) {
	test_ej();

	return 0;
}

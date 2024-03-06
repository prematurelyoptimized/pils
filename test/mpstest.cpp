#include<iostream>
#include<cassert>
#include<numeric>

using std::gcd;

#include"../src/problem.hpp"
#include"../src/formats.hpp"

void test_ej(void) {
	Problem<long long> problem = read_mps<long long>("test/ej.mps");

	SolutionStats res = problem.solve();

	std::cout << *(problem.tab) << '\n';

	auto x0 = problem.getVariable("x0");
	auto x1 = problem.getVariable("x1");
	auto x2 = problem.getVariable("x2");

	assert(x2->getValue() == 15506);
	assert(x1->getValue() == 1);
	assert(x0->getValue() == 25507);

	assert(res.divisibility_bound_improvements == 10);
}

int main(void) {
	test_ej();

	return 0;
}

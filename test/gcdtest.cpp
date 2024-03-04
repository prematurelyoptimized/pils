#include<iostream>
#include<cassert>
#include<numeric>

using std::gcd;

#include"../src/problem.hpp"

int main(void) {
	Problem<long> problem;

	auto x1 = problem.addVariable("x1", 35);
	auto x2 = problem.addVariable("x2", 100);

	problem.addConstraint("0 = x1 - 2*x2");
	
	auto obj = problem.addObjective("-x1-x2");

	SolutionStats res = problem.solve();
	
	assert(x1->getValue() == 34);
	assert(x2->getValue() == 17);
	assert(obj->getValue() == -51);

	assert(res.divisibility_bound_improvements == 1);
}

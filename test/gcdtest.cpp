#include<iostream>
#include<cassert>
#include<numeric>

using std::gcd;

#include"../src/problem.hpp"

void all_but_one_gcd(void) {
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

void kitchen_sink(void) {
	Problem<long long> problem;

	auto x1 = problem.addVariable("x1", 99999999999);
	auto x2 = problem.addVariable("x2", 99999999999);
	auto x3 = problem.addVariable("x3", 99999999999);

	problem.addConstraint("0 = 313*x1 + 414*x2 -515*x3 -515");

	auto obj = problem.addObjective("x3");

	SolutionStats res = problem.solve();

	assert(x1->getValue() == 257);
	assert(x2->getValue() == 1);
	assert(x3->getValue() == 156);

}

int main(void) {
	all_but_one_gcd();
	kitchen_sink();
}

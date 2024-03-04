#include<iostream>
#include<cassert>
#include<cstdlib>
#include<numeric>

// This is a horrible hack to import the std::gcd functionality under
// the name "gcd".  Ideally, this would be a template parameter on the
// problem, but bignum libraries like gmp end up using weird signatures
// to enable lazy evaluation, which causes template deduction to fail.
using std::gcd;

#include"../src/problem.hpp"
#include"../src/constraint.hpp"
#include"../src/variable.hpp"

void test_simple_2_variables_single_pivot(void) {
	/*
	Maximize 3*x1 + 2*x2

	Subject to:
		2*x1 + x2 <= 100
		x1   + x2 <= 80
		x1        <= 40
	*/
	Problem<long> problem;

	auto x1 = problem.addVariable("x1", 40);
	auto x2 = problem.addVariable("x2", 100);

	problem.addConstraint("0 >= 2*x1 + x2 - 100");
	problem.addConstraint("0 >= x1   + x2 - 80");

	auto obj = problem.addObjective("-3*x1 -2*x2");
	
	SolutionStats res = problem.solve();

	std::cout << *(problem.tab) << '\n';

	assert(x1->getValue() == 20);
	assert(x2->getValue() == 60);
	assert(obj->getValue() == -180);
}

void test_simple_2_variables_multiple_pivots(void) {
	/*
	Minimize x2

	Subject to:
		x1			>= 2
		3*x1 - x2 	>= 0
		x1 + x2		>= 6
		-x1 + 2*x2  >= 0
	*/
	Problem<long> problem;
	
	auto x1 = problem.addVariable("x1", 50);
	auto x2 = problem.addVariable("x2", 50);

	problem.addConstraint("0 <= x1 - 2");
	problem.addConstraint("0 <= 3*x1 - x2");
	problem.addConstraint("0 <= x1 + x2 -6");
	problem.addConstraint("0 <= 2*x2 - x1");

	auto obj = problem.addObjective("x2");

	SolutionStats res = problem.solve();

	std::cout << *(problem.tab) << '\n';

	assert(x1->getValue() == 4);
	assert(x2->getValue() == 2);
	assert(obj->getValue() == 2);
}

void test_simple_infeasible(void) {
	/*
	Minimize 

	Subject to:
		x1 		   <= 2
		3*x1 - x2  >= 0
		x1 + x2	   >= 6
		-x1 + 2*x2 <= 0
	*/
	Problem<long> problem;
	
	auto x1 = problem.addVariable("x1", 50);
	auto x2 = problem.addVariable("x2", 50);

	problem.addConstraint("0 >= x1 - 2");
	problem.addConstraint("0 <= 3*x1 - x2");
	problem.addConstraint("0 <= x1 + x2 -6");
	problem.addConstraint("0 >= 2*x2 - x1");

	auto obj = problem.addObjective("x2");

	SolutionStats res = problem.solve();
}

void test_shortest_hamiltonian_path(void) {
	/*
	Minimize:
		2*ab + 8*ad + 5*bd + 6*be + 9*ce + 3*cf + 
			3*de + 2*df + ef

	Subject to:
		ab + ad 		  = 1
		ab + bd + be	  = 2
		ce + cf 		  = 1
		ad + bd + de + df = 2
		be + ce + de + ef = 2
		cf + df + ef	  = 2
		and all variables are binary
	*/
	Problem<long> problem;
	
	auto ab = problem.addVariable("ab", 1);
	auto ad = problem.addVariable("ad", 1);
	auto bd = problem.addVariable("bd", 1);
	auto be = problem.addVariable("be", 1);
	auto ce = problem.addVariable("ce", 1);
	auto cf = problem.addVariable("cf", 1);
	auto de = problem.addVariable("de", 1);
	auto df = problem.addVariable("df", 1);
	auto ef = problem.addVariable("ef", 1);

	problem.addConstraint("0 == ab + ad -1");
	problem.addConstraint("0 == ab + bd + be -2");
	problem.addConstraint("0 == ce + cf -1");
	problem.addConstraint("0 == ad + bd + de + df -2");
	problem.addConstraint("0 == be + ce + de + ef -2");
	problem.addConstraint("0 == cf + df + ef -2");

	auto obj = problem.addObjective("2*ab + 8*ad + 5*bd + 6*be + 9*ce + 3*cf + 3*de + 2*df + ef");

	SolutionStats res = problem.solve();

	std::cout << *(problem.tab) << '\n';

	assert(ab->getValue() == 1);
	assert(bd->getValue() == 1);
	assert(de->getValue() == 1);
	assert(ef->getValue() == 1);
	assert(cf->getValue() == 1);

	assert(be->getValue() == 0);
	assert(ad->getValue() == 0);
	assert(ce->getValue() == 0);
	assert(df->getValue() == 0);

	assert(obj->getValue() == 14);
}

void test_random_10x5(void) {
	Problem<long> problem;

	auto x1 = problem.addVariable("x1",8);
	auto x2 = problem.addVariable("x2",8);
	auto x3 = problem.addVariable("x3",8);
	auto x4 = problem.addVariable("x4",8);
	auto x5 = problem.addVariable("x5",8);

	auto obj = problem.addObjective("-10*x1+2*x2+17*x3-15*x4+11*x5");

	problem.addConstraint("0 <= 2*x1+2*x2+5*x4+3*x5-30");
	problem.addConstraint("0 >= -5*x1-x2+x3-x4-5*x5+35");
	problem.addConstraint("0 >= -4*x1-x2-x3-2*x4+4*x5");
	problem.addConstraint("0 <= -4*x1+3*x2-4*x3-x4+5*x5+4");
	problem.addConstraint("0 >= -3*x1-5*x2-2*x3+4*x4-3*x5+29");
	problem.addConstraint("0 >= x2+2*x4+x5-13");
	problem.addConstraint("0 >= -4*x1-2*x3-8*x4-x5");
	problem.addConstraint("0 >= 5*x1+2*x2-2*x3-2*x4+2*x5-8");
	problem.addConstraint("0 <= x1-2*x2+2*x3+3*x4+3*x5-30");
	problem.addConstraint("0 >= x1-2*x2-3*x3+5*x4-3*x5+17");

	SolutionStats res = problem.solve();

	std::cout << *(problem.tab) << '\n';

	assert(x1->getValue() == 2);
	assert(x2->getValue() == 2);
	assert(x3->getValue() == 5);
	assert(x4->getValue() == 3);
	assert(x5->getValue() == 5);

	assert(obj->getValue() == 79);
}

int main(void) {
	test_simple_2_variables_single_pivot();
	test_simple_2_variables_multiple_pivots();
	test_shortest_hamiltonian_path();
	test_random_10x5();

	try {
		test_simple_infeasible();
	} catch (const std::runtime_error& e) {
		return 0;
	}

	return 1;
}

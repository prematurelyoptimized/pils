#include<iostream>
#include<cassert>
#include<gmpxx.h>
#include"../src/problem.hpp"
#include"../src/formats.hpp"

void test_ej(void) {
	Problem<mpz_class> problem = read_mps<mpz_class>("test/markshare_4_0.mps");

	problem.getVariable("s1")->expected_value = 0; 
	problem.getVariable("s2")->expected_value = 0; 
	problem.getVariable("s3")->expected_value = 1; 
	problem.getVariable("s4")->expected_value = 0; 
	problem.getVariable("x1")->expected_value = 1; 
	problem.getVariable("x2")->expected_value = 1; 
	problem.getVariable("x3")->expected_value = 1; 
	problem.getVariable("x4")->expected_value = 0; 
	problem.getVariable("x5")->expected_value = 0; 
	problem.getVariable("x6")->expected_value = 1; 
	problem.getVariable("x7")->expected_value = 1; 
	problem.getVariable("x8")->expected_value = 1; 
	problem.getVariable("x9")->expected_value = 0; 
	problem.getVariable("x10")->expected_value = 0; 
	problem.getVariable("x11")->expected_value = 0; 
	problem.getVariable("x12")->expected_value = 0; 
	problem.getVariable("x13")->expected_value = 1; 
	problem.getVariable("x14")->expected_value = 1; 
	problem.getVariable("x15")->expected_value = 1; 
	problem.getVariable("x16")->expected_value = 1; 
	problem.getVariable("x17")->expected_value = 0; 
	problem.getVariable("x18")->expected_value = 0; 
	problem.getVariable("x19")->expected_value = 0; 
	problem.getVariable("x20")->expected_value = 0; 
	problem.getVariable("x21")->expected_value = 1; 
	problem.getVariable("x22")->expected_value = 0; 
	problem.getVariable("x23")->expected_value = 1; 
	problem.getVariable("x24")->expected_value = 0; 
	problem.getVariable("x25")->expected_value = 0; 
	problem.getVariable("x26")->expected_value = 1; 
	problem.getVariable("x27")->expected_value = 1; 
	problem.getVariable("x28")->expected_value = 0; 
	problem.getVariable("x29")->expected_value = 0; 
	problem.getVariable("x30")->expected_value = 0;
 
	SolutionStats res = problem.solve();
}

int main(void) {
	test_ej();

	return 0;
}

#include<iostream>
#include<gmpxx.h>
#include"../src/problem.hpp"
#include"../src/formats.hpp"

int main(int argc, char** argv) {
	if(argc < 2) {
		std::cout << "Usage: solve filename.mps\n";
		return 1;
	}

	std::string filename = std::string(argv[1]);

	Problem<mpz_class> problem = read_mps<mpz_class>(filename);
	SolutionStats res = problem.solve();

	for(auto it : problem.getVarNames()) {
		std::cout << it << " \t" << problem.getVariable(it)->getValue() << '\n';
	}

	std::cout << "Used " << res.pivots << " pivots\n";
	std::cout << "Improved " << res.divisibility_bound_improvements << " bounds\n";

	return 0;
}

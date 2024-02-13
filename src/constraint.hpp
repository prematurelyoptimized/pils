#ifndef PILS_CONSTRAINT
#define PILS_CONSTRAINT

#include<functional>
#include<stdexcept>
#include<cctype>
#include<sstream>
#include<unordered_map>
#include"variable.hpp"


template<typename int_type>
struct Constraint {
	// Represents the fact that lhs == rhs
	Term<int_type> lhs;
	std::vector<Term<int_type>> rhs;

	Constraint(std::shared_ptr<Variable<int_type>> in_lhs, std::vector<Term<int_type>> in_rhs) :
		lhs(Term<int_type>(in_lhs, 1)), rhs(in_rhs) {};

	Constraint(Term<int_type> in_lhs, std::vector<Term<int_type>> in_rhs) :
		lhs(in_lhs), rhs(in_rhs) {};
};

enum PARSE_STATE { start, op1, op2, term, coeff, varname };

enum SENSE { LESS_THAN, GREATER_THAN, EQUALS };

template<typename int_type>
int_type compute_bound(std::vector<Term<int_type>> terms) {
	int_type retval = int_type(0);
	for(auto it = terms.begin(); it != terms.end(); ++it) {
		if(it->coefficient > 0) {
			retval += it->coefficient * it->variable->upper_bound;
		}
	}
	return retval;
}

#endif
#ifndef PILS_VARIABLE
#define PILS_VARIABLE

#include<vector>
#include<string>

enum Bound { UPPER = -1, LOWER = 1};

template<typename int_type>
struct Term;

template<typename int_type>
struct Variable{
	std::string name;
	int_type upper_bound;
	bool is_slack;
	size_t priority;
	int_type expected_value;	// If the problem has a known solution, setting this value accordingly will cause the solving code to do validity checks along the way

	bool is_basic;
	Bound pegged_bound;
	size_t index;

	// If this variable is expressed in terms of problem variables, this is the expansion	
	std::vector<Term<int_type>> terms;
	// Otherwise, it's just a plain value
	int_type value;
	
	Variable(std::string name, int_type upper_bound, bool is_slack, size_t priority) :
		name(name), upper_bound(upper_bound), is_slack(is_slack), priority(priority), is_basic(false), pegged_bound(LOWER), value(int_type(0)), expected_value(int_type(-1)) {};
	Variable(std::string name, int_type upper_bound, int_type value, int_type ex_value = int_type(-1)) :
		name(name), upper_bound(upper_bound), value(value), is_slack(false), priority(0), is_basic(false), pegged_bound(LOWER), expected_value(ex_value) {};
	Variable(std::string name, int_type upper_bound) :
		name(name), upper_bound(upper_bound), is_slack(false), priority(0), is_basic(false), pegged_bound(LOWER), value(int_type(0)), expected_value(int_type(-1)) {};

	int_type getValue() {
		if(terms.size() > 0) {
			int_type retval = 0;
			for(auto i = terms.begin(); i != terms.end(); ++i) {
				retval += i->variable->getValue() * i->coefficient;
			}
			return retval;
		}
		return value;
	}

	static const std::shared_ptr<Variable<int_type>> ONE;

	bool operator==(Variable const& other) const {
		return name == other.name &&
				upper_bound == other.upper_bound &&
				is_slack == other.is_slack;
	}
};

template<typename int_type>
const std::shared_ptr<Variable<int_type>> Variable<int_type>::ONE = std::make_shared<Variable<int_type>>("ONE", 1, int_type(1), int_type(1));


template<typename int_type>
struct Term {
	std::shared_ptr<Variable<int_type>> variable;
	int_type coefficient;

	Term(std::shared_ptr<Variable<int_type>> in_variable, int_type in_coefficient) :
		variable(in_variable), coefficient(in_coefficient) {};
};

#endif

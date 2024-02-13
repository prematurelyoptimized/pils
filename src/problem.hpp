#ifndef PILS_PROBLEM
#define PILS_PROBLEM

#include<memory>
#include<string>
#include<vector>
#include<unordered_map>
#include<stdexcept>
#include<algorithm>
#include<optional>
#include<iostream>
#include"constraint.hpp"
#include"variable.hpp"
#include"tableau.hpp"
#include"solutionstats.hpp"

template<typename int_type, int_type(*gcd)(int_type,int_type), int_type(*abs)(int_type)>
class Problem {
	size_t next_priority = 1;	// Priority 0 is reserved for the objective
	size_t next_id = 0;			// Used for naming dummy and slack variables

public:
	std::unique_ptr<Tableau<int_type, gcd, abs>> tab;
	std::vector<Constraint<int_type>> problem_constraints;
	std::optional<Constraint<int_type>> objective;
	std::unordered_map<std::string, std::shared_ptr<Variable<int_type>>> var_map;

	std::shared_ptr<Variable<int_type>> addVariable(std::string name, int_type upper_bound) {
		std::shared_ptr<Variable<int_type>> newVar = std::make_shared<Variable<int_type>>(name, upper_bound, false, this->get_next_priority());
		var_map.emplace(name, newVar);
		return newVar;
	}

	void addConstraint(const std::string & constraint) {
		addConstraint(this->parse_constraint(constraint.c_str()));
	}

	void addConstraint(Constraint<int_type> constraint) {
		constraint.lhs.variable->priority = this->get_next_priority();
		constraint.lhs.variable->is_basic = true;
		problem_constraints.push_back(constraint);
	}

	std::shared_ptr<Variable<int_type>> addObjective(Constraint<int_type> constraint) {
		constraint.lhs.variable->priority = 0;
		constraint.lhs.variable->name = "OBJ";
		constraint.lhs.variable->is_slack = false;
		constraint.lhs.variable->is_basic = true;
		objective = constraint;
		return constraint.lhs.variable;
	}

	std::shared_ptr<Variable<int_type>> addObjective(const std::string & constraint) {
		return addObjective(this->parse_constraint(("0="+constraint).c_str()));
	}

	SolutionStats solve(void) {
		SolutionStats stats;
		std::vector<std::shared_ptr<Variable<int_type>>> variables;
		for(auto it = var_map.begin(); it != var_map.end(); ++it) {
			variables.push_back(it->second);
		}
		tab.reset(new Tableau<int_type, gcd, abs>(variables, &stats));
		// Start by optimizing the objective with an empty set of constraints
		if(objective.has_value()) {
			tab->add_constraint(objective.value());
			for(auto term = objective.value().rhs.begin(); term != objective.value().rhs.end(); ++term) {
				if(term->coefficient < 0) {
					tab->toggle(term->variable->index);
				}
			}
		}	

		// Next, introduce the problem constraints 1-by-1 as they are violated
		std::vector<Constraint<int_type>> pending_constraints = problem_constraints;
		bool done = false;
		while(!done) {
			/*
			std::cout << *tab << '\n';
			for(auto var : tab->column_headers) {
				std::cout << var->name << '\t' << var->value << '\t' << var->priority << '\n';
			}
			for(auto var : tab->row_headers) {
				std::cout << var->name << '\t' << tab->lhs_values[var->index] << '\t' << var->priority << '\n';
			}
			*/

			done = true;
			// Find a pending constraint that is violated
			for(auto it = pending_constraints.begin(); it != pending_constraints.end(); ++it) {
				if(is_violated(*it)) {
					// If one is found, add it to the problem and refeasiblize
					tab->add_constraint(*it);
					pending_constraints.erase(it);
					// It could happen that adding this constraint put the first 
					// non-zero entry into a column.  In that case, we need to do
					// a little work to ensure every column is lex-positive.
					for(auto term = it->rhs.begin(); term != it->rhs.end(); ++term) {
						if((term->variable->priority > it->lhs.variable->priority) &&
						  		(term->variable->pegged_bound * term->coefficient < 0)) {
							size_t row = 0;
							for(; row < tab->rows.size(); ++it) {
								if((tab->rows[row][term->variable->index] != 0) &&
										(tab->row_headers[row]->priority < it->lhs.variable->priority)) {
									break;
								}
							}
							if(row == tab->rows.size()) {
								tab->toggle(term->variable->index);
							}
						}
					}
					// Then, worry about primal feasibility
					this->optimize(stats);
					done = false;
					break;
				}
			}
		}

		for(size_t it = 0; it < tab->row_headers.size(); ++it) {
			tab->row_headers[it]->value = tab->lhs_values[it] / tab->lhs_coefficients[it];
		}
		return stats;
	}

private:
	bool is_violated(const Constraint<int_type>& constraint) {
		int_type rhs_value = 0;
		int_type rhs_denom = 1;
		for(auto it = constraint.rhs.begin(); it != constraint.rhs.end(); ++it) {
			if (it->variable->is_basic) {
				const int_type& other_denom = tab->lhs_coefficients[it->variable->index];
				int_type multiplier = other_denom / gcd(rhs_denom, other_denom);
				rhs_denom *= multiplier;
				rhs_value *= multiplier;
				rhs_value += it->coefficient * tab->constants[it->variable->index] * (rhs_denom / other_denom);
				for(auto var : tab->column_headers) {
					rhs_value += it->coefficient * var->value * tab->rows[it->variable->index][var->index] * (rhs_denom / other_denom);
				}
			} else {
				rhs_value += it->coefficient * it->variable->getValue() * rhs_denom;
			}
		}
		return rhs_value < 0 || rhs_value > constraint.lhs.variable->upper_bound * constraint.lhs.coefficient * rhs_denom;
	}

	bool column_comparator(size_t pivot_row, size_t column1, size_t column2) {
		// True if column1 <lex< column2
		size_t comparison_point = tab->row_headers[pivot_row]->priority;
		bool retval = tab->column_headers[column1]->priority > tab->column_headers[column2]->priority;	// High priorities are lower in lex-ordering
		for(size_t row_idx = 0; row_idx < tab->rows.size(); ++row_idx) {
			if(tab->row_headers[row_idx]->priority < comparison_point) {
				// Watch out for overflow
				int_type denominator = gcd(tab->rows[pivot_row][column2], tab->rows[pivot_row][column1]);
				int_type comparison = abs(tab->rows[row_idx][column1] * (tab->rows[pivot_row][column2] / denominator))
								- abs(tab->rows[row_idx][column2] * (tab->rows[pivot_row][column1] / denominator));
				if(comparison < 0) {
					retval = true;
					comparison_point = tab->row_headers[row_idx]->priority;
				} else if(comparison > 0) {
					retval = false;
					comparison_point = tab->row_headers[row_idx]->priority;
				}
			}
		}
		return retval;
	}

	size_t pivot_column_is_valid(size_t pivot_row, size_t candidate_column) {
		if (tab->lhs_values[pivot_row] < 0) {
			return tab->rows[pivot_row][candidate_column] * tab->column_headers[candidate_column]->pegged_bound > 0;
		} else {
			return tab->rows[pivot_row][candidate_column] * tab->column_headers[candidate_column]->pegged_bound < 0;
		}
	}

	size_t find_pivot_column(size_t pivot_row) {
		// Find the lex-minimal column with a positive entry in the pivot row
		// Start by finding any column with a positive entry in the pivot row
		size_t retval = 0;
		for(;retval < tab->column_headers.size();++retval) {
			if(pivot_column_is_valid(pivot_row, retval)) break;
		}
		if(retval == tab->column_headers.size()) {
			// If the whole row is negative, the problem is infeasible
			throw std::runtime_error("Problem is infeasible");
		}
		// Then, look for any column that is lex-smaller
		for(size_t it = retval + 1; it < tab->column_headers.size(); ++it) {
			if(pivot_column_is_valid(pivot_row, it) && column_comparator(pivot_row, it, retval)) {
				retval = it;
			}
		}
		return retval;
	}

	void optimize(SolutionStats& stats) {
		// This presumes that the tableau is lex-positive
		bool done = false;
		while(true) {
			/*
			std::cout << *tab << "\n";
			for(auto var : tab->row_headers) {
				std::cout << var->name << '\t' << var->value << '\n';
			}
			for(auto var : tab->column_headers) {
				std::cout << var->name << '\t' << var->value << '\n';
			}
			std::cout << "\n\n\n";
			*/
			done = true;
			size_t pivot_row = 0;
			size_t pivot_column = 0;
			// See if anything is violated (objective is row 0, so skip that)
			for(size_t it = 1; it < tab->lhs_values.size(); ++it) {
				if(tab->lhs_values[it] < 0 || tab->lhs_values[it] > tab->lhs_coefficients[it] * tab->row_headers[it]->upper_bound) {
					pivot_row = it;
					pivot_column = find_pivot_column(pivot_row);
					done = false;
					break;
				}
			}
			if(done) {
				break;
			}
			// Do the pivot
			if(abs(tab->column_headers[pivot_column]->upper_bound * tab->rows[pivot_row][pivot_column])
				< abs(tab->lhs_values[pivot_row])) {
				tab->toggle(pivot_column);
			} else {
				tab->pivot(pivot_row, pivot_column);
			}
		}
	}

	Constraint<int_type> parse_constraint(const char* str) {
		std::vector<Term<int_type>> rhs;
		PARSE_STATE state = start;
		std::stringstream buffer;
		int_type buffer_coeff = int_type(1);
		bool negated = false;
		SENSE sense = EQUALS;
		while(*str != '\0') {
			char character = *(str++);
			// skip whitespace
			if(character == ' ' || character == '\t') continue;

			switch(state) {
				case start:
					if(character != '0') {
						throw std::invalid_argument("Comparisons must be against 0");
					}
					state = op1;
					break;
				case op1:
					if(character == '<') {
						sense = LESS_THAN;
					} else if(character == '>') {
						sense = GREATER_THAN;
					} else if(character == '=') {
						sense = EQUALS;
					} else {
						throw std::invalid_argument("Bad operator");
					}
					state = op2;
					break;
				case op2:
					if(character == '=') {
						state = term;
						break;
					}
					// Intentional fall-through
				case term:
					if(character == '-') {
						negated = !negated;
						state = term;
						break;
					}
					if(!isalnum(character)) {
						throw std::invalid_argument("Bad character");
					}
					buffer << character;
					if(isdigit(character)) {
						state = coeff;
					} else {
						buffer_coeff = negated ? -1 : 1;				
						state = varname;
					}
					break;
				case coeff:
					if(character == '*' || character == '+' || character == '-') {
						buffer >> buffer_coeff;
						buffer.clear();
						buffer.str("");
						if(negated) {
							buffer_coeff = -buffer_coeff;
						}
						if(character == '*') {
							state = varname;
						} else {
							rhs.push_back(Term<int_type>(std::make_shared<Variable<int_type>>("ONE", 1, 1), buffer_coeff));
							negated = (character == '-');
							buffer_coeff = 1;
						}
					} else {
						buffer << character;
					}
					break;
				case varname:
					if(character != '+' && character != '-') {
						buffer << character;
						break;
					}
					std::string var_name;
					buffer >> var_name;
					buffer.clear();
					buffer.str("");
					rhs.push_back(Term<int_type>(var_map.at(var_name), buffer_coeff));
					buffer_coeff = 1;
					negated = (character == '-');
					state = term;
					break;
			}
		}

		if(buffer.rdbuf()->in_avail() > 0) {
			std::shared_ptr<Variable<int_type>> variable;
			if(state == coeff) {
				buffer >> buffer_coeff;
				variable = std::make_shared<Variable<int_type>>("ONE", 1, 1);
				buffer_coeff *= negated ? -1 : 1;
			} else if(state == varname) {
				std::string var_name;
				buffer >> var_name;
				variable = var_map.at(var_name);
			}
			rhs.push_back(Term<int_type>(variable, buffer_coeff));
		}

		int_type ub = int_type(0);
		switch(sense) {
			case EQUALS:
				break;
			case GREATER_THAN:
				for(auto it = rhs.begin(); it != rhs.end(); ++it) {
					it->coefficient = -it->coefficient;
				}
				// Intentional fall-through
			case LESS_THAN:
				ub = compute_bound(rhs);
				break;
		}
		std::ostringstream varname;
  		varname << "s" << get_next_id();
		std::shared_ptr<Variable<int_type>> lhs = std::make_shared<Variable<int_type>>(varname.str(), ub, true, 0);
		return Constraint<int_type>(lhs, rhs);
	}

	size_t get_next_priority() {
		return next_priority++;
	}

	size_t get_next_id() {
		return next_id++;
	}
};

#endif
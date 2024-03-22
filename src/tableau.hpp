#ifndef PILS_TABLEAU
#define PLIS_TABLEAU

#include<vector>
#include<algorithm>
#include<ostream>
#include"variable.hpp"
#include"row.hpp"
#include"constraint.hpp"
#include"solutionstats.hpp"

// Forward declare Problem class
template<typename int_type>
class Problem;

template <typename int_type>
int_type evaluate(const int_type& constant, const Row<int_type>& row, const std::vector<std::shared_ptr<Variable<int_type>>>& variables) {
	int_type retval = constant;
	for(size_t i = 0; i < variables.size(); ++i) {
		if(variables[i]->pegged_bound == UPPER) {
			retval += row[i] * variables[i]->upper_bound;
		}
	}
	return retval;
}

template <typename int_type>
class Tableau {
	friend class Problem<int_type>;
	std::vector<std::shared_ptr<Variable<int_type>>> column_headers;
	std::vector<std::shared_ptr<Variable<int_type>>> row_headers;
	
	std::vector<Row<int_type>> rows;
	std::vector<int_type> lhs_coefficients;
	std::vector<int_type> lhs_values;
	std::vector<int_type> constants;

	SolutionStats* const stats;

	friend std::ostream& operator<<(std::ostream& os, const Tableau& tab) {
		os << "\t\t\t";
		for(auto iter = tab.column_headers.begin(); iter != tab.column_headers.end(); ++iter) {
			os << (*iter)->name << ' ' << ((*iter)->pegged_bound == LOWER ? 'v' : '^') << "\t";
		}
		os << "\n";
		for(size_t i = 0; i < tab.row_headers.size(); ++i) {
			os << tab.lhs_coefficients[i] << " *\t";
			os << tab.row_headers[i]->name << " =\t";
			os << tab.constants[i] << " +\t";
			for(size_t j = 0; j < tab.column_headers.size(); ++j) {
				os << tab.rows[i][j] << " \t";
			}
			os << "\n";
		}
		return os;
	}

public:
	Tableau(std::vector<std::shared_ptr<Variable<int_type>>> variables, SolutionStats* const in_stats) : stats(in_stats), column_headers(variables) {
		init();
	};

	Tableau(std::vector<std::shared_ptr<Variable<int_type>>> variables) : stats(NULL), column_headers(variables) {
		init();
	};

	void add_constraint(const Constraint<int_type>& constraint) {
		// This presumes the lhs of the constraint does not already exist in the tableau
		Row<int_type> new_row;
		
		int_type lhs_coefficient = constraint.lhs.coefficient;
		int_type constant = int_type(0);
		for(auto it = constraint.rhs.begin(); it != constraint.rhs.end(); ++it) {
			if(*(it->variable) == ONE<int_type>) {
				constant += lhs_coefficient * it->coefficient;
			} else if(it->variable->upper_bound == 0) {
				continue;
			} else if(!it->variable->is_basic) {
				new_row[it->variable->index] += lhs_coefficient * it->coefficient;
			} else {
				// We really need to compute new_row + it->coefficient*rows[row_index].
				// However, getting the denominators right is tricky.
				const int_type& r = it->coefficient;
				const int_type& x = lhs_coefficients[it->variable->index];
				const int_type& y = lhs_coefficient;
				const int_type& gcdrx = gcd(r,x);
				const int_type& biggcd = gcd(y, x/gcdrx);
				const int_type& a = x/(gcdrx*biggcd);
				const int_type& b = (r/gcdrx) * (y/biggcd);

				new_row *= a;
				new_row += rows[it->variable->index] * b;
				lhs_coefficient = a * y;				
				
				constant *= a;
				constant += constants[it->variable->index] * b;
			}
		}

		constraint.lhs.variable->index = row_headers.size();
		constraint.lhs.variable->is_basic = true;
		row_headers.push_back(constraint.lhs.variable);
		rows.push_back(new_row);
		lhs_coefficients.push_back(lhs_coefficient);
		constants.push_back(constant);
		
		lhs_values.push_back(evaluate(constant, new_row, column_headers));
	}

	template<typename iter>
	void add_constraints(iter begin, iter end) {
		static_assert(std::is_convertible<decltype(*begin), Constraint<int_type>>::value, "Iterators must iterate over constraints");
		for(iter it = begin; it != end; ++it) {
			add_constraint(*it);
		}
	}

	void delete_column(size_t index) {
		size_t end = column_headers.size() - 1;
		column_headers[index] = column_headers.back();
		column_headers[index]->index = index;
		column_headers.pop_back();
		for(auto& row : rows) {
			row[index] = row[end];
			row[end] = 0;
		}
	}

	void delete_row(size_t index) {
		size_t end = row_headers.size() - 1;
		
		rows[index] = rows[end];
		lhs_coefficients[index] = lhs_coefficients[end];
		lhs_values[index] = lhs_values[end];
		constants[index] = constants[end];
		row_headers[index] = row_headers[end];
		row_headers[index]->index = index;

		rows.pop_back();
		lhs_coefficients.pop_back();
		lhs_values.pop_back();
		constants.pop_back();
		row_headers.pop_back();
	}

	void remove_slack_rows(void) {
		for(size_t row_idx = 0; row_idx < rows.size();/*empty*/) {
			if(row_headers[row_idx]->is_slack) {
				delete_row(row_idx);
			} else {
				++row_idx;
			}
		}
	}

	void update_bound(std::shared_ptr<Variable<int_type>> var, int_type bound) {
		if(!var->is_basic) {
			// If the variable is basic, then the worst that happens is we become infeasible
			for(size_t it = 0; it < row_headers.size(); ++it) {
				// We need to update the lhs_values table
				lhs_values[it] = evaluate(constants[it], rows[it], column_headers);
			}
		}
	}

	void pivot(size_t pivot_row, size_t column) {
		if (lhs_values[pivot_row] >= 0) {
			row_headers[pivot_row]->value = row_headers[pivot_row]->upper_bound;
			row_headers[pivot_row]->pegged_bound = UPPER;
		} else {
			row_headers[pivot_row]->value = 0;
			row_headers[pivot_row]->pegged_bound = LOWER;
		}
		++(stats->pivots);
		std::swap(row_headers[pivot_row]->is_basic, column_headers[column]->is_basic);
		std::swap(row_headers[pivot_row]->index, column_headers[column]->index);
		std::swap(row_headers[pivot_row], column_headers[column]);

		// Handle pivot row
		std::swap(lhs_coefficients[pivot_row],rows[pivot_row][column]);
		rows[pivot_row][column] *= -1;
		if(lhs_coefficients[pivot_row] < 0) {
			lhs_coefficients[pivot_row] = -lhs_coefficients[pivot_row];
		} else {
			rows[pivot_row] *= -1;
			constants[pivot_row] *= -1;
		}
		lhs_values[pivot_row] = evaluate(constants[pivot_row], rows[pivot_row], column_headers);

		// Handle the rest of the rows
		for(size_t row = 0; row < rows.size(); ++row) {
			if((row != pivot_row) && (rows[row][column] != 0)) {
				int_type multiplier = rows[row][column];
				rows[row][column] = 0;
				int_type denominator = gcd(lhs_coefficients[pivot_row], multiplier);

				lhs_coefficients[row] *= lhs_coefficients[pivot_row] / denominator;
				rows[row] *= lhs_coefficients[pivot_row] / denominator;
				constants[row] *= lhs_coefficients[pivot_row] / denominator;
				rows[row] += rows[pivot_row] * (multiplier / denominator);
				constants[row] += constants[pivot_row] * (multiplier / denominator);
			}
			lhs_values[row] = evaluate(constants[row], rows[row], column_headers);
		}

		// Check if the variable that we just pivoted out of the basis is constant
		if(column_headers[column]->upper_bound == 0) {
			delete_column(column);
		}
	}

	void toggle(size_t pivot_column) {
		switch(column_headers[pivot_column]->pegged_bound) {
			case UPPER:
				column_headers[pivot_column]->pegged_bound = LOWER;
				column_headers[pivot_column]->value = 0;
				break;
			case LOWER:
				column_headers[pivot_column]->pegged_bound = UPPER;
				column_headers[pivot_column]->value = column_headers[pivot_column]->upper_bound;
				break;
		}
		for(size_t row = 0; row < rows.size(); ++row) {
			lhs_values[row] = evaluate(constants[row], rows[row], column_headers);
		}
	}

private:
	void init() {
		size_t index = 0;
		for(auto it = column_headers.begin(); it != column_headers.end(); ) {
			if((*it)->upper_bound == 0) {
				it = column_headers.erase(it);
			} else {
				(*it)->is_basic = false;
				(*it)->index = index++;
				++it;
			}
		}
	}
};

#endif

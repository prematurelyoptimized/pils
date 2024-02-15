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
				os << tab.rows[i][j] << "\t";
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
	void clean_rows(void) {
		// Check for rows with non-zero gcd
		for(size_t row_idx = 0; row_idx < rows.size(); ++row_idx) {
			int_type running_gcd = gcd(lhs_coefficients[row_idx], constants[row_idx]);
			for(size_t col_idx = 0; col_idx < column_headers.size(); ++col_idx) {
				running_gcd = gcd(running_gcd, rows[row_idx][col_idx]);
			}
			if(running_gcd > 1) {
				rows[row_idx] /= running_gcd;
				constants[row_idx] /= running_gcd;
				lhs_coefficients[row_idx] /= running_gcd;
				lhs_values[row_idx] /= running_gcd;
			}
		}

		// Check for rows with all-but-one gcd
	}

	void clean_columns(void) {
		// Check for constant variables

	}

	void init() {
		size_t index = 0;
		for(auto& variable: column_headers) {
			variable->is_basic = false;
			variable->index = index++;
		}
	}
};

#endif

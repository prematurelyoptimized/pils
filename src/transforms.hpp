#ifndef PILS_TRANSFORMS
#define PILS_TRANSFORMS

#include<stdexcept>
#include"tableau.hpp"


template<typename int_type>
size_t get_best_row_for_column(size_t column, const Tableau<int_type>& tableau) {
	size_t best_row = 0;
	for(row = 0; row < tableau.row_headers.size(); ++row) {
		if(tableau.rows[best_row][column] == 0)) {
			// The best row should at least have a non-zero coefficient
			++best_row;
			continue;
		}
		if(tableau.rows[row][column] == 0) {
			// The best row should at least have a non-zero coefficient
			continue;
		}
		if(tableau.row_headers[row].priority < tableau.row_headers[best_row].priority) {
			best_row = row;
		}
	}
	return best_row;
}

template<typename int_type>
size_t find_pivot_column(size_t pivot_row, const Tableau<int_type>& tableau, int sense) {
	// Find the first usable column
	size_t best_column = 0;
	while(best_column < tableau.column_headers.size() && 
		  sgn(tableau.rows[pivot_row][best_column]) != sense * tableau.column_headers[best_column].pegged_bound) {
		++ best_column;
	}
	if(best_column >= tableau.column_headers.size()) {
		// No column is usable
		return best_column;
	}

	size_t best_row = get_best_row_for_column(best_column);

	for(size_t column = best_column + 1; column < tableau.column_headers.size(); ++column) {
		size_t row = get_best_row_for_column(column);
		if(tableau.row_headers[row].priority > tableau.row_headers[best_row].priority) {
			best_column = column;
			best_row = row;
		}
		if(tableau.row_headers[row].priority == tableau.row_headers[best_row].priority) {
			if(tableau.rows[row][column] * tableau.rows[pivot_row][best_column] * sense < 
				tableau.rows[best_row][best_column] * tableau.rows[pivot_row][column] * sense) {
				best_column = column;
				best_row = row;
			}
		}
	}
	return best_column; 
}

template<typename int_type>
void make_feasible(Tableau<int_type>& tableau) {
	bool done = false;
	while(!done) {
		done = true;
		for(size_t row = 0; row < tableau.constants.size(); ++row) {
			if((row_headers[row].value < 0) || (row_headers[row].value > row_headers[row].upper_bound)) {
				size_t pivot;
				if(constants[row] < 0) {
					pivot = find_pivot_column(row, tableau, 1);
				}
				if(constants[row] > lhs_coefficients[row] * row_headers[row].upper_bound) {
					pivot = find_pivot_column(row, tableau, -1);
				}
				if(pivot >= tableau.column_headers.size()) {
					tableau.stats->status = SolutionStatus.INFEASIBLE;
					return;
				}
				tableau.pivot(row, pivot);
				done = false;
				break;
			}
		}
	}
}


#endif

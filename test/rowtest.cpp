#include<iostream>
#include<cassert>
#include"../src/row.hpp"

int main(void) {
	// Constructor
	Row<int> test_row = Row<int>();

	// Copy constructor
	Row<int> other_test_row = Row<int>(test_row);

	// Setter
	test_row[5] = 5;

	// Getter
	assert(test_row[5] == 5);
	assert(other_test_row[5] == 0);

	// In-place sum
	other_test_row += test_row;
	assert(other_test_row[5] == 5);
	assert(other_test_row[4] == 0);

	// Stream
	std::cout << "Should see:\n0 0 0 0 0 5\n";
	std::cout << other_test_row << '\n';

	// Out-of-place sum
	Row<int> sum = other_test_row + other_test_row;
	assert(other_test_row[5] == 5);
	assert(sum[5] == 10);
	assert(sum[3] == 0);

	// In-place product
	other_test_row *= 3;
	assert(other_test_row[5] == 15);
	assert(other_test_row[2] == 0);

	// Out-of-place product
	Row<int> product = other_test_row*5;
	assert(product[5] == 75);
	assert(product[1] == 0);
}

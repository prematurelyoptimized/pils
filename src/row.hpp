#ifndef PILS_ROW
#define PILS_ROW

#include<vector>
#include<ostream>

template<typename int_type> 
class Row;

template<typename int_type>
std::ostream& operator<<(std::ostream& stream, const Row<int_type>& row) {
	for(auto i = row.data.begin(); i != row.data.end(); ++i) {
		stream << *i << ' ';
	}
	return stream;
}

template<typename int_type>
class Row {
	public:

	Row(void);
	Row(size_t size);
	Row(const Row<int_type>& other);

	Row<int_type> operator+(const Row<int_type>& other) const;
	Row<int_type>& operator+=(const Row<int_type>& other);

	Row<int_type> operator*(const int_type& coefficient) const;
	Row<int_type>& operator*=(const int_type& coefficient);

	int_type operator[](const size_t index) const;
	int_type& operator[](const size_t index);

	friend std::ostream & operator<< <int_type> (std::ostream& stream, const Row& row);

	private:

	std::vector<int_type> data;
	size_t last_non_zero;

};

template<typename int_type>
Row<int_type>::Row(void) {
	// Default construction of all fields is sufficient
}

template<typename int_type>
Row<int_type>::Row(size_t size) {
	data.resize(size);
}

template<typename int_type>
Row<int_type>::Row(const Row<int_type>& other) {
	data = other.data;
	last_non_zero = other.last_non_zero;
}

template<typename int_type>
Row<int_type> Row<int_type>::operator+(const Row<int_type>& other) const {
	Row retval = Row(*this);
	retval += other;
	return retval;
}

template<typename int_type>
Row<int_type>& Row<int_type>::operator+=(const Row<int_type>& other) {
	if(other.last_non_zero >= data.size()) {
		data.resize(other.last_non_zero + 1);
	}
	for(size_t i = 0; i <= other.last_non_zero; ++i) {
		data[i] += other[i];
		if(data[i] != 0) {
			last_non_zero = i;
		}
	}
	return *this;
}

template<typename int_type>
Row<int_type> Row<int_type>::operator*(const int_type& coefficient) const {
	Row retval = Row(*this);
	retval *= coefficient;
	return retval;
}

template<typename int_type>
Row<int_type>& Row<int_type>::operator*=(const int_type& coefficient) {
	for(auto i = data.begin(); i != data.end(); ++i) {
		(*i) *= coefficient;
	}
	if(coefficient == 0) {
		last_non_zero = 0;
	}
	return *this;
}

template<typename int_type>
int_type Row<int_type>::operator[](const size_t index) const {
	if(index <= last_non_zero) {
		return data[index];
	}
	return 0;
}

template<typename int_type>
int_type& Row<int_type>::operator[](const size_t index) {
	if(index >= data.size()) {
		data.resize(index+1);
		last_non_zero = index;
	}
	return data[index];
}

#endif

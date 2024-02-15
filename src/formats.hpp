#ifndef PILS_FORMATS
#define PILS_FORMATS

#include<fstream>
#include<string>
#include<stdexcept>
#include<utility>
#include<cstdlib>
#include<unordered_map>
#include"problem.hpp"
#include"constraint.hpp"

enum MPS_STATE { NONE, ROWS, COLUMNS, RHS, BOUNDS };

// From Evan Teran's answer at
// https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
inline std::string rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
	return s;
}

template<typename int_type>
Term<int_type> parse_term(const std::string& str, 
						  std::shared_ptr<Variable<int_type>> var, 
						  const std::unordered_map<std::string, std::pair<Constraint<int_type>, SENSE>>& constraint_map, 
						  bool invert=false) {
	std::string name = rtrim(str.substr(0,10));
	long coeff = std::atoi(str.substr(10).c_str());
	if(coeff != 0) {
		Constraint<int_type> constraint = constraint_map[name];
		constraint.rhs.push_back(Term<int_type>(var, coeff * (invert ? -1 : 1)));
	} else {
		throw std::runtime_error("Could not parse coefficient");
	}
}

template<typename int_type, int_type(*gcd)(int_type,int_type), int_type(*abs)(int_type)>
Problem<int_type, gcd, abs> read_mps(std::string filename) {
	Problem<int_type, gcd, abs> retval;
	
	std::ifstream file(filename);
	if(file.is_open) {
		std::string line;
		MPS_STATE state = NONE;
		std::unordered_map<std::string, std::pair<Constraint<int_type>, SENSE>> constraint_map;
 
		while(std::getline(file, line)) {
			if(line.substr(0,4) == "NAME" || line.substr(0,4) == "name") {
				// Ignore the name
				continue;
			}
			if(line.substr(0,4) == "ROWS" || line.substr(0,4) == "rows") {
				state = ROWS;
				continue;
			}
			if(line.substr(0,7) == "COLUMNS" || line.substr(0,7) == "columns") {
				state = COLUMNS;
				continue;
			}
			if(line.substr(0,3) == "RHS" || line.substr(0,3) == "rhs") {
				state = RHS;
				continue;
			}
			if(line.substr(0,6) == "BOUNDS" || line.substr(0,6) == "bounds") {
				state = BOUNDS;
				continue;
			}
			switch(state) {
				case NONE:
					// Skip any junk at the beginning that isn't in a section
					continue;
				case ROWS:
					SENSE sense;
					if(line[1] == 'N' || line[1] == 'n') {
						sense = OTHER;
					} else if (line[1] == 'G' || line[1] == 'g') {
						sense = GREATER_THAN;
					} else if (line[1] == 'L' || line[1] == 'l') {
						sense = LESS_THAN;
					} else if (line[1] == 'E' || line[1] == 'e') {
						sense = EQUALS;
					} else {
						throw std::runtime_error("Unrecognized sense");
					}
					std::string name = rtrim(line.substr(5));
					constraint_map.emplace(name, std::make_pair(Constraint<int_type>(retval.addVariable(name, 0)), sense));
					break;
				case COLUMNS:
					std::string name = rtrim(line.substr(5,15));
					std::shared_ptr<Variable<int_type>> var = retval.addVariable(name, 0, true);
					
					parse_term(line.substr(15,25), var, constraint_map);
					if(line.size() > 40) {
						parse_term(line.substr(40), var, constraint_map);
					}
					break;
				case RHS:
					parse_term(line.substr(15,25), std::make_shared<Variable<int_type>>("ONE", 1, 1), constraint_map, true);
					if(line.size() > 40) {
						parse_term(line.substr(40), std::make_shared<Variable<int_type>>("ONE", 1, 1), constraint_map, true);
					}
					break;
				case BOUNDS:
					htodi
					break;
			}
		}
		// Update bounds on constraints and make sure sense is correct
		nthdai
	} else {
		throw std::runtime_error("Failed to open file");
	}
}

#endif

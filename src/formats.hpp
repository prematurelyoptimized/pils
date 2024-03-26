#ifndef PILS_FORMATS
#define PILS_FORMATS

#include<fstream>
#include<string>
#include<stdexcept>
#include<utility>
#include<cstdlib>
#include<limits>
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
void parse_term(const std::string& str, 
				std::shared_ptr<Variable<int_type>> var, 
				std::unordered_map<std::string, std::pair<Constraint<int_type>, SENSE>>& constraint_map, 
				bool invert=false) {
	std::string name = rtrim(str.substr(0,10));
	long coeff = std::atoi(str.substr(10).c_str());
	if(coeff != 0) {
		Constraint<int_type>& constraint = constraint_map.at(name).first;
		constraint.rhs.push_back(Term<int_type>(var, coeff * (invert ? -1 : 1)));
	} else {
		throw std::runtime_error("Could not parse coefficient");
	}
}

template<typename int_type>
Problem<int_type> read_mps(std::string filename, int_type default_upper_bound = 0) {
	Problem<int_type> retval;
	
	std::ifstream file(filename);
	if(file.is_open()) {
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
			if(line.substr(0,6) == "ENDATA" || line.substr(0,6) == "endata") {
				state = NONE;
				continue;
			}
			if(line.substr(4,4) == "MARK" || line.substr(4,4) == "mark") {
				// Ignore marker lines
				continue;
			}
			switch(state) {
				case NONE:
					// Skip any junk at the beginning that isn't in a section
					continue;
				case ROWS: {
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
					std::string row_name = rtrim(line.substr(4));
					constraint_map.emplace(row_name, std::make_pair(Constraint<int_type>(std::make_shared<Variable<int_type>>(row_name, 0)), sense));
					break;
				}
				case COLUMNS: {
					std::string col_name = rtrim(line.substr(4,10));
					std::shared_ptr<Variable<int_type>> var = retval.addVariable(col_name, default_upper_bound, true);
					
					parse_term(line.substr(14,35), var, constraint_map);
					if(line.size() > 40) {
						parse_term(line.substr(39), var, constraint_map);
					}
					break;
				}
				case RHS: {
					parse_term(line.substr(14,35), Variable<int_type>::ONE, constraint_map, true);
					if(line.size() > 40) {
						parse_term(line.substr(39), Variable<int_type>::ONE, constraint_map, true);
					}
					break;
				}
				case BOUNDS: {
					std::string name = rtrim(line.substr(14,10));
					long coeff = std::atol(line.substr(24).c_str());
					if(line.substr(1,2) == "UP" || line.substr(1,2) == "up") {
						if(coeff < 0) {
							throw std::runtime_error("Negative upper bounds not supported");
						}
						std::shared_ptr<Variable<int_type>> var = retval.getVariable(name);
						var->upper_bound = coeff;
					} else if (line.substr(1,2) == "LO" || line.substr(1,2) == "lo" || line.substr(1,2) == "LI" || line.substr(1,2) == "li") {
						if(coeff != 0) {
							throw std::runtime_error("Non-zero lower bounds not supported");
						}
					} else {
						throw std::runtime_error("Unrecognized bound");
					}
					break;
				}
			}
		}

		// Update bounds on constraints and make sure sense is correct
		for(auto it = constraint_map.begin(); it != constraint_map.end(); ++it) {
			SENSE sense = it->second.second;
			Constraint<int_type>& constraint = it->second.first;
			if(sense == LESS_THAN) {
				// Remember that the MPS format puts constants on the right-hand side,
				// But our convention is to put them on the left-hand side.
				for(auto inner_it = constraint.rhs.begin(); inner_it != constraint.rhs.end(); ++inner_it) {
					inner_it->coefficient = inner_it->coefficient * -1;
				}
			}
			if(sense == OTHER) {
				// This is the objective function
				if(retval.objective.has_value()) {
					// According to the spec, the first non-constraining row is the
					// objective function and the rest can be ignored.
					continue;
				} else {
					retval.addObjective(constraint);
				}
			} else {
				if(sense == EQUALS) {
					constraint.lhs.variable->upper_bound = 0;
				} else {
					constraint.lhs.variable->upper_bound = compute_bound(constraint.rhs);
				}
				retval.addConstraint(constraint);
			}
		}
	} else {
		throw std::runtime_error("Failed to open file");
	}

	return retval;
}

#endif

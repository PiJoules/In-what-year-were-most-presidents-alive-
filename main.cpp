#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <string>
#include <vector>
#include <sstream>


std::regex YEAR_PAT("\\d{4}");


void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


/**
 * Trim whitespace from string.
 */
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}


/**
 * Get the date from an entry in the csv.
 */
int parse_date(std::string line){
	std::smatch match;
	std::regex_search(line, match, YEAR_PAT);
	if (match.empty()){
		return 9999;
	}
	return stoi(match[0]);
}


class YearRange {
	public:
		typedef std::map<int, int> year_map;

		YearRange(const year_map years=year_map()): years(years){}
		static YearRange from_start_end(const int start, const int end){
			year_map years;
			for (int i = start; i <= end; i++){
				years[i] = 1;
			}
			return YearRange(years);
		}

		year_map getYears() const{
			return years;
		}

		YearRange operator+=(const YearRange other){
			// Iterate over other year ranges' years
			for (auto const& x : other.getYears()){
				int year = x.first;
				char count = x.second;
				if (years.find(year) == years.end()){
					// Not found
					years[year] = count;
				}
				else {
					// Found
					years[year] += count;
				}
			}

			return YearRange(years);
		}
	private:
		year_map years;
};


int main(int argc, char* argv[]){
	YearRange yr;
	for (std::string line; std::getline(std::cin, line); ){
		line = trim(line);
		if (line.empty()){
			continue;
		}
		std::vector<std::string> parts = split(line, ',');
		int born = parse_date(trim(parts[1]));
		int death = parse_date(trim(parts[3]));
		yr += YearRange::from_start_end(born, death);
	}

	YearRange::year_map m = yr.getYears();
	auto x = std::max_element(m.begin(), m.end(),
	    [](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
	        return p1.second < p2.second;
	    });
	std::cout << x->first << "," << x->second << std::endl;
}
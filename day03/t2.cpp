#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using std::cin, std::cout, std::string, std::vector, std::pair;

// still keep as a stirng for now
pair<size_t,size_t> ParseIntBoundaries(string& line, size_t idx) {
    size_t start_idx = idx;
    size_t end_idx = idx;

    while (start_idx > 0 && std::isdigit(line[start_idx-1])) {
        start_idx--;
    }

    while (end_idx < line.size() -1 && std::isdigit(line[end_idx+1])) {
        end_idx++;
    }

    cout << "Parsed: " << line.substr(start_idx, end_idx-start_idx+1) << "\n";
    return {start_idx, end_idx};
}

bool IsSymbol(string& line, size_t idx) {
    return !(std::isdigit(line[idx]) || line[idx] == '.');
}

int main() {
    string filename;
    cout << "Enter input file name: \n";
    cin >> filename;
    std::ifstream input(filename);

    if (!input.is_open()) {
        cout << "File was not opened correctly\n";
        return -1;
    }

    uint32_t sum = 0;
    string line;
    vector<string> lines;

    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    //
    for (int lines_idx = 0; lines_idx < lines.size(); lines_idx++) {
        string& curr_line = lines[lines_idx];

        for (size_t cline_idx = 0; cline_idx < curr_line.size();cline_idx++) {
            if (IsSymbol(curr_line, cline_idx)) {
                vector<string> part_nums;
                
                // search row above
                if (lines_idx > 0) {
                    for (size_t i = std::max(cline_idx-1, 0ul); i <= std::min(cline_idx+1, curr_line.size()-1);i++) {
                        if (std::isdigit(lines[lines_idx-1][i])) {
                            auto [start_idx, end_idx] = ParseIntBoundaries(lines[lines_idx-1],i);
                            part_nums.push_back(lines[lines_idx-1].substr(start_idx, end_idx-start_idx+1));
                            // skip over rest of number
                            i = end_idx;
                        }
                    }
                }
                // search left of symbol
                if (cline_idx > 0 && std::isdigit(curr_line[cline_idx-1])) {
                    auto [start_idx, end_idx] = ParseIntBoundaries(curr_line, cline_idx-1);
                    part_nums.push_back(curr_line.substr(start_idx, end_idx-start_idx+1));
                }
                // search right of symbol
                if (cline_idx < curr_line.size()-1 && std::isdigit(curr_line[cline_idx+1])) {
                    auto [start_idx, end_idx] = ParseIntBoundaries(curr_line, cline_idx+1);
                    part_nums.push_back(curr_line.substr(start_idx, end_idx-start_idx+1));      
                }
                // search row below
                if (lines_idx < lines.size()-1) {
                    for (size_t i = std::max(cline_idx-1, 0ul); i <= std::min(cline_idx+1, curr_line.size()-1);i++) {
                        if (std::isdigit(lines[lines_idx+1][i])) {
                            auto [start_idx, end_idx] = ParseIntBoundaries(lines[lines_idx+1],i);
                            part_nums.push_back(lines[lines_idx+1].substr(start_idx, end_idx-start_idx+1));
                            // skip over rest of number
                            i = end_idx;
                        }
                    }
                }

                if (part_nums.size() == 2) {
                    cout << "Gear Found, parts: " << part_nums[0] << ", " << part_nums[1] << "\n";
                    sum += std::stoi(part_nums[0]) * std::stoi(part_nums[1]);
                }
            }
        }
    }

    cout << "sum: " << sum << "\n";
};
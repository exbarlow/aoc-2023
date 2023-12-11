#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
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

        for (int cline_idx = 0; cline_idx < curr_line.size();cline_idx++) {
            if (std::isdigit(curr_line[cline_idx])) {
                bool is_part = false;
                auto [start_idx, end_idx] = ParseIntBoundaries(curr_line, cline_idx);
                
                // left
                if (start_idx > 0) {
                    is_part = is_part || (lines_idx > 0 && IsSymbol(lines[lines_idx-1],start_idx-1));
                    is_part = is_part || IsSymbol(curr_line, start_idx-1);
                    is_part = is_part || (lines_idx < lines.size()-1 && IsSymbol(lines[lines_idx+1],start_idx-1));
                }
                
                // up and down
                for (size_t i = start_idx; i < end_idx + 1; i++) {
                    is_part = is_part || (lines_idx > 0 && IsSymbol(lines[lines_idx-1],i));
                    is_part = is_part || (lines_idx < lines.size()-1 && IsSymbol(lines[lines_idx+1],i));
                }

                // right
                if (end_idx < curr_line.size()-1) {
                    is_part = is_part || (lines_idx > 0 && IsSymbol(lines[lines_idx-1],end_idx+1));
                    is_part = is_part || IsSymbol(curr_line, end_idx+1);
                    is_part = is_part || (lines_idx < lines.size()-1 && IsSymbol(lines[lines_idx+1],end_idx+1));
                }
                if (is_part) {
                    sum += std::stoi(curr_line.substr(start_idx, end_idx-start_idx+1));
                } else {
                    cout << "Not a part: " << curr_line.substr(start_idx, end_idx-start_idx+1) << "\n";
                }
                // advance past the number
                cline_idx = end_idx;
            }
        }
    }

    cout << "sum: " << sum << "\n";
};
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <set>

using std::string, std::cout, std::cin;

int main() {
    string filename;
    cout << "Enter input file name: \n";
    cin >> filename;
    std::ifstream input(filename);

    if (!input.is_open()) {
        cout << "File was not opened correctly\n";
        return -1;
    }
    string line;
    uint32_t sum = 0;
    std::regex num_regex("((\\d+))");

    while (std::getline(input, line)) {
        size_t col_idx = line.find_first_of(':');
        size_t bar_idx = line.find_first_of('|');

        std::sregex_iterator witer(line.begin()+col_idx,line.begin()+bar_idx, num_regex);
        std::sregex_iterator end;

        std::set<int> winning_nums;
        while (witer != end) {
            winning_nums.insert(std::stoi((*witer)[0]));
            ++witer;
        }
        
        std::sregex_iterator hiter(line.begin()+bar_idx, line.end(), num_regex);
        std::unordered_set<int> our_nums;
        while (hiter != end) {
            our_nums.insert(std::stoi((*hiter)[0]));
            ++hiter;
        }

        int matches = 0;
        for (const int wnum : winning_nums) {
            if (our_nums.count(wnum)) {
                matches++;
            }
        }
        if (matches > 0) {
            sum += static_cast<uint32_t>(pow(2, matches-1));
        }

    }
    cout << "sum: " << sum << "\n";
}

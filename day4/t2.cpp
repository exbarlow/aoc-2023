#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <numeric>
#include <vector>

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
    std::regex num_regex("((\\d+))");

    std::vector<int> card_matches;
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
        card_matches.push_back(matches);
    }

    std::vector<int> cards(card_matches.size(),1);

    for (size_t i = 0; i < cards.size(); i++) {
        int matches = card_matches[i];
        for (int j = std::min(i+1, cards.size()); j < std::min(i+matches+1, cards.size()); j++) {
            cards[j] += cards[i];
        }
    }

    cout << "sum: " << std::accumulate(cards.begin(),cards.end(),0) << "\n";
}

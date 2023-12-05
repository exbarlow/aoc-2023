#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <vector>
#include <chrono>

using std::string, std::cout, std::cin;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage is ./task <filename>" <<"\n";
        return 0;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    std::ifstream input(argv[1]);

    if (!input.is_open()) {
        cout << "File was not opened correctly\n";
        return -1;
    }
    string line;

    std::vector<int> card_matches;
    while (std::getline(input, line)) {
        size_t col_idx = line.find_first_of(':');
        size_t bar_idx = line.find_first_of('|');


        std::stringstream win_stream(line.substr(col_idx+1, bar_idx-col_idx-1));
        std::unordered_set<int> winning_nums;
        string curr;
        while (win_stream >> curr) {
            winning_nums.insert(std::stoi(curr));
        }

        std::stringstream have_stream(line.substr(bar_idx+1));
        std::unordered_set<int> have_nums;
        while (have_stream >> curr) {
            have_nums.insert(std::stoi(curr));
        }

        int matches = 0;
        for (const int wnum : winning_nums) {
            if (have_nums.count(wnum)) {
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

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    cout << "sum: " << std::accumulate(cards.begin(),cards.end(),0) << "\n";
    cout << "program runtime: " << duration.count() << " ms" << "\n";
}

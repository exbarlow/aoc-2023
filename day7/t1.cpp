#include <fstream>
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <utility>
#include <sstream>
#include <algorithm>

using std::string, std::cout, std::cin, std::vector, std::pair;

enum class HandType {
    High_card,
    One_pair,
    Two_pair,
    Three_of_a_kind,
    Full_house,
    Four_of_a_kind,
    Five_of_a_kind
};

class Hand {
public:
    Hand(string card_str, uint64_t bid): cards(card_str), bid(bid) {
        std::unordered_map<char, int> counts;
        for (const char& c : cards) {
            counts[c]++;
        }
        // sort by count descending
        vector<pair<char, int>> elems(counts.begin(),counts.end());
        std::sort(elems.begin(),elems.end(), [](const pair<char,int>& lhs, const pair<char,int>& rhs){
            return lhs.second > rhs.second;
        });

        if (elems[0].second == 5) {
            type = HandType::Five_of_a_kind;
        } else if (elems[0].second == 4) {
            type = HandType::Four_of_a_kind;
        } else if (elems[0].second == 3 && elems[1].second == 2) {
            type = HandType::Full_house;
        } else if (elems[0].second == 3) {
            type = HandType::Three_of_a_kind;
        } else if (elems[0].second == 2 && elems[1].second == 2) {
            type = HandType::Two_pair;
        } else if (elems[0].second == 2) {
            type = HandType::One_pair;
        } else {
            type = HandType::High_card;
        }
    };

    bool operator <(const Hand& rhs) const {
        if (type == rhs.type) {
            for (int i = 0; i < 5; i++) {
                if (cards[i] != rhs.cards[i]) {
                    return GetCardValue(cards[i]) < GetCardValue(rhs.cards[i]);
                }
            }
            return false;
        } else {
            return type < rhs.type;
        }

    }

    static int GetCardValue(char c) {
        if (std::isdigit(c)) {
            return c - '0';
        }
        switch (c) {
            case 'T':
                return 10;
            case 'J':
                return 11;
            case 'Q':
                return 12;
            case 'K':
                return 13;
            case 'A':
                return 14;
            default:
                return 0;
        }
    }

    uint64_t WinAmount(int rank) {
        return rank * bid;
    }

private:
    string cards;
    uint64_t bid;
    HandType type;
};
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
    vector<Hand> hands;
    while (std::getline(input, line)) {
        std::stringstream stream(line);
        string cards;
        string bid;
        stream >> cards;
        stream >> bid;
        hands.push_back(Hand(cards, std::stoull(bid)));
    }

    std::sort(hands.begin(), hands.end());

    uint64_t sum = 0;
    
    for (int i = 0; i < hands.size(); i++) {
        sum += hands[i].WinAmount(i+1);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    if (duration.count() == 0) {
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        cout << "program runtime: " << duration_us.count() << " us" << "\n";
    } else {
        cout << "program runtime: " << duration.count() << " ms" << "\n";
    }

    cout << "sum: " << sum << "\n";

}
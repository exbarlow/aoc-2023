#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>
#include <algorithm>

using std::string, std::cout, std::cin, std::vector;

void ReadNums(string s, vector<int>& vec) {
    string num;
    std::stringstream stream(s);

    stream >> num;
    while (stream >> num) {
        vec.push_back(std::stoi(num));
    }

}

template <typename T>
void PrintVector(const vector<T>& vec) {
    cout << "[";
    for (const auto& e : vec) {
        cout << e << ",";
    }
    cout << "]\n";
}

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

    vector<int> times;
    vector<int> distances;
    string times_str;
    string dist_str;

    std::getline(input, times_str);
    std::getline(input, dist_str);

    ReadNums(times_str, times);
    ReadNums(dist_str, distances);

    vector<long> possible_wins(times.size(),0);

    for (int i = 0; i < times.size(); i++) {
        int race_dur = times[i];
        long first_win = std::ceil(0.5 * (-1 * std::sqrt(race_dur * race_dur - 4 * distances[i]) + race_dur));
        long last_win = std::floor(0.5 * (std::sqrt(race_dur * race_dur - 4 * distances[i]) + race_dur));

        if (last_win * (race_dur-last_win) == distances[i]) {
            last_win--;
        }

        if (first_win * (race_dur-first_win) == distances[i]) {
            first_win++;
        }

        cout << "time: " << times[i] << std::endl;
        cout << first_win << std::endl;
        cout << last_win << std::endl;
        cout << "dist: " << distances[i] << std::endl;

        possible_wins[i] = last_win-first_win+1;
    }

    uint64_t ans = 1;

    PrintVector(possible_wins);

    for (const long& w : possible_wins) {
        ans *= w;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    if (duration.count() == 0) {
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        cout << "program runtime: " << duration_us.count() << " us" << "\n";
    } else {
        cout << "program runtime: " << duration.count() << " ms" << "\n";
    }

    cout << "ans: " << ans << std::endl;

}
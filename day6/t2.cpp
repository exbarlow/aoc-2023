#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>
#include <algorithm>

using std::string, std::cout, std::cin, std::vector;

// void ReadNums(string s, vector<int>& vec) {
//     string num;
//     std::stringstream stream(s);

//     stream >> num;
//     while (stream >> num) {
//         vec.push_back(std::stoi(num));
//     }

// }

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

    uint64_t time;
    uint64_t dist;
    string times_str;
    string dist_str;

    std::getline(input, times_str);
    std::getline(input, dist_str);

    times_str = times_str.substr(times_str.find_first_of(':')+1);
    dist_str = dist_str.substr(dist_str.find_first_of(':')+1);

    // remove whitespace
    times_str.erase(remove_if(times_str.begin(), times_str.end(), isspace), times_str.end());
    dist_str.erase(remove_if(dist_str.begin(), dist_str.end(), isspace), dist_str.end());

    time = std::stoull(times_str);
    dist = std::stoull(dist_str);

    cout << time << std::endl;
    cout << dist << std::endl;

    uint64_t first_win = 0;
    uint64_t last_win = 0;
    
    for (uint64_t i = 1; i < time; i++) {
        if ((i * (time-i)) > dist) {
            first_win = i;
            break;
        }
    }

    for (uint64_t i = time-1; i > 0; i--) {
        if ((i * (time-i)) > dist) {
            last_win = i;
            break;
        }
    }

    uint64_t ans = last_win-first_win+1;

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
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <vector>

using std::string, std::cout, std::cin, std::vector;

template <typename T>
vector<T> ParseNums(string& s) {
    vector<T> res;
    std::stringstream stream(s);
    string temp;
    while (stream >> temp) {
        res.push_back(static_cast<T>(std::stol(temp)));
    }
    return res;
}

template <typename T>
bool VectorIsAllZero(const vector<T>& vec) {
    for (const T& elem : vec) {
        if (elem != 0) {
            return false;
        }
    }
    return true;
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

    long sum = 0l;
    string line;
    while (std::getline(input, line)) {
        vector<long> seq = ParseNums<long>(line);
        vector<vector<long>> reduced_seqs;
        reduced_seqs.push_back(seq);

        while (true) {
            vector<long>& curr = reduced_seqs.back();
            if (VectorIsAllZero(curr)) {
                break;
            }
            vector<long> next_seq;
            for (int i = 0; i < curr.size()-1; i++) {
                next_seq.push_back(curr[i+1]-curr[i]);
            }
            reduced_seqs.push_back(next_seq);
        }

        long to_subtract = 0;
        for (int i = reduced_seqs.size()-2; i >= 0; i--) {
            vector<long>& curr = reduced_seqs[i];
            to_subtract = curr[0] - to_subtract;
        }
        sum +=  to_subtract;
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
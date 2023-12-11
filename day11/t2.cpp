#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>

using std::string, std::cout, std::cin, std::vector;

template <typename T>
void PrintVector(const vector<T>& vec) {
    cout << "[";
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i != vec.size()-1) {cout << ",";}
    }
    cout << "]\n";
}

class Coordinate {
    public:
        Coordinate(int row, int col): row(row), col(col){}

        int Distance(const Coordinate& other) {
            return std::abs(row-other.row) + std::abs(col-other.col);
        }

        int row;
        int col;
};

long DoubledInRange(const vector<int>& doubled, int n1, int n2) {
    if (n1 > n2) {
        int temp = n1;
        n1 = n2;
        n2 = temp;
    }
    int count = 0;
    for (int i = 0; i < doubled.size(); i++) {
        if (n1 <= doubled[i] && doubled[i] <= n2) {
            count++;
        } else if (doubled[i] > n2) {
            break;
        }
    }
    return count * (1000000-1);
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

    vector<vector<char>> grid;
    string line;

    while (std::getline(input, line)) {
        vector<char> curr_line;
        for (int i = 0; i < line.size(); i++) {
            curr_line.push_back(line[i]);
        }
        grid.push_back(curr_line);
    }

    vector<int> doubled_rows;
    vector<int> doubled_cols;
    vector<Coordinate> galaxies;

    for (int row = 0; row < grid.size(); row++) {
        bool empty = true;
        for (int col = 0; col < grid[0].size(); col++) {
            if (grid[row][col] == '#') {
                galaxies.push_back(Coordinate(row, col));
                empty = false;
            }
        }
        if (empty) {
            doubled_rows.push_back(row);
        }
    }

    for (int col = 0; col < grid[0].size(); col++) {
        bool empty = true;
        for (int row = 0; row < grid.size(); row++) {
            if (grid[row][col] == '#') {
                empty = false;
            }
        }
        if (empty) {
            doubled_cols.push_back(col);
        }
    }

    long sum = 0;
    for (int i = 0; i < galaxies.size(); i++) {
        Coordinate galaxy1 = galaxies[i];
        for (int j = i+1; j < galaxies.size(); j++) {
            Coordinate galaxy2 = galaxies[j];
            sum += galaxy1.Distance(galaxy2) + DoubledInRange(doubled_rows, galaxy1.row, galaxy2.row) + DoubledInRange(doubled_cols, galaxy1.col, galaxy2.col);
        }
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
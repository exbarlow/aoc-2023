#include <fstream>
#include <iostream>

int main() {

    std::string filename;
    std::cout << "Enter input file name: \n";
    std::cin >> filename;
    std::ifstream input(filename);

    if (!input.is_open()) {
        std::cout << "File was not opened correctly\n";
        return -1;
    }
    
    int sum = 0;
    std::string line;
    while (std::getline(input, line)) {
        int n1 = -1;
        int n2 = -1;

        for (const char& c : line) {
            if (isdigit(c)) {
                if (n1 == -1) {
                    n1 = c - '0';
                }
                n2 = c - '0';
            }
        }
        sum += n1*10 + n2;
    }
    std::cout << "sum: " << sum << "\n";
}
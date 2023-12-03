#include <fstream>
#include <iostream>
#include <vector>
using std::vector, std::string, std::ifstream, std::cin, std::cout;

vector<string> digit_strings({
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
});

int get_digit(string& line, std::size_t i) {
    // normal digit
    if (std::isdigit(line[i])) {
        return line[i] - '0';
    }

    for (int d = 0; d < digit_strings.size(); d++) {
        if (i + digit_strings[d].size() > line.size()) {
            continue;
        }
        // this is probably not great
        // cout << line.substr(i, digit_strings[d].size()) << "\n";
        if (line.substr(i, digit_strings[d].size()) == digit_strings[d]) {
            return d;
        }
    }
    return -1;
}

int main() {
    string filename;
    cout << "Enter input file name: \n";
    cin >> filename;
    ifstream input(filename);

    if (!input.is_open()) {
        cout << "File was not opened correctly\n";
        return -1;
    }

    string line;
    int sum = 0;
    while (std::getline(input, line)) {
        int n1 = -1;
        int n2 = -1;
        for (int i = 0; i < line.size(); i++) {
            int digit = get_digit(line, i);
            if (digit != -1) {
                if (n1 == -1) {
                    n1 = digit;
                }
                n2 = digit;
            }
        }
        sum += n1*10 + n2;
    }
    cout << "sum: " << sum << "\n";
}
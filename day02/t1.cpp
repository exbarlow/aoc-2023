#include <fstream>
#include <iostream>
using std::cin, std::cout;

int ParseNumColor(std::string& line, size_t start_idx, std::string color) {
    size_t stop_idx = line.find_first_of(';',start_idx);
    if (stop_idx == std::string::npos) {
        stop_idx = line.size()-1;
    }
    size_t color_idx = line.find(color, start_idx);
    // if color not in the showing, return 0
    if (color_idx == std::string::npos || color_idx > stop_idx) {
        return 0;
    }

    size_t num_start = color_idx - 1;
    while (std::isdigit(line[num_start-1])) {
        num_start--;
    }
    int ans = std::stoi(line.substr(num_start, color_idx-num_start-1));
    return ans;
}

int main() {
    std::string filename;
    cout << "Enter input file name: \n";
    cin >> filename;
    std::ifstream input(filename);

    if (!input.is_open()) {
        cout << "File was not opened correctly\n";
        return -1;
    }
    std::string line;
    int sum = 0;
    int game_num = 1;

    int red_max = 12;
    int blue_max = 14;
    int green_max = 13;

    while (std::getline(input, line)) {
        size_t start_idx = 0;
        bool possible = true;
        while (true) {
            int red = ParseNumColor(line, start_idx, "red");
            int blue = ParseNumColor(line, start_idx, "blue");
            int green = ParseNumColor(line, start_idx, "green");
            if (red > red_max || blue > blue_max || green > green_max) {
                possible = false;
                break;
            }
            size_t next_start = line.find_first_of(';',start_idx);
            if (next_start != std::string::npos) {
                start_idx = next_start + 1;
            } else {
                break;
            }
        }
        if (possible) {
            sum += game_num;
        }
        game_num++;
    }

    cout << "sum: " << sum << "\n";
}

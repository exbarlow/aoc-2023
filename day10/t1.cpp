#include <fstream>
#include <iostream>
#include <chrono>
#include <utility>
#include <vector>
#include <map>

using std::string, std::cout, std::cin, std::pair, std::vector, std::map;

class Coords {
    public:
        Coords(int row, int col): row(row), col(col){}

        Coords Up() {
            return Coords(row-1, col);
        }
        Coords Down() {
            return Coords(row+1, col);
        }
        Coords East() {
            return Coords(row, col+1);
        }
        Coords West() {
            return Coords(row, col-1);
        }

        void Print() {
            cout << "(" << row << "," << col << ")";
        }

        friend bool operator< (const Coords& lhs, const Coords& rhs) {
            if (lhs.row == rhs.row) {
                return lhs.col < rhs.col;
            } else {
                return lhs.row < rhs.row;
            }
        }

    private:
        int row;
        int col;
};

class Pipe {
    public:
        Pipe(char type, int row, int col): 
            pipe_type(type), 
            connects_down(false), 
            connects_up(false), 
            connects_east(false), 
            connects_west(false),
            coords(row, col), 
            links({nullptr, nullptr})
        {
            if (pipe_type == '7' || pipe_type == '|' || pipe_type == 'F') {
                connects_down = true;
            }
            if (pipe_type == '|' || pipe_type == 'L' || pipe_type == 'J') {
                connects_up = true;
            }
            if (pipe_type == '-' || pipe_type == 'J' || pipe_type == '7') {
                connects_west = true;
            }
            if (pipe_type == '-' || pipe_type == 'F' || pipe_type == 'L') {
                connects_east = true;
            }
        }

        // todo: need to fix the S bool stuff before this gets run
        pair<Coords, Coords> GetNextCoords() {
            vector<Coords> next;
            if (connects_down) {
                next.push_back(coords.Down());
            }
            if (connects_up) {
                next.push_back(coords.Up());
            }
            if (connects_east) {
                next.push_back(coords.East());
            }
            if (connects_west) {
                next.push_back(coords.West());
            }
            return {next[0], next[1]};
        }

        void PrintConnections() {
            cout << "up: " << connects_up << " down: " << connects_down << " east: " << connects_east << " west: " << connects_west << "\n";
        }

        char pipe_type;
        bool connects_down;
        bool connects_up;
        bool connects_east;
        bool connects_west;
        Coords coords;
        pair<Pipe*, Pipe*> links;
};

void FillSConnects(Pipe* s_pipe, const map<Coords, Pipe*>& m) {
    Coords s_coords = s_pipe->coords;
    // if there is a pipe above the S and it connects downward, then S must connect upwards
    if (m.count(s_coords.Up()) && m.at(s_coords.Up())->connects_down) {
        s_pipe->connects_up = true;
    }
    if (m.count(s_coords.Down()) && m.at(s_coords.Down())->connects_up) {
        s_pipe->connects_down = true;
    }
    if (m.count(s_coords.East()) && m.at(s_coords.East())->connects_west) {
        s_pipe->connects_east = true;
    }
    if (m.count(s_coords.West()) && m.at(s_coords.West())->connects_east) {
        s_pipe->connects_west = true;
    }
}

void PrintCoordMap(const map<Coords,Pipe*>& m) {
    for (auto& [c, p] : m) {
        cout << p->pipe_type << " at ";
        p->coords.Print();
        cout << "\n";
    } 
}

void FreeCoordMapMemory(map<Coords,Pipe*>& m) {
    for (auto& [c, p] : m) {
        delete p;
    }
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

    vector<vector<char>> symbols;
    string line;
    map<Coords, Pipe*> coord_map;
    Coords s_coords = Coords(-1, -1);

    while (std::getline(input, line)) {
        vector<char> curr_line_chars;
        for (int i = 0; i < line.size(); i++) {
            curr_line_chars.push_back(line[i]);
        }
        symbols.push_back(curr_line_chars);
    }


    for (int row = 0; row < symbols.size(); row++) {
        for (int col = 0; col < symbols[row].size(); col++) {
            char c = symbols[row][col];
            if (c == '.') {continue;}
            Pipe* new_pipe = new Pipe(c, row, col);
            if (c == 'S') {
                s_coords = Coords(row, col);
            }
            coord_map[{row, col}] = new_pipe;
        }
    }

    FillSConnects(coord_map[s_coords], coord_map);

    for (auto& [c, p] : coord_map) {
        pair<Coords, Coords> next_coords = p->GetNextCoords();
        if (coord_map.count(next_coords.first)) {
            p->links.first = coord_map.at(next_coords.first);
        }
        if (coord_map.count(next_coords.second)) {
            p->links.second = coord_map.at(next_coords.second);
        }
    }

    int counter = 1;
    Pipe* s_pipe = coord_map[s_coords];
    Pipe* curr_pipe = s_pipe->links.second;
    Pipe* last_pipe = s_pipe;
    while (true) {
        if (curr_pipe == s_pipe) {break;}

        counter++;
        Pipe* next_pipe = curr_pipe->links.first;

        if (next_pipe == last_pipe) {
            next_pipe = curr_pipe->links.second;
        }
        last_pipe = curr_pipe;
        curr_pipe = next_pipe;
    }


    FreeCoordMapMemory(coord_map);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    if (duration.count() == 0) {
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        cout << "program runtime: " << duration_us.count() << " us" << "\n";
    } else {
        cout << "program runtime: " << duration.count() << " ms" << "\n";
    }

    cout << "ans: " << counter/2 << "\n";

}
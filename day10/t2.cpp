#include <fstream>
#include <iostream>
#include <chrono>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <stack>

using std::string, std::cout, std::cin, std::pair, std::vector, std::map, std::set;

class Coords {
    public:
        Coords(int row, int col): row(row), col(col){}

        Coords Up() const {
            return Coords(row-1, col);
        }
        Coords Down() const {
            return Coords(row+1, col);
        }
        Coords East() const {
            return Coords(row, col+1);
        }
        Coords West() const {
            return Coords(row, col-1);
        }

        void Print() const {
            cout << "(" << row << "," << col << ")";
        }

        friend bool operator< (const Coords& lhs, const Coords& rhs) {
            if (lhs.row == rhs.row) {
                return lhs.col < rhs.col;
            } else {
                return lhs.row < rhs.row;
            }
        }

        friend bool operator==(const Coords& lhs, const Coords& rhs) {
            return (lhs.row == rhs.row && lhs.col == rhs.col);
        }
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
            links({nullptr, nullptr}),
            in_loop(false)
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

        pair<Coords, Coords> GetNextCoords() const {
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

        void PrintConnections() const {
            cout << "up: " << connects_up << " down: " << connects_down << " east: " << connects_east << " west: " << connects_west << "\n";
        }

        char pipe_type;
        bool connects_down;
        bool connects_up;
        bool connects_east;
        bool connects_west;
        Coords coords;
        pair<Pipe*, Pipe*> links;
        bool in_loop;
};

void ReplaceSInSymbols(vector<vector<char>>& symbols, Pipe* s_pipe) {
    char c = 'S';
    if (s_pipe->connects_down && s_pipe->connects_up) {
        c = '|';
    } else if (s_pipe->connects_down && s_pipe->connects_east) {
        c = 'F';
    } else if (s_pipe->connects_down && s_pipe->connects_west) {
        c = '7';
    } else if (s_pipe->connects_up && s_pipe->connects_east) {
        c = 'L';
    } else if (s_pipe->connects_up && s_pipe->connects_west) {
        c = 'J';
    } else if (s_pipe->connects_east && s_pipe->connects_west) {
        c = '-';
    }
    symbols[s_pipe->coords.row][s_pipe->coords.col] = c;
}

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
    s_pipe->in_loop = true;
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

bool IsVerticalAlignedRow(char c1, char c2) {
    return (c1 == '|' || c1 == 'J' || c1 == '7') && (c2 == '|' || c2 == 'F' || c2 == 'L');
}

bool IsHorizontalAlignedRow(char c1, char c2) {
    return (c1 == '-' || c1 == 'L' || c1 == 'F') && (c2 == '-' || c2 == 'J' || c2 == '7');
}

bool IsVerticalAlignedCol(char c1, char c2) {
    return (c1 == '|' || c1 == 'F' || c1 == '7') && (c2 == '|' || c2 == 'J' || c2 == 'L');
}

bool IsHorizontalAlignedCol(char c1, char c2) {
    return (c1 == '-' || c1 == 'L' || c1 == 'J') && (c2 == '-' || c2 == 'F' || c2 == '7');
}

void ExpandSpacesCols(vector<vector<char>>& symbols, int& s_col, const map<Coords, Pipe*>& cmap) {
    vector<set<int>> row_wise_insert;
    set<int> total_insert;
    // figure out where we need to insert
    for (int row = 0; row < symbols.size(); row++) {
        set<int> row_insert;
        for (int col = 0; col < symbols[0].size()-1; col++) {
            char c1 = symbols[row][col];
            char c2 = symbols[row][col+1];
            if (IsVerticalAlignedRow(c1,c2)) {
                row_insert.insert(col+1);
                total_insert.insert(col+1);
            }
        }
        row_wise_insert.push_back(row_insert);
    }

    int s_col_increase = 0;
    for (int i : total_insert) {
        if (i <= s_col) {
            s_col_increase++;
        }
    }
    s_col += s_col_increase;

    for (int row = 0; row < symbols.size(); row++) {
        int offset = 0;
        vector<char>& row_vec = symbols[row];
        set<int> row_insert = row_wise_insert[row];
        set<int> all_insert = total_insert;

        vector<char> new_row_vec(row_vec.size()+total_insert.size(), '0');

        for (int i = 0; i < new_row_vec.size(); i++) {
            if (all_insert.count(i-offset)) {
                if (!row_insert.count(i-offset) && i - offset < row_vec.size() && IsHorizontalAlignedRow(row_vec[i-offset-1], row_vec[i-offset])) {
                    Pipe* pipe = cmap.at({row,i-offset-1});
                    if (pipe->in_loop) {
                        new_row_vec[i] = '-';
                    } else {
                        new_row_vec[i] = '_';
                    }
                    
                } else {
                    new_row_vec[i] = '*';
                    row_insert.erase(i-offset);
                }
                all_insert.erase(i-offset);
                offset++;
            } else {
                new_row_vec[i] = row_vec[i-offset];
            }
        }
        row_vec = new_row_vec;
    }
}

void FillCoordMap(const vector<vector<char>>& symbols, map<Coords, Pipe*>& coord_map, Coords& s_coords){
    for (int row = 0; row < symbols.size(); row++) {
        for (int col = 0; col < symbols[row].size(); col++) {
            char c = symbols[row][col];
            if (c == '.' || c == '*' || c == '_' || c == '/') {
                continue;
            }
            if (c == 'S') {
                s_coords = {row, col};
            }
            Pipe* new_pipe = new Pipe(c, row, col);
            coord_map[{row, col}] = new_pipe;
        }
    }
}

void LinkPipes(const map<Coords, Pipe*>& coord_map) {
    for (auto& [c, p] : coord_map) {
        assert (p!=nullptr);
        pair<Coords, Coords> next_coords = p->GetNextCoords();
        if (coord_map.count(next_coords.first)) {
            p->links.first = coord_map.at(next_coords.first);
        }
        if (coord_map.count(next_coords.second)) {
            p->links.second = coord_map.at(next_coords.second);
        }
    }
}

void FindPipeLoop(Pipe* s_pipe) {
    Pipe* curr_pipe = s_pipe->links.second;
    Pipe* last_pipe = s_pipe;
    
    while (true) {
        if (curr_pipe == s_pipe) {break;}
        curr_pipe->in_loop = true;

        Pipe* next_pipe = curr_pipe->links.first;
        if (next_pipe == last_pipe) {
            next_pipe = curr_pipe->links.second;
        }
        last_pipe = curr_pipe;
        curr_pipe = next_pipe;
    }
}

void ExpandSpaceRows(vector<vector<char>>& symbols, int& s_row, const map<Coords, Pipe*>& cmap) {
    map<int, set<int>> replace;
    map<int, vector<char>> new_rows;
    // can have a map ## row : [array of cols to add *]
    for (int row = 0; row < symbols.size()-1; row++) {
        for (int col = 0; col < symbols[0].size(); col++) {
            char c1 = symbols[row][col];
            char c2 = symbols[row+1][col];
            if (IsHorizontalAlignedCol(c1,c2)) {
                replace[row+1].insert(col);
            }
        }
    }

    for (auto& [row, cols] : replace) {
        vector<char> new_row(symbols[0].size(), '0');
        for (int i = 0; i < new_row.size(); i++) {
            if (!cols.count(i) && IsVerticalAlignedCol(symbols[row-1][i],symbols[row][i])) {
                Pipe* pipe = cmap.at({row-1,i});
                if (pipe->in_loop) {
                    new_row[i] = '|';
                } else {
                    new_row[i] = '/';
                }
            } else {
                new_row[i] = '*';
            }
        }
        new_rows[row] = new_row;
    }

    int s_row_increase = 0;
    for (auto& [row, v] : new_rows) {
        if (row <= s_row) {
            s_row_increase++;
        }
    }
    s_row+=s_row_increase;

    int offset = 0;
    vector<vector<char>> new_symbols(symbols.size()+new_rows.size(), vector<char>(symbols[0].size(), '0'));
    for (int i = 0; i < new_symbols.size(); i++) {
        if (new_rows.count(i-offset)) {
            new_symbols[i] = new_rows[i-offset];
            new_rows.erase(i-offset);
            offset++;
        } else {
            new_symbols[i] = symbols[i-offset];
        }
    }
    symbols = new_symbols;
}


void PrintSymbols(const vector<vector<char>>& symbols) {
    for (const auto& v : symbols) {
        for (const auto& c : v) {
            cout << c;
        }
        cout << "\n";
    }
}

bool InBounds(const vector<vector<char>>& symbols, const Coords& coords) {
    return (coords.row >= 0 && coords.row < symbols.size() && coords.col >= 0 && coords.col < symbols[0].size());
}

bool TouchesBadChar(const vector<vector<char>>& symbols, const Coords& coords) {
    vector<Coords> neighbors({coords.Up(), coords.Down(), coords.East(), coords.West()});
    for (const auto& n : neighbors) {
        if (!InBounds(symbols, n)) {
            return true;
        }
    }
    return false;
}

bool CanRecurse(const vector<vector<char>>& symbols, const Coords& coords, const set<Coords>& visited) {
    if (!InBounds(symbols, coords)) {
        return false;
    }
    char c = symbols[coords.row][coords.col];
    return(c != 'X' && !visited.count(coords));
}
void Backtrack(const Coords& coords, vector<vector<char>>& symbols, set<Coords>& visited) {
    visited.insert(coords);

    vector<Coords> neighbors({coords.Up(), coords.Down(), coords.East(), coords.West()});

    for (const auto& n : neighbors) {
        if (CanRecurse(symbols, n, visited)) {
            Backtrack(n, symbols, visited);
        }
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

    // fill in the coord map to get original pipes
    FillCoordMap(symbols, coord_map, s_coords);
    Pipe* s_pipe = coord_map[s_coords];
    FillSConnects(s_pipe, coord_map);
    LinkPipes(coord_map);
    FindPipeLoop(s_pipe);

    ReplaceSInSymbols(symbols, s_pipe);
    ExpandSpacesCols(symbols, s_coords.col, coord_map);

    FreeCoordMapMemory(coord_map);
    coord_map.clear();

    // i think this is just easier to do, fixes needing to find new column offsets and such
    FillCoordMap(symbols, coord_map, s_coords);
    s_pipe = coord_map[s_coords];
    FillSConnects(s_pipe, coord_map);
    LinkPipes(coord_map);
    FindPipeLoop(s_pipe);

    ReplaceSInSymbols(symbols, s_pipe);
    ExpandSpaceRows(symbols, s_coords.row, coord_map);
    symbols[s_coords.row][s_coords.col] = 'S';


    // s_pipe is now dangling !
    FreeCoordMapMemory(coord_map);
    coord_map.clear();

    // fill coord map w. extra pipes
    Coords dummy({-1,-1});
    FillCoordMap(symbols, coord_map, dummy);

    s_pipe = coord_map[s_coords];

    assert(s_pipe != nullptr);

    FillSConnects(s_pipe, coord_map);
    LinkPipes(coord_map);
    FindPipeLoop(s_pipe);

    for (auto& [c, p] : coord_map) {
        if (p->in_loop) {
            symbols[c.row][c.col] = 'X';
        }
    }

    set<Coords> visited;
    std::stack<Coords> s;
    long count = 0;

    for (int row = 0; row < symbols.size(); row++) {
        for (int col = 0; col < symbols[0].size(); col++) {
            if (symbols[row][col] != 'X' && !visited.count({row,col})) {
                set<Coords> curr_group;
                Backtrack({row,col},symbols,curr_group);
        
                bool enclosed = true;
                for (const Coords& co : curr_group) {
                    visited.insert(co);
                    if (TouchesBadChar(symbols, co)) {
                        enclosed = false;
                    }
                    
                }
                if (enclosed) {
                    for (const Coords& co : curr_group) {
                        if (symbols[co.row][co.col] != '*' && symbols[co.row][co.col] != '_' && symbols[co.row][co.col] != '/') {
                            symbols[co.row][co.col] = 'O';
                            count++;
                        }
                    }
                }
            }

        }
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

    cout << "count: " << count << '\n';


}
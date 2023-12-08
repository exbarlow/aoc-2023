#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <regex>
#include <numeric>
#include <utility>
#include <queue>

using std::string, std::cout, std::cin, std::vector, std::pair;

class Node {
    public:
        Node(string name, Node* left=nullptr, Node* right=nullptr): name(name), left(left), right(right){}
        string name;
        Node* left;
        Node* right;

        void Print() {
            cout << this->name << "  " << "[" << left->name << "," << right->name << "]\n";
        }
};

template <typename T>
void PrintVector(const vector<T>& vec) {
    cout << "[";
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i != vec.size()-1) {cout << ",";}
    }
    cout << "]\n";
}

/* new approach: find how long it takes each to reach z */

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

    string instructions;
    string line;

    std::getline(input,instructions);
    // skip whitespace line
    std::getline(input, line);

    std::unordered_map<string, Node*> nodes;
    vector<vector<string>> node_strs;
    std::regex nodes_regex("(\\w{3})");

    // read in lines
    while (std::getline(input, line)) {
        auto begin = std::sregex_iterator(line.begin(), line.end(), nodes_regex);
        auto end = std::sregex_iterator();
        vector<string> curr_row;

        for (auto i = begin; i != end; ++i) {
            curr_row.push_back((*i).str());
        }
        node_strs.push_back(curr_row);  
    }

    // construct nodes without connecting yet
    for (const vector<string>& v : node_strs) {
        string node_str = v[0];
        Node* new_node = new Node(node_str);
        nodes[node_str] = new_node;
    }

    // connect nodes
    for (const vector<string>& v: node_strs) {
        Node* node = nodes[v[0]];
        node->left = nodes[v[1]];
        node->right = nodes[v[2]];
    }

    vector<Node*> curr_nodes;

    for (const auto& [key, val] : nodes) {
        if (key[2] == 'A') {
            curr_nodes.push_back(val);
        }
    }
    
    std::queue<uint64_t> z_steps;
    for (const auto& n : curr_nodes) {
        std::map<pair<string, char>, uint64_t> visited;
        uint64_t steps = 0;
        Node* curr = n;
        while (true) {
            char instr = instructions[(steps%instructions.size())];
            if (visited.count({curr->name, instr}) && curr->name[2] == 'Z') {
                uint64_t cycle_len = steps - visited[{curr->name, instr}];
                z_steps.push(cycle_len);
                break;
            }
            visited[{curr->name, instr}] = steps++;
            switch (instr) {
                case 'L':
                    curr = curr->left;
                    break;
                case 'R':
                    curr = curr->right;
                    break;
            }
        }
    }

    while (z_steps.size() > 1) {
        uint64_t z1 = z_steps.front();
        z_steps.pop();
        uint64_t z2 = z_steps.front();
        z_steps.pop();
        z_steps.push(std::lcm(z1,z2));
    }

    for (auto& [key, val] : nodes) {
        delete val;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    if (duration.count() == 0) {
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        cout << "program runtime: " << duration_us.count() << " us" << "\n";
    } else {
        cout << "program runtime: " << duration.count() << " ms" << "\n";
    }

    cout << "steps: " << z_steps.front() << "\n";
}
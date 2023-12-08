#include <fstream>
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <regex>

using std::string, std::cout, std::cin, std::vector;

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
        // cout << "node_str: " << node_str << std::endl;
        Node* new_node = new Node(node_str);
        nodes[node_str] = new_node;
    }

    // return 0;

    // connect nodes
    for (const vector<string>& v: node_strs) {
        Node* node = nodes[v[0]];
        node->left = nodes[v[1]];
        node->right = nodes[v[2]];
    }

    uint64_t steps = 0;
    Node* curr_node = nodes["AAA"];

    while (true) {
        if (curr_node->name == "ZZZ") {
            break;
        }
        // curr_node->Print();
        char instr= instructions[(steps%instructions.size())];
        // cout << "instr: " << instr << "\n";
        switch (instr) {
            case 'L':
                curr_node = curr_node->left;
                break;
            case 'R':
                curr_node = curr_node->right;
                break;
        }
        steps++;
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

    cout << "steps: " << steps << "\n";
}
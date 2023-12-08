#include <fstream>
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <map>
#include <vector>
#include <regex>
#include <utility>
#include <stack>

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

vector<uint64_t> FirstPrimesInMillionNums() {
    vector<uint64_t> sieve(1000001, 0);
    vector<uint64_t> primes;
    size_t curr = 2;
    while (curr < sieve.size()) {
        if (sieve[curr] == 1) {
            curr++;
            continue;
        }
        primes.push_back(curr);
        for (size_t mult = curr; mult < sieve.size(); mult += curr) {
            sieve[mult] = 1;
        }
        curr++;
    }
    return primes;
}

void PrintPrimeFactorization(const std::unordered_map<uint64_t,int> m) {
    cout << "starting prime factorization\n";
    for (const auto& [factor, count] : m) {
        cout << "\tfactor: " << factor << " count: " << count << '\n';
    }
}

std::unordered_map<uint64_t,int> GetPrimeFactors(uint64_t num, const vector<uint64_t>& primes) {
    size_t curr_prime_idx = 0;
    std::unordered_map<uint64_t, int> factors;
    while (num > 1) {
        uint64_t curr_prime = primes[curr_prime_idx];
        if (num % curr_prime == 0) {
            num /= curr_prime;
            factors[curr_prime]++;
        }
        curr_prime_idx++;
    }
    return factors;

};

std::unordered_map<uint64_t,int> MergeFactors(std::unordered_map<uint64_t,int>& m1, std::unordered_map<uint64_t,int>& m2) {
    std::unordered_map<uint64_t, int> merged;

    for (auto& [factor, val] : m1) {
        int other = m2[factor];
        merged[factor] = std::max(val, other);
    }

    for (auto& [factor, val] : m2) {
        int other = m1[factor];
        merged[factor] = std::max(val, other);
    }
    return merged;
}

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
    vector<uint64_t> primes = FirstPrimesInMillionNums();

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
    
    vector<uint64_t> z_steps;
    for (const auto& n : curr_nodes) {
        // cout << "processing node: ";
        // n->Print();
        std::map<pair<string, char>, uint64_t> visited;
        uint64_t steps = 0;
        Node* curr = n;
        while (true) {
            char instr = instructions[(steps%instructions.size())];
            if (visited.count({curr->name, instr}) && curr->name[2] == 'Z') {
                // cout << "cycle found on node: ";
                // curr->Print();
                uint64_t cycle_len = steps - visited[{curr->name, instr}];
                // cout << "\tcycle length: " << cycle_len << std::endl;
                z_steps.push_back(cycle_len);
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

    vector<std::unordered_map<uint64_t,int>> pfactors;
    for (uint64_t n : z_steps) {
        pfactors.push_back(GetPrimeFactors(n, primes));
    }


    std::stack<std::unordered_map<uint64_t,int>> s;
    for (auto& pf : pfactors) {
        // PrintPrimeFactorization(pf);
        s.push(pf);
    }

    while (s.size() > 1) {
        auto pf1 = s.top();
        s.pop();
        auto pf2 = s.top();
        s.pop();
        // PrintPrimeFactorization(pf1);
        // PrintPrimeFactorization(pf2);
        auto n = MergeFactors(pf1, pf2);
        // PrintPrimeFactorization(n);
        s.push(n);
    }   

    uint64_t ans = 1;

    // PrintPrimeFactorization(s.top());

    for (auto& [factor, count] : s.top()) {
        ans *= std::pow(factor, count);
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

    cout << "steps: " << ans << "\n";
}
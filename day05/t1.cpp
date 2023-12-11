#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>

using std::string, std::cout, std::cin, std::stol;

class Map {
    public:
        Map(long dest, long source, long len): source_start(source), dest_start(dest), len(len) {} 

        bool Contains(long cand) const {
            return (cand > source_start) && (cand - source_start < len);
        }

        long GetDest(long cand) const {
            return dest_start + cand - source_start;
        }

        void Print(void) const {
            cout << "Map: maps source from " << source_start << " to " << source_start + len - 1 << ", dest from " << dest_start << " to " << dest_start + len - 1 << "\n";
        }

    private:
        long source_start;
        long dest_start;
        long len;
};

class Mapping {
    public:
        Mapping() {};

        void Fill(std::ifstream& stream) {
            string line;
            while (std::getline(stream, line)) {
                if (line.empty()) {
                    return;
                }
                std::stringstream sstream(line);
                string dest;
                string src;
                string len;
                sstream >> dest;
                sstream >> src;
                sstream >> len;
                maps.push_back(Map(stol(dest),stol(src),stol(len)));
            }
        }

        long GetMappedVal(long source) {
            for (const Map& m : maps) {
                if (m.Contains(source)) {
                    return m.GetDest(source);
                }
            }
            return source;
        }
    private:
        std::vector<Map> maps;
};

void ParseSeeds(std::vector<long>& seeds, string line) {
    std::stringstream stream(line.substr(line.find_first_of(':')+1));
    string num;
    while (stream >> num) {
        seeds.push_back(std::stol(num));
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

    /* boilerplate ends here */

    string line;
    std::vector<long> seeds;
    Mapping seed_to_soil;
    Mapping soil_to_fert;
    Mapping fert_to_water;
    Mapping water_to_light;
    Mapping light_to_temp;
    Mapping temp_to_humid;
    Mapping humid_to_loc;

    while (std::getline(input, line)) {
        // skip empty lines
        if (line.empty()) {continue;}

        if (line.find("seeds:") != string::npos) {
            ParseSeeds(seeds, line);
        } else if (line.find("to-soil") != string::npos) {
            seed_to_soil.Fill(input);
        } else if (line.find("to-fert") != string::npos) {
            soil_to_fert.Fill(input);
        } else if (line.find("to-water") != string::npos) {
            fert_to_water.Fill(input);
        } else if (line.find("to-light") != string::npos) {
            water_to_light.Fill(input);
        } else if (line.find("to-temp") != string::npos) {
            light_to_temp.Fill(input);
        } else if (line.find("to-humid") != string::npos) {
            temp_to_humid.Fill(input);
        } else if (line.find("to-loc") != string::npos) {
            humid_to_loc.Fill(input);
        }

    }
    long curr_min = LONG_MAX;
    for (const long& seed : seeds) {
        long location = humid_to_loc.GetMappedVal(temp_to_humid.GetMappedVal(light_to_temp.GetMappedVal(water_to_light.GetMappedVal(fert_to_water.GetMappedVal(soil_to_fert.GetMappedVal(seed_to_soil.GetMappedVal(seed)))))));
        curr_min = std::min(curr_min, location);
    }


    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    cout << "program runtime: " << duration.count() << " us" << "\n";

    cout << "minimum:" << curr_min <<"\n";
}
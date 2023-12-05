#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>
#include <utility>

using std::string, std::cout, std::cin, std::stol, std::pair;

typedef pair<long,long> SeedRange;

void PrintRange(const SeedRange& sr) {
    cout << "[" << sr.first << "," << sr.second << "] ";
}

class Map {
    public:
        Map(long dest, long source, long len): interval({source, source+len-1}), transform(dest-source) {} 

        bool Overlaps(const SeedRange& s1) const {
            return !(s1.first > interval.second || s1.second < interval.first);
        }

        bool OvershootsBegin(const SeedRange& sr) const {
            return (Overlaps(sr) && (sr.first < interval.first));
        }

        bool OvershootsEnd(const SeedRange& sr) const {
            return (Overlaps(sr) && (sr.second > interval.second));
        }

        SeedRange GetOvershootBegin(const SeedRange& sr) const {
            return {sr.first, interval.first-1};
        }

        SeedRange GetOvershootEnd(const SeedRange& sr) const {
            return {interval.second+1, sr.second};
        }

        SeedRange TranfsormOverlap(const SeedRange& sr) const {
            return {std::max(sr.first, interval.first)+transform, std::min(sr.second, interval.second)+transform};
        }

        bool operator < (const Map& rhs) const {
            return (interval < rhs.interval);
        }

        void PrintMap(void) const {
            cout << "[" << interval.first << "," << interval.second << "," << transform << "] ";
        }
    private:
        SeedRange interval;
        long transform;
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
            sort(maps.begin(), maps.end());
        }

        std::vector<SeedRange> MapRanges(std::vector<SeedRange> ranges) {
            std::vector<SeedRange> new_ranges;

            for (size_t i = 0; i < ranges.size(); i++) {
                SeedRange seed_range = ranges[i];
                bool found = false;
                for (const Map& m : maps) {
                    if (!m.Overlaps(seed_range)) {
                        continue;
                    } else {
                        found = true;
                        new_ranges.push_back(m.TranfsormOverlap(seed_range));
                        if (m.OvershootsBegin(seed_range)) {
                            ranges.push_back(m.GetOvershootBegin(seed_range));
                        }
                        if (m.OvershootsEnd(seed_range)) {
                            ranges.push_back(m.GetOvershootEnd(seed_range));
                        }
                        break;
                    }
                }
                if (!found) {
                    new_ranges.push_back(seed_range);
                }
            }

            sort(new_ranges.begin(), new_ranges.end());
            return new_ranges;
        }

        void PrintMaps(void) const {
            cout << "Maps: ";
            for (const Map& m : maps) {
                m.PrintMap();
            }
            cout << "\n";
        }

    private:
        std::vector<Map> maps;
};

void PrintRanges(const std::vector<SeedRange>& seed_ranges) {
    for (const SeedRange& sr : seed_ranges) {
        PrintRange(sr);
        cout << std::endl;
    }
}

void ParseSeedRanges(std::vector<SeedRange>& seeds, string& line) {
    std::stringstream stream(line.substr(line.find_first_of(':')+1));
    string num;
    string range;
    while (stream >> num && stream >> range) {
        seeds.push_back({std::stol(num),std::stol(range) + std::stol(num) - 1l});
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
    std::vector<SeedRange> seed_ranges;
    Mapping seed_to_soil;
    Mapping soil_to_fert;
    Mapping fert_to_water;
    Mapping water_to_light;
    Mapping light_to_temp;
    Mapping temp_to_humid;
    Mapping humid_to_loc;

    while (std::getline(input, line)) {
        if (line.empty()) {continue;}

        if (line.find("seeds:") != string::npos) {
            ParseSeedRanges(seed_ranges, line);
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

    seed_ranges = seed_to_soil.MapRanges(seed_ranges);
    seed_ranges = soil_to_fert.MapRanges(seed_ranges);
    seed_ranges = fert_to_water.MapRanges(seed_ranges);
    seed_ranges = water_to_light.MapRanges(seed_ranges);
    seed_ranges = light_to_temp.MapRanges(seed_ranges);
    seed_ranges = temp_to_humid.MapRanges(seed_ranges);
    seed_ranges = humid_to_loc.MapRanges(seed_ranges);


    long curr_min = LONG_MAX;

    for (const SeedRange& sr: seed_ranges) {
        curr_min = std::min(curr_min, sr.first);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    if (duration.count() == 0) {
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        cout << "program runtime: " << duration_us.count() << " us" << "\n";
    } else {
        cout << "program runtime: " << duration.count() << " ms" << "\n";
    }
    

    cout << "minimum:" << curr_min <<"\n";
}
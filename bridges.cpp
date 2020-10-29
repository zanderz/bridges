//

#include <iostream>
#include <limits>
#include <fstream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// Hash the current configuration of hikers plus flashlight location (bridge)
std::size_t HikersHash(const vector<set<double>> &hikers, size_t bridge) noexcept
{
    size_t seed = hash<int>{}(bridge);
    int index = 0;
    for (const auto& bridge : hikers) {
        // Include the bridge index in the hash
        seed ^= hash<int>{}(++index) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        for (const auto h : bridge) {
            seed ^= hash<double>{}(h) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    }
    return seed;
}

vector<double> bridges;
vector<set<double>> hikers;
unordered_map<size_t, double> record;
size_t total_hikers;
double FromBridge(size_t bridge, double current_time);


// See what happens when 2 hikers advance to the start of the given 
double TestForward(size_t bridge, double current_time, double hiker1, double hiker2) {
    double result = std::numeric_limits<double>::max();
    auto& h = hikers.at(bridge);
    // Assume we've tested whether there's another bridge

    h.insert(hiker1);
    h.insert(hiker2);
    // If this configuration has a faster solution already, skip it
    auto r = record.insert({ HikersHash(hikers, bridge), current_time });
    if (r.second || r.first->second > current_time) {
        // Configuration not visited more quickly - DFS
        if (r.first->second > current_time) {
            r.first->second = current_time;
        }
        result = FromBridge(bridge, current_time);
    }
    h.erase(h.find(hiker1));
    h.erase(h.find(hiker2));
    return result;
}

// See what happens when this 1 hiker carries the flashlight back 
double TestBack(size_t bridge, double current_time, double hiker) {
    double result = std::numeric_limits<double>::max();
    // Assume we've tested whether we're at the start
    auto& h = hikers.at(bridge);
    h.insert(hiker);
    // If this configuration has a faster solution already, skip it
    auto r = record.insert({ HikersHash(hikers, bridge), current_time });
    if (r.second || r.first->second > current_time) {
        // Configuration not visited more quickly - DFS
        if (r.first->second > current_time) {
            r.first->second = current_time;
        }
        result = FromBridge(bridge, current_time);
    }
    h.erase(h.find(hiker));
    return result;
}

// Try moving every pair forward from the indexed bridge,
// then try moving every hiker back, in a DFS search,
// returning the minimum result.
double FromBridge(size_t bridge, double current_time) {
    auto& h = hikers.at(bridge);
    double result = std::numeric_limits<double>::max();
    // Test if we're done
    if (bridge == bridges.size() && h.size() == total_hikers) {
        return current_time;
    }
    
    for (auto i1 = h.begin(); i1 != h.end(); i1++) {
        auto speed1 = *i1;
        // Remove the first hiker of the pair from this bridge
        h.erase(i1);
        // Now remove each candidate for the second
        if (bridge < bridges.size()) {
            for (auto i2 = h.begin(); i2 != h.end(); i2++) {
                auto speed2 = *i2;
                h.erase(i2);
                double cost = bridges[bridge] / min(speed1, speed2) + current_time;

                // Check the config
                result = min(result, TestForward(bridge + 1, cost, speed1, speed2));

                // put the second hiker back in
                i2 = h.insert(speed2).first;
            }
        }
        // Now dfs with each hiker returning with the flashlight
        if (bridge > 0) {
            double cost = bridges[bridge - 1] / speed1 + current_time;
            result = min(result, TestBack(bridge - 1, cost, speed1));
        }
        // put the first hiker back in
        i1 = h.insert(speed1).first;
    }
    return result;
}

void ReadInput(char* filename) {
    total_hikers = 0;
    fstream newfile;
    newfile.open(filename, ios::in); //open a file to perform read operation using file object
    if (!newfile.is_open()) {   //checking whether the file is open
        cout << "Can't open " << filename << endl;
        return;
    }
    string tp;
    if (!getline(newfile, tp) || tp != "bridges:") {
        cout << "expected \'bridges:\' " << filename << endl;
        return;
    }
    // scan all the bridge lengths
    while (getline(newfile, tp)) { //read data from file object and put it into string.
        if (!(tp[0] == '-' && tp[1] == ' ')) {
            break;
        }
        bridges.push_back(stod(tp.substr(2)));
    }
    // now start scanning hikers for each bridge
    // assume at least one bridge
    hikers.push_back({});
    while (getline(newfile, tp)) { //read data from file object and put it into string.
        if (!(tp[0] == '-' && tp[1] == ' ')) {
            hikers.push_back({});
            continue;
        }
        hikers.back().insert(stod(tp.substr(2)));
        total_hikers++;
    }
    // placeholder for the end of the last bridge
    hikers.push_back({});
    newfile.close(); //close the file object.
}

int main(int argc, char* argv[])
{
    ReadInput(argv[1]);
    std::cout << "Fastest traversal: " << FromBridge(0, 0) << endl;
}

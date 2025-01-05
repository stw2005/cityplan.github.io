#include "../include/graph.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
using namespace std;

// Reads the area names from the "areas.txt" file.
vector<string> read_areas();

// Reads the data from "distances.txt" file and fills the graph with it.
void fill_graph(Graph<double>&);

// Returns the name of the area at the given index
string find_area(const vector<string>&, const int);

// Writes the result to a output file named "output.txt".
void output_result_to_file(const pair<vector<tuple<unsigned, unsigned, double>>, double>&, const vector<string>&);

int main() {
    vector<string> areas = read_areas();
    Graph<double> dist_graph(areas.size());
    fill_graph(dist_graph);
    pair<vector<tuple<unsigned, unsigned, double>>, double> routes = dist_graph.make_spanning_tree();
    output_result_to_file(routes, areas);
    return 0;
}

vector<string> read_areas() {
    vector<string> areas;
    ifstream input_file("data/areas.txt");
    if (!input_file)
        throw FileNotOpeningException();
    string line;
    while (getline(input_file, line))
        areas.push_back(line);
    input_file.close();
    return areas;
}

void fill_graph(Graph<double>& dist_graph) {
    ifstream input_file("data/distances.txt");
    if (!input_file)
        throw FileNotOpeningException();
    string line = "";
    int i = 0;
    while (getline(input_file, line)) {
        stringstream ss(line);
        int j = 0;
        string distance = "";
        while (ss >> distance) {
            dist_graph.add_edge(i, j, stod(distance));
            j++;
        }
        i++;
    }
    input_file.close();
}

string find_area(const vector<string>& areas, const int value) {
    return areas[value];
}

void output_result_to_file(const pair<vector<tuple<unsigned, unsigned, double>>, double>& routes, const vector<string>& areas) {
    ofstream output_file("output.txt");
    if (output_file.is_open()) {
        size_t n = routes.first.size();
        output_file << "The roads to be constructed are: \n";
        for (int i = 0; i < n; i++) {
            output_file << find_area(areas, get<0>(routes.first[i])) << " to "
                        << find_area(areas, get<1>(routes.first[i])) << '\t'
                        << "Distance: " << get<2>(routes.first[i]) << '\n';
        }
        output_file << "\nThe total length of the road is " << routes.second << endl;
        cout << "The output file has been generated with name \"output.txt\"." << endl;
        output_file.close();
    } else 
        throw FileNotOpeningException();
}
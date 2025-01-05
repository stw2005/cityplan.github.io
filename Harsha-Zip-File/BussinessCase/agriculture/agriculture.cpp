#include "../include/heap.hpp"
#include "../include/graph.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

// Read the data from the crops.txt file
vector<string> read_crop_names();

// Read the data from the crops-data.txt file
void read_crop_data(Graph<double>&);

// Finds the most vulnerable crop
string most_vulnerable_crop(vector<long double>&, vector<string>&);

// Finds the least vulnerable crop
string least_vulnerable_crop(vector<long double>&, vector<string>&);

// Print the analyzed data to the output.txt file
void write_data(string, string, vector<string>&, Graph<double>&);

int main() {
    vector<string> crops = read_crop_names();
    Graph<double> crop_graph(crops.size());
    read_crop_data(crop_graph);
    vector<long double> output = crop_graph.bfs();
    write_data(most_vulnerable_crop(output, crops), least_vulnerable_crop(output, crops), crops, crop_graph);
    return 0;
}

vector<string> read_crop_names() {
    ifstream input_file("data/crops.txt");
    if (!input_file)
        throw FileNotOpeningException();
    string line;
    vector<string> crops;
    while (getline(input_file, line))
        crops.push_back(line);
    input_file.close();
    return crops;
}

void read_crop_data(Graph<double>& crops_graph) {
    ifstream input_file("data/crops-data.txt");
    if (!input_file)
        throw FileNotOpeningException();
    string line;
    int i = 0;
    while (getline(input_file, line)) {
        istringstream ss(line);
        string attribute;
        double edge_weight = 0;
        int j = 0;
        while (getline(ss, attribute, ',')) {
            edge_weight = (i == j)? 0.0 : stod(attribute);
            crops_graph.add_edge(i, j, edge_weight);
            j++;
        }
        i++;
    }
}

string most_vulnerable_crop(vector<long double>& crop_data, vector<string>& crop_names) {
    return crop_names[distance(crop_data.begin(), max_element(crop_data.begin(), crop_data.end()))];
}

string least_vulnerable_crop(vector<long double>& crop_data, vector<string>& crop_names) {
    return crop_names[distance(crop_data.begin(), min_element(crop_data.begin(), crop_data.end()))];
}

void write_data(string crop1, string crop2, vector<string>& crops, Graph<double>& crop_graph) {
    ofstream output_file("output.txt");
    if (!output_file)
        throw FileNotOpeningException();
    function<bool(pair<INDEX, long double>, pair<INDEX, long double>)> comparator = [](pair<INDEX, long double> a, pair<INDEX, long double> b) { return a.second > b.second; };
    output_file << "\t\t\tData Analysis of crops\n";
    output_file << "The most vulnerable crop is " << crop1 << endl;
    output_file << "The least vulnerable crop is " << crop2 << endl;
    int n = crops.size();
    cout << "Enter the name of the crop to check its impact on other crops: "; cin >> crop1;
    int i = 0;
    for (string x: crops) {
        if (x == crop1)
            break;
        i++;
    }
    if (i > crops.size())
        cout << "Invalid input." << endl;
    else {
        output_file << "\nThe impact of " << crops[i] << " on:\n";
        vector<long double> impact = crop_graph.Dijkstra_Algorithm(i, comparator);
        for (int j = 0; j < 40; j++)
            output_file << crops[j] << ": " << impact[j] << endl;
    }
    output_file.close();
}
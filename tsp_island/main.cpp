


#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include "functions.h"
#include "algorithms.h"

using namespace std;

int main(int argc, char* argv[]) {
    bool greedy = false, island = false;
    string input_file;

    // Parsowanie argumentów
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--greedy") greedy = true;
        else if (arg == "--island") island = true;
        else input_file = arg; 
    }

    vector<vector<double>> graph;

    ifstream file(input_file);
    if (!file.is_open()) {
        cerr << "Nie można otworzyć pliku: " << input_file << endl;
        return 1;
    }

    int n;
    file >> n; 
    vector<vector<int>> cities(n, vector<int>(3));
    for (int i = 0; i < n; i++) {
        file >> cities[i][0] >> cities[i][1] >> cities[i][2];
    }
    file.close();

    graph = create_distance_graph(cities);

    if (greedy) {
        tsp_nearest_neighbour(graph);
    } else if (island) {


        if (n < 50) {
            tsp_island_model(graph, 7, 200, 3500, 250, 0.1, 0.95, 450, 4, 180);
        } else if (n < 200) {
            tsp_island_model(graph, 10, 250, 6000, 700, 0.15, 0.95, 650, 5, 180);
        } else if (n < 500) {
            tsp_island_model(graph, 11, 275, 7500, 1000, 0.175, 0.95, 800, 7, 180);
        } else {
            tsp_island_model(graph, 13, 325, 10000, 1750, 0.19, 0.95, 950, 9, 180);
        }
    } else {
        cerr << "Nie rozpoznano opcji. Użyj --greedy lub --island." << endl;
        return 1;
    }

    return 0;
}


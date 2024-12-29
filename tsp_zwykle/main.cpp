
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
    bool greedy = false, genetic = false;
    string input_file;

    // Parsowanie argumentów
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--greedy") greedy = true;
        else if (arg == "--genetic") genetic = true;
        else input_file = arg; // Zakładamy, że nazwa pliku jest ostatnim argumentem
    }

    vector<vector<double>> graph;

    ifstream file(input_file);
    if (!file.is_open()) {
        cerr << "Nie można otworzyć pliku: " << input_file << endl;
        return 1;
    }

    int n;
    file >> n; // Liczba miast jest odczytywana z pliku
    vector<vector<int>> cities(n, vector<int>(3));
    for (int i = 0; i < n; i++) {
        file >> cities[i][0] >> cities[i][1] >> cities[i][2];
    }
    file.close();

    graph = create_distance_graph(cities);

    if (greedy) {
        tsp_nearest_neighbour(graph);
    } else if (genetic) {
        if (n < 50) {
            tsp_genetic(graph, 250, 0.1, 0.95, 3500, 450, 4, 180);
        } else if (n < 200) {
            tsp_genetic(graph, 700, 0.15, 0.95, 5500, 650, 5, 180);
        } else if (n < 500) {
            tsp_genetic(graph, 1000, 0.175, 0.95, 7500, 800, 7, 180);
        } else {
            tsp_genetic(graph, 1750, 0.19, 0.95, 10000, 950, 9, 180);
        }
    } else {
        cerr << "Nie rozpoznano opcji. Użyj --greedy lub --genetic." << endl;
        return 1;
    }

    return 0;
}










































/*
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include<algorithm>
#include <cctype>
#include "functions.h"
#include "algorithms.h"

using namespace std;

int main(int argc, char* argv[]) {
    bool greedy = false, genetic = false;

    // Parsowanie argumentów
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--greedy") greedy = true;
        else if (arg == "--genetic") genetic = true;
    }

    vector<vector<double>> graph;

    while (true) {
        if (greedy) {
            int n;
            cout << "Podaj liczbę miast: ";
            cin >> n;
            vector<vector<int>> cities(n, vector<int>(3));
            for (int i = 0; i < n; i++) {
                cin >> cities[i][0] >> cities[i][1] >> cities[i][2];
            }
            graph = create_distance_graph(cities);
            tsp_nearest_neighbour(graph);
            return 0;
        } else if (genetic) {
            int n;
            cout << "Podaj liczbę miast: ";
            cin >> n;
            vector<vector<int>> cities(n, vector<int>(3));
            for (int i = 0; i < n; i++) {
                cin >> cities[i][0] >> cities[i][1] >> cities[i][2];
            }
            graph = create_distance_graph(cities);

            if (n < 50) {
                tsp_genetic(graph, 250, 0.1,0.95, 750, 250, 4,180);
            } else if (n < 200) {
                tsp_genetic(graph, 700, 0.15,0.95, 7000, 400, 5,180);
            } else if (n < 500) {
                tsp_genetic(graph, 1000, 0.175,0.95, 10000, 500, 7,180);
            } else {
                tsp_genetic(graph, 1750, 0.19,0.95, 15000, 750, 9,180);
            }

            return 0;
        }

        string choice;
        cout << "action> ";
        cin >> choice;
        transform(choice.begin(),choice.end(),choice.begin(),::tolower);
        if (choice == "help") {
            cout << "Help        Show this message\n";
            cout << "CreateEmpty Create the graph (empty)\n";
            cout << "CreateFull  Create the graph (full)\n";
            cout << "Print       Print the graph\n";
            cout << "TSP         Use nearest neighbour algorithm to find the shortest path and its length\n";
            cout << "TSP_GENETIC Use genetic algorithm to find the shortest path and its length\n";
            cout << "Exit        Exits the program (same as ctrl+D)\n";
        } else if (choice == "createfull") {
            graph = full_graph();
        } else if (choice == "createempty") {
            graph = empty_graph();
        } else if (choice == "print") {
            if (graph.empty()) {
                cout << "Graph is empty\n";
            } else {
                print_matrix(graph);
            }
        } else if (choice == "tsp") {
            tsp_nearest_neighbour(graph);
        } else if (choice == "tsp_genetic") {
            tsp_genetic(graph, 1750, 0.19,0.95, 15000, 750, 9,180);
        } else if (choice == "exit") {
            break;
        } else {
            cout << "To view possible actions type: Help\n";
        }
    }

    return 0;
}


*/
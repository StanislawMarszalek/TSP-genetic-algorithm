#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
using namespace std;

void print_matrix(const vector<vector<double>>& graph) {
    try {
        cout << "  |";
        for (size_t i = 0; i < graph.size(); i++) {
            cout << " " << i + 1;
        }
        cout << "\n--+";
        for (size_t i = 0; i < graph.size(); i++) {
            cout << "--";
        }
        cout << endl;

        for (size_t i = 0; i < graph.size(); i++) {
            cout << i + 1 << " |";
            for (size_t j = 0; j < graph[i].size(); j++) {
                cout << " " << graph[i][j];
            }
            cout << endl;
        }
    } catch (...) {
        cout << "An error occurred while printing the matrix.\n";
    }
}

vector<vector<double>> empty_graph() {
    try {
        int number_of_nodes;
        cout << "Enter number of nodes: ";
        cin >> number_of_nodes;

        vector<vector<double>> matrix(number_of_nodes, vector<double>(number_of_nodes, 0));
        return matrix;
    } catch (...) {
        cout << "An error occurred while creating an empty graph.\n";
        return {};
    }
}

vector<vector<double>> full_graph() {
    try {
        int number_of_nodes, max_length;
        cout << "Enter number of nodes: ";
        cin >> number_of_nodes;
        cout << "Enter max length: ";
        cin >> max_length;

        vector<vector<double>> matrix(number_of_nodes, vector<double>(number_of_nodes, 0));
        for (int i = 0; i < number_of_nodes; i++) {
            for (int j = i + 1; j < number_of_nodes; j++) {
                int distance = rand() % max_length + 1;
                matrix[i][j] = distance;
                matrix[j][i] = distance;
            }
        }
        return matrix;
    } catch (...) {
        cout << "An error occurred while creating a full graph.\n";
        return {};
    }
}

vector<vector<double>> create_distance_graph(const vector<vector<int>>& cities) {
    int n = cities.size();
    vector<vector<double>> graph(n, vector<double>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            double distance = sqrt(pow(cities[i][1] - cities[j][1], 2) + pow(cities[i][2] - cities[j][2], 2));
            graph[i][j] = distance;
            graph[j][i] = distance;
        }
    }
    return graph;
}

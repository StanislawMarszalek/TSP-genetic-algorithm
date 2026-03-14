#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include <cfloat>
#include <chrono>
#include <numeric>
#include <thread>

using namespace std;
using namespace chrono;

vector<int> tsp_nearest_neighbour(const vector<vector<double>>& graph) {
    int n = graph.size();
    vector<bool> visited(n, false);
    int start = 0;
    int current_city = start;
    double total_distance = 0;
    vector<int> path = {start + 1};
    visited[start] = true;

    for (int i = 0; i < n - 1; i++) {
        int nearest_city = -1;
        double min_distance = DBL_MAX;

        for (int city = 0; city < n; city++) {
            if (!visited[city] && graph[current_city][city] < min_distance) {
                nearest_city = city;
                min_distance = graph[current_city][city];
            }
        }

        if (nearest_city != -1) {
            visited[nearest_city] = true;
            path.push_back(nearest_city + 1);
            total_distance += min_distance;
            current_city = nearest_city;
        }
    }

    total_distance += graph[current_city][start];
    path.push_back(start + 1);

    cout << "Nearest Neighbour - Path: ";
    for (int city : path) cout << city << " ";
    cout << "\nNearest Neighbour - Distance: " << total_distance << endl;

    return path;
}


void tsp_genetic(const vector<vector<double>>& graph, int population_size = 100, double mutation_rate = 0.05, double crossover_rate = 0.95, int generations = 1000, int max_generations_without_improvement = 250, int tournamentNumb = 10, int max_time_seconds = 180) {
    int n = graph.size();
    if (n < 2) {
        cout << "Graph is too small for TSP." << endl;
        return;
    }

    random_device rd;
    mt19937 rng(rd());
    uniform_real_distribution<> random_rate(0.0, 1.0);

    auto calculate_fitness = [&](const vector<int>& route) {
        double distance = 0;
        for (size_t i = 0; i < route.size() - 1; i++) {
            distance += graph[route[i] - 1][route[i + 1] - 1];
        }
        distance += graph[route.back() - 1][route.front() - 1];
        return distance;
    };

    auto initiate_population = [&](int graph_size, int population_size, const vector<int>& current_best_route) {
        vector<vector<int>> population;
        population.push_back(current_best_route);
        for (int i = 1; i < population_size; i++) {
            vector<int> route(graph_size);
            iota(route.begin(), route.end(), 1);
            shuffle(route.begin() + 1, route.end() - 1, rng);
            population.push_back(route);
        }
        return population;
    };

    auto shuffle_mutation = [&](vector<int>& route) {

        if (random_rate(rng) <= mutation_rate) {
            int i, j;
            do {
                i = 1 + rng() % (n - 2);
                j = 1 + rng() % (n - 2);
            } while (i == j);
            if (i > j) swap(i, j);
            reverse(route.begin() + i, route.begin() + j + 1);
        }
    };

    auto tournament_selection = [&](const vector<vector<int>>& population) {
        vector<int> best_route;
        double best_fitness = DBL_MAX;
        for (int i = 0; i < tournamentNumb; i++) {
            int index = rng() % population.size();
            double fitness = calculate_fitness(population[index]);
            if (fitness < best_fitness) {
                best_fitness = fitness;
                best_route = population[index];
            }
        }
        return best_route;
    };

    auto order_crossover = [&](const vector<int>& parent1, const vector<int>& parent2) {
        vector<int> child1(n, -1);
        child1[0] = 1;

        if (random_rate(rng) <= crossover_rate) {
            int start = 1 + rng() % (n - 2);
            int end = 1 + rng() % (n - 2);
            if (start > end) swap(start, end);

            copy(parent1.begin() + start, parent1.begin() + end + 1, child1.begin() + start);

            vector<int> remaining;
            for (int gene : parent2) {
                if (find(child1.begin(), child1.end(), gene) == child1.end()) {
                    remaining.push_back(gene);
                }
            }

            auto it = child1.begin();
            for (int gene : remaining) {
                it = find(it, child1.end(), -1);
                if (it != child1.end()) {
                    *it = gene;
                }
            }
        } else {
            child1 = parent1;
        }

        return child1;
    };

    vector<int> current_best_route = tsp_nearest_neighbour(graph);
    vector<vector<int>> population = initiate_population(n, population_size, current_best_route);
    vector<int> best_route = current_best_route;
    double best_fitness = calculate_fitness(best_route);
    vector<int> tmp_best_route = best_route;
    double tmp_best_fitness = best_fitness;

    int generations_without_improvement = 0;
    double old_mutation_rate = mutation_rate;
    double old_crossover_rate = crossover_rate;
    auto start_time = high_resolution_clock::now();

    for (int generation = 0; generation < generations; generation++) {
        auto current_time = high_resolution_clock::now();
        if (duration_cast<seconds>(current_time - start_time).count() > max_time_seconds) {
            cout << "Stop: Time limit of " << max_time_seconds << " seconds exceeded." << endl;
            break;
        }

        vector<vector<int>> new_population;
        sort(population.begin(), population.end(), [&](const vector<int>& a, const vector<int>& b) {
            return calculate_fitness(a) < calculate_fitness(b);
        });

        new_population.insert(new_population.end(), population.begin(), population.begin() + population_size / 4);

        while (new_population.size() < population_size) {
            vector<int> parent1 = tournament_selection(population);
            vector<int> parent2 = tournament_selection(population);

            vector<int> child1 = order_crossover(parent1, parent2);
            shuffle_mutation(child1);
            new_population.push_back(child1);

            if (new_population.size() < population_size) {
                vector<int> child2 = order_crossover(parent2, parent1);
                shuffle_mutation(child2);
                new_population.push_back(child2);
            }
        }

        population = new_population;

        vector<int> current_best = *min_element(population.begin(), population.end(), [&](const vector<int>& a, const vector<int>& b) {
            return calculate_fitness(a) < calculate_fitness(b);
        });

        double current_fitness = calculate_fitness(current_best);

        if (current_fitness < best_fitness) {
            best_route = current_best;
            best_fitness = current_fitness;
            generations_without_improvement = 0;
            mutation_rate = old_mutation_rate;
            crossover_rate *= 0.9;
        } else {
            generations_without_improvement++;
            mutation_rate *= 1.15;
            crossover_rate = old_crossover_rate;
        }

        if (generations_without_improvement >= max_generations_without_improvement) {
            if (tmp_best_fitness > best_fitness) {
                tmp_best_route = best_route;
                tmp_best_fitness = best_fitness;
            }
            population = initiate_population(n, population_size,best_route);

            generations_without_improvement = 0;
            mutation_rate = old_mutation_rate;
            crossover_rate = 1.0;
        }

    }

    cout << "Genetic Algorithm - Best Path: ";
    for (int city : tmp_best_route) {
        cout << city << " ";
    }
    cout << "\nGenetic Algorithm - Best Distance: " << tmp_best_fitness << endl;
}

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
#include <mutex>

using namespace std;
using namespace chrono;

mutex population_mutex;

// Function for nearest neighbor heuristic
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

// Function to initialize diverse populations for islands
vector<vector<int>> initiate_population(int graph_size, int population_size, const vector<int>& current_best_route, int island_id) {
    vector<vector<int>> population;
    population.push_back(current_best_route);
    random_device rd;
    mt19937 rng(rd() + island_id); // Unikalne ziarno dla każdej wyspy

    for (int i = 1; i < population_size; i++) {
        vector<int> route(graph_size);
        iota(route.begin(), route.end(), 1);
        shuffle(route.begin() + 1, route.end() - 1, rng); // Tasowanie tylko wierzchołków oprócz 1
        route.push_back(1); // Dodanie powrotu do wierzchołka startowego
        population.push_back(route);
    }
    return population;
}



// Function for the Genetic Algorithm on a single island
void tsp_genetic_island(const vector<vector<double>>& graph, vector<vector<int>>& island_population, int population_size, double mutation_rate, double crossover_rate, int generations, int max_generations_without_improvement, int tournamentNumb, int max_time_seconds, vector<int>& best_route, double& best_fitness) {
    int n = graph.size();
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

    auto start_time = high_resolution_clock::now();
    int generations_without_improvement = 0;
    double old_mutation_rate = mutation_rate;
    double old_crossover_rate = crossover_rate;

    for (int generation = 0; generation < generations; generation++) {
        auto current_time = high_resolution_clock::now();
        if (duration_cast<seconds>(current_time - start_time).count() > max_time_seconds) {
            cout << "Island stopped: Time limit exceeded." << endl;
            break;
        }

        vector<vector<int>> new_population;
        sort(island_population.begin(), island_population.end(), [&](const vector<int>& a, const vector<int>& b) {
            return calculate_fitness(a) < calculate_fitness(b);
        });

        // Elitaryzm: najlepsze 25% przechodzi do następnej populacji
        new_population.insert(new_population.end(), island_population.begin(), island_population.begin() + population_size / 4);

        while (new_population.size() < population_size) {
            vector<int> parent1 = tournament_selection(island_population);
            vector<int> parent2 = tournament_selection(island_population);

            vector<int> child1 = order_crossover(parent1, parent2);
            shuffle_mutation(child1);
            new_population.push_back(child1);

            if (new_population.size() < population_size) {
                vector<int> child2 = order_crossover(parent2, parent1);
                shuffle_mutation(child2);
                new_population.push_back(child2);
            }
        }

        island_population = new_population;

        vector<int> current_best = *min_element(island_population.begin(), island_population.end(), [&](const vector<int>& a, const vector<int>& b) {
            return calculate_fitness(a) < calculate_fitness(b);
        });

        double current_fitness = calculate_fitness(current_best);

        if (current_fitness < best_fitness) {
            lock_guard<mutex> lock(population_mutex);
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
            generations_without_improvement = 0;
            mutation_rate = old_mutation_rate;
            crossover_rate = 1.0;
        }
    }

    cout << "Island Best Distance: " << best_fitness << endl;
}

// Function to manage multiple islands
void tsp_island_model(const vector<vector<double>>& graph, int num_islands, int migration_interval, int generations, int population_size, double mutation_rate, double crossover_rate, int max_generations_without_improvement, int tournamentNumb, int max_time_seconds) {
    vector<vector<vector<int>>> islands(num_islands);
    vector<int> global_best_route;
    double global_best_fitness = DBL_MAX;
    vector<thread> threads;

    // Initialize islands with separate populations
    // Initialize islands with separate populations
    for (int i = 0; i < num_islands; i++) {
    vector<int> initial_route = tsp_nearest_neighbour(graph);
    islands[i] = initiate_population(graph.size(), population_size, initial_route, i);
    }


    // Run genetic algorithm on each island in separate threads
    for (int i = 0; i < num_islands; i++) {
        threads.emplace_back([&, i]() {
            tsp_genetic_island(graph, islands[i], population_size, mutation_rate, crossover_rate, generations, max_generations_without_improvement, tournamentNumb, max_time_seconds, global_best_route, global_best_fitness);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Perform migration between islands at intervals
    for (int migration_step = 0; migration_step < generations; migration_step += migration_interval) {
        for (int i = 0; i < num_islands; i++) {
            int target_island = (i + 1) % num_islands;
            int num_migrants = population_size / 10; // 10% migration

            lock_guard<mutex> lock(population_mutex);
            for (int j = 0; j < num_migrants; j++) {
                islands[target_island][j] = islands[i][j];
            }
        }
    }

    cout << "Final Global Best Distance: " << global_best_fitness << endl;
    cout << "Final Global Best Path: ";
    for (int city : global_best_route) {
        cout << city << " ";
    }
    cout << endl;
}

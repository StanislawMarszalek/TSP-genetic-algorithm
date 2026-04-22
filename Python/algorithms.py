from random import sample,randint,random
import time

#the greedy version

def tsp_nearest_neighbour(graph:list[list[float]])->list[int]:
    """
    Finds the best route using greedy algorithm (going to the nearest neighbour)
    
    :param graph: Adjacency matrix of the cities containing the distances between them
    :type graph: list[list[float]]
    :return: The best found route
    :rtype: list[int]
    """
    number_of_cities:int = len(graph)
    visited:list[bool] = [False] * number_of_cities
    start:int = 0
    current_city:int = start
    total_distance:float = 0.0
    path:list[int] = [start+1]
    visited[start] = True  

    for _ in range(number_of_cities - 1):
        nearest_city:int = -1
        min_distance:float = float('inf')

        for city in range(number_of_cities):
            if not visited[city] and graph[current_city][city] < min_distance:
                nearest_city = city
                min_distance = graph[current_city][city]

        if nearest_city is not None:
            visited[nearest_city] = True
            path.append(nearest_city+1)
            total_distance += min_distance
            current_city = nearest_city

    total_distance += graph[current_city][start]
    path.append(start+1)

    print("Path:", *path, "\nDistance:", total_distance)
    return path

#Functions to the genetic version
def crossover(list1:list[int], list2:list[int])->tuple[list[int],list[int]]:
    """
    Do crossover operation on given sequences of cities,and return two new sequences (children)
    
    :param list1: First sequnce of cities 
    :type list1: list[int]
    :param list2: Second sequnce of cities 
    :type list2: list[int]
    :return: Children,made by doing crossover operation on the first and second sequences
    :rtype: tuple[list[int], list[int]]
    """
    index:int = randint(1, len(list1) - 2)
    child1:list[int] = [1] + list1[1:index]
    child1 += [city for city in list2[1:] if city not in child1]
    child1.append(1)

    child2:list[int] = [1] + list2[1:index]
    child2 += [city for city in list1[1:] if city not in child2]
    child2.append(1)
    return child1, child2

def mutate(route:list[int],mutation_rate:float)->list[int]:
    """
    Mutate given sequence with given probability
    
    :param route: Sequence of cities to mutate
    :type route: list[int]
    :param mutation_rate: Mutation's probability
    :type mutation_rate: float
    :return: The same sequence if mutation didn't happen, 
    otherwise a sequence with two cities swaped
    :rtype: list[int]
    """
    if random() < mutation_rate:
        idx1, idx2 = sample(range(1, len(route) - 1), 2)
        route[idx1], route[idx2] = route[idx2], route[idx1]
    return route

def initiate_population(graph_size:int,population_size:int,current_best_route)->list[list[int]]:
    return [current_best_route] + [[1] + sample(range(2, graph_size + 1), graph_size - 1) + [1] for _ in range(population_size - 1)]

def calculate_fitness(route:list[int], graph:list[list[float]])->float:
    """
    Calculate fitness for the given route by evalueting it's total distance
    
    :param route: Sequence of cities
    :type route: list[int]
    :param graph: Adjacency matrix of the cities containing the distances between them
    :type graph: list[list[float]]
    :return: Total distance of the given route
    :rtype: float
    """
    distance:float = 0.0
    for i in range(len(route) - 1):
        distance += graph[route[i] - 1][route[i + 1] - 1]#Nodes are labeled from 1 to N
    distance += graph[route[-1] - 1][route[0] - 1]
    return distance

def tournament_selection(population:list[list[int]], graph:list[list[float]], k:int=10)->list[int]:
    selected = sample(population, k)
    selected.sort(key=lambda route: calculate_fitness(route, graph))
    return selected[0]

#the genetic version

def tsp_genetic(graph:list[list[float]], population_size:int = 100, mutation_rate:float = 0.05, generations:int = 1000, max_generations_without_improvement:int = 250, max_time_seconds:int = 60)->list[int]:

    """
    Finds the best route (with given constraints) using genetic algorithm
    
    :param graph: Adjacency matrix of the cities containing the distances between them
    :type graph: list[list[float]]
    :param population_size: Number of routes in a generation
    :type population_size: int
    :param mutation_rate: Mutation's probability
    :type mutation_rate: float
    :param generations: Total numbers of generations
    :type generations: int
    :param max_generations_without_improvement: Max number of generations without imporvement
    before program stops executing
    :type max_generations_without_improvement: int
    :param max_time_seconds: Max number of seconds to find the best route 
    :type max_time_seconds: int
    :return: Best found route  
    :rtype: list[int]
    """

    graph_size:int = len(graph)
    print("Result of the greedy version:")
    current_best_route:list[int] = tsp_nearest_neighbour(graph)
    population:list[list[int]] = initiate_population(graph_size, population_size,current_best_route)
    best_route:list[int] = min(population, key=lambda x: calculate_fitness(x, graph))
    best_fitness:float = calculate_fitness(best_route, graph)
    generations_without_improvement:int = 0

    start_time:float = time.time()

    for _ in range(generations):
        if time.time() - start_time >= max_time_seconds:
            print(f"Stop: Time limit of {max_time_seconds} seconds exceeded.")
            break

        new_population:list[list[int]] = [best_route]

        for _ in range((population_size - 1) // 2):
            parent1:list[int] = tournament_selection(population, graph)
            parent2:list[int] = tournament_selection(population, graph)
            child1, child2 = crossover(parent1, parent2)
            new_population.append(mutate(child1,mutation_rate))
            new_population.append(mutate(child2,mutation_rate))

        population = new_population
        current_best_route = min(population, key=lambda x: calculate_fitness(x, graph))
        current_best_fitness:float = calculate_fitness(current_best_route, graph)

        if current_best_fitness < best_fitness:
            best_route = current_best_route
            best_fitness = current_best_fitness
            generations_without_improvement = 0
        else:
            generations_without_improvement += 1

        if generations_without_improvement >= max_generations_without_improvement:
            break

    print("Path:", *best_route, "\nDistance:", best_fitness)
    return best_route

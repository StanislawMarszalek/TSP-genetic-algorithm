import argparse
from copy import deepcopy
from functions import print_matrix, full_graph, empty_graph, create_distance_graph
from algorithms import tsp_nearest_neighbour, tsp_genetic

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    group = parser.add_mutually_exclusive_group()
    group.add_argument('--greedy', action='store_true', help='Greedy TSP algorithm')
    group.add_argument('--genetic', action='store_true', help='Genetic TSP algorithm')
    args = parser.parse_args()

    while True:
        if args.greedy:
            number_of_cities:int = int(input("Enter the number of cities: "))
            cities:list[list[int]] = []
            for _ in range(number_of_cities):
                city_data:list[int] = list(map(int, input().split()))
                cities.append(city_data)
            graph:list[list[int]]|list[list[float]] = create_distance_graph(cities)
            tsp_nearest_neighbour(graph)
            break

        if args.genetic:
            number_of_cities = int(input("Enter the number of cities: "))
            cities = []
            for _ in range(number_of_cities):
                city_data = list(map(int, input().split()))
                cities.append(city_data)
            graph = create_distance_graph(cities)
            tsp_genetic(graph)
            break

        choice:str = input("action> ").strip().lower()
        if choice == "help":
            print("""Help        Show this message
CreateEmpty Create the graph(empty)
CreateFull  Create the graph(full)                
Print       Print the graph
TSP         Use nearest neighbour algorithm to find the shortest path and its length
TSP_GENETIC Use genetic algorithm to find the shortest path and its length
Exit        Exits the program (same as ctrl+D)""")
        elif choice == "createfull":
            tmp_graph:list[list[int]] = full_graph()
            graph = create_distance_graph(tmp_graph)
        elif choice == "createempty":
            graph = empty_graph()
            tmp_graph=deepcopy(graph)
        elif choice == "print":
            print("Graph is empty") if not graph else print_matrix(tmp_graph) # pylint:disable=expression-not-assigned
        elif choice == "tsp":
            tsp_nearest_neighbour(graph)
        elif choice == "tsp_genetic":
            tsp_genetic(graph)
        elif choice == "exit":
            break
        else:
            print("To view possible actions type: Help")

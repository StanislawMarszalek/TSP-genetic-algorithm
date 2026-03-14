import random
from math import sqrt

def print_matrix(graph:list[list[int]])->None:
    graph_size:int=len(graph)
    print("  |", * [i + 1 for i in range(graph_size)])
    print("--+", "-" * 2 * len(graph), sep="")
    for i in range(graph_size):
        print(i + 1, "|", * graph[i])

    return

def empty_graph()->list[list[int]]:
    try:
        number_of_nodes=abs(int(input('Enter number of nodes: ')))
        matrix=[[0]*number_of_nodes for _ in range(number_of_nodes)]
        return matrix
    except ValueError as e:
        print(f"An error occurred - {e}")
        return [[-1]]

def full_graph()->list[list[int]]:
    try:
        number_of_nodes:int=abs(int(input('Enter number of nodes: ')))
        max_lenght:int=abs(int(input("Enter max length: ")))
        matrix:list[list[int]]=[[0]*number_of_nodes for _ in range(number_of_nodes)]
        for row_numb in range(number_of_nodes):
            for col_numb in range(row_numb,number_of_nodes):
                if row_numb==col_numb:
                    matrix[row_numb][col_numb]=0
                    continue
                matrix[row_numb][col_numb]=random.randint(1,max_lenght)
                matrix[col_numb][row_numb]=matrix[row_numb][col_numb]
        return matrix
    except ValueError as e:
        print(f"An error occurred - {e}")
        return [[-1]]

def create_distance_graph(cities:list[list[int]])->list[list[float]]:
    number_of_cities:int = len(cities)
    graph:list[list[float]]= [[0.0] * number_of_cities for _ in range(number_of_cities)]
    for i in range(number_of_cities):
        for j in range(i, number_of_cities):
            distance = sqrt(pow(cities[i][1] - cities[j][1], 2) + pow(cities[i][2] - cities[j][2], 2))
            graph[i][j] = distance
            graph[j][i] = distance
    return graph

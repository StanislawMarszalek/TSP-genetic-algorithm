import random
from math import sqrt

def print_matrix(graph:list[list[int]])->None:
    """
    Print an adjacency matrix as a table
    
    :param graph: Adjacency matrix
    :type graph: list[list[int]]
    """
    graph_size:int=len(graph)
    print("  |", * [i + 1 for i in range(graph_size)])
    print("--+", "-" * 2 * len(graph), sep="")
    for i in range(graph_size):
        print(i + 1, "|", * graph[i])

    return

def empty_graph()->list[list[int]]:
    """
    Ask for number of nodes and create an empty matrix of size number_of_nodes X number_of_nodes
    
    :return: Adjacency matrix of the nodes
    :rtype: list[list[int]]
    """
    try:
        number_of_nodes=abs(int(input('Enter number of nodes: ')))
        matrix=[[0]*number_of_nodes for _ in range(number_of_nodes)]
        return matrix
    except ValueError as e:
        print(f"An error occurred - {e}")
        return [[-1]]

def full_graph()->list[list[int]]:
    """
    Ask for number of nodes and max distance between them, 
    then create a full matrix of size number_of_nodes X number_of_nodes 
    
    :return: Adjacency matrix of the nodes with distances between them
    :rtype: list[list[int]]
    """
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
    """
    Creates an adjacency matrix of the cities with distances between them
    
    :param cities: Matrix of the cities with their coordinates
    :type cities: list[list[int]]
    :return: Adjacency matrix of the cities with distances between them
    :rtype: list[list[float]]
    """
    number_of_cities:int = len(cities)
    graph:list[list[float]]= [[0.0] * number_of_cities for _ in range(number_of_cities)]
    for i in range(number_of_cities):
        for j in range(i, number_of_cities):
            distance = sqrt(pow(cities[i][1] - cities[j][1], 2) + pow(cities[i][2] - cities[j][2], 2))
            graph[i][j] = distance
            graph[j][i] = distance
    return graph

# TSP solver based on genetic algorithm
This repository contains implementations of a TSP (Travelling Salesperson Problem) solver based on  Genetic Algorithm </br>
The algorithm is based on:
- Order Crossover (OX)
- Shuffle mutation
- Tournament selection
- Fitness/Cost function: total route distance (lower is better)
- Threads (as islands) and  migration between them (only in the island model)

## Brief description of the program
The program allows the user to generate a random graph with a specified number of nodes and a maximum distance between two nodes provided by the user, or the program can read data from a given file. It then creates a distance graph that contains the distances between nodes and uses the algorithm specified by the user to find a solution.

</br>There are three versions to choose from:
- greedy — nearest-neighbour heuristic
- genetic — genetic algorithm
- island — island-model genetic algorithm (**C++ only**)

In all cases the program prints the best route and its length.

## Usage
### For C++ (after compiling)
Type ```.\main.exe [OPTIONS] [PATHFILE]```</br>
Options:
- `--greedy`— run nearest-neighbour heuristic
- `--island` — run island-model genetic algorithm (C++ only)
- `--generate` — ignore `PATHFILE `and generate a random instance (program will request generation parameters)
### For Python
Type ```python3 main.py [OPTION] < PATHFILE```</br>
Options:
- `--greedy`— run nearest-neighbour heuristic
- `--genetic` — run genetic algorithm
  
If no option and no file are provided, the Python program will display an interactive menu. 

### Creating examples for testing
Run `python3 benchmark_generate.py <number_of_cities> <coord_min> <coord_max>` to generate a file in the correct format for testing the algorithms.

## File format
The input file must follow this format:
```
number_of_cities
id_1 x_coordinate_1 y_coordinate_1
id_2 x_coordinate_2 y_coordinate_2
...
id_n x_coordinate_n y_coordinate_n
```
Example
```
5
1 2993 961
2 1025 1002
3 2439 1203
4 1652 2267
5 4524 1625
```


import random
import sys

def generate_file(pathfile:str,number_of_cities:int,coord_min_value:int,coord_max_value:int)->None:
    """
    Generate file that contains cities with their coordinates
    
    :param filename: Path to the file
    :type filename: str
    :param number_of_cities: Number of cities to create (each city is stored in a new line)
    :type number_of_cities: int
    :param coord_min_value: Minimal value for coordinates
    :type coord_min_value: int
    :param coord_max_value: Maximal value for coordinates
    :type coord_max_value: int
    """
    cities:set = set()
    with open(pathfile, 'w',encoding="utf-8") as file:
        file.write(f"{number_of_cities}\n")
        for i in range(1, number_of_cities + 1):
            while True:
                coord_x = random.randint(coord_min_value, coord_max_value)
                coord_y = random.randint(coord_min_value, coord_max_value)
                if (coord_x, coord_y) not in cities:
                    cities.add((coord_x, coord_y))
                    file.write(f"{i} {coord_x} {coord_y}\n")
                    break

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python3 benchmark_generate.py <number_of_cities> <coord_min> <coord_max>")
        sys.exit(1)
    try:
        n = int(sys.argv[1])
        coord_min = int(sys.argv[2])
        coord_max = int(sys.argv[3])
    except ValueError as e:
        print(f"Error: {e}")
        print("Usage: python3 benchmark_generate.py <number_of_cities> <coord_min> <coord_max>")
        sys.exit(1)
    if coord_min >= coord_max:
        print("Error: coord_min must be less than coord_max")
        sys.exit(1)
    FILENAME:str = 'test.txt'
    generate_file(FILENAME, n, coord_min, coord_max)

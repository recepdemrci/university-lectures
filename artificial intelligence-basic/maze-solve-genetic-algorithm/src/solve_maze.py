import src.setup as setup
from src.position import Position
from src.population import Population

import random
"""
Solve the Maze: try to find a path from start to target in the maze which has some obstacles.
Use Genetic Algorithm
"""


# Create a maze, and obstacle(x, y, vertical)
def create_maze():
    mutation_rate = 0.1
    population_size = 150
    generation_size = 100
    random_crossover = False

    while True:                                                                # Take maze size and control input
        try:
            print("   (Enter '0' to change access settings)")
            n = int(input(" > Enter the maze size: "))
            if n < 0:
                print("[ERROR] : Size must be positive! duh..")
                continue
            if n == 0:
                print(" 1 > Change Generation Count   (default: ", generation_size, ")")
                print(" 2 > Change Population Size    (default: ", population_size, ")")
                print(" 3 > Change Mutation Rate      (default: ", mutation_rate, ")")
                print(" 4 > ON / OFF Random Crossover (default: ", random_crossover, ")")
                print(" 4 > ON / OFF Visualization    (default: ", setup.screen, ")")
                print(" 0 > Exit Settings")
                while True:
                    try:
                        settings = int(input("   > "))
                        if settings == 1:
                            generation_size = int(input("   > New Generation Count: "))
                            continue
                        elif settings == 2:
                            population_size = int(input("   > New Population Size: "))
                            continue
                        elif settings == 3:
                            mutation_rate = int(input("   > New Mutation Rate: "))
                            continue
                        elif settings == 4:
                            random_crossover = bool(int(input("   > ON(1)/OFF(0) Random Crossovers: ")))
                            continue
                        elif settings == 5:
                            setup.screen = bool(int(input("   > ON(1)/OFF(0) Visualization Screen: ")))
                            continue
                    except ValueError:
                        print("   That's not a valid input!")
                    else:
                        break
                continue
        except ValueError:
            print("That's not a valid input!")
        else:
            break

    while True:                                                                # Take obstacles number and control input
        try:
            obstacle_size = int(input(" > Enter obstacle number: "))
            if obstacle_size < 0:
                print("It have to be positive or 0 sir")
                continue
        except ValueError:
            print("That's not a valid input!")
        else:
            break

    setup.maze_size = n
    setup.start = (1, 1)                                                       # Assign start position
    setup.target = Position(n - 1, n - 1)                                      # Assign target position
    obstacles = []
    for i in range(0, obstacle_size):                                          # Generate random obstacle
        vertical = bool(random.getrandbits(1))
        if vertical:
            x = random.randint(1, n - 4)
            y = random.randint(1, n - 1)
            obstacle = Position.create_obstacle(Position(x, y), vertical=True)
        else:
            x = random.randint(1, n - 1)
            y = random.randint(1, n - 4)
            obstacle = Position.create_obstacle(Position(x, y), vertical=False)
        obstacles.append(obstacle)

    setup.canvas.configure(width=n * setup.grid + 1, height=n * setup.grid + 1)
    return obstacles, mutation_rate, population_size, generation_size, random_crossover


def solve_maze(obstacles, mutation_rate, population_size, generation_size, random_crossover=True):
    dna_size = setup.maze_size ** 2                                      # Define dna size
    population = Population(dna_size, mutation_rate, population_size)    # Create random population

    population.live(obstacles, dna_size)
    for i in range(0, generation_size):
        population.calculate_fitness()                                   # Calculate fitness values
        population.natural_selection()                                   # Select new parents based on their fitness
        population.next_generation(random_crossover)                     # Generate new populations
        population.live(obstacles, dna_size)                             # Population lives up to dna_size or until stop
    return population


# Get best agent in given populations
def get_best_agent(agents):
    best_fitness = 0
    best_agent = None
    for agent in agents:
        if best_fitness < agent.fitness:
            best_fitness = agent.fitness
            best_agent = agent
    return best_agent


def main():
    obstacles, mutation_rate, population_size, generation_size, random_crossover = create_maze()
    population = solve_maze(obstacles, mutation_rate, population_size, generation_size, random_crossover)

    setup.screen = True
    agent = get_best_agent(agents=setup.winner)
    agent.display(delete=False)


if __name__ == '__main__':
    main()

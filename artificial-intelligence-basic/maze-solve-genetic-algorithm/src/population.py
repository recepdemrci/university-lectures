import src.setup as setup
from src.dna import DNA
from src.agent import Agent
from src.position import Position

import random
from numpy import interp


# Population of agent(Person)
class Population:
    def __init__(self, dna_size, mutation_rate, population_size):
        self.mutation_rate = mutation_rate
        self.population = []
        self.selection_pool = []
        self.generations = 0
        self.rank = 0                                                          # Rank of reaching target

        for i in range(0, population_size):                                    # Create a new population of agents
            agent = Agent(DNA(dna_size), Position(setup.start[0], setup.start[1]))
            self.population.append(agent)
        print("[INFO] : Generation:  0")

    # FUNCTIONS---------------------------------------------------------------------------------------------------------

    # Calculate Fitness for all agent in population
    def calculate_fitness(self):
        for agent in self.population:
            agent.calculate_fitness()

    # Get max and min fitness value in populations
    def max_and_min_fitness(self):
        max_fitness = -(setup.maze_size * 2)
        min_fitness = 1
        for agent in self.population:
            if max_fitness < agent.fitness:
                max_fitness = agent.fitness
            if min_fitness > agent.fitness:
                min_fitness = agent.fitness
        return min_fitness, max_fitness

    # Generate a pool for selection based on fitness value
    def natural_selection(self):
        self.selection_pool.clear()                                            # Clear the pool for selection

        for agent in self.population:
            i, j = self.max_and_min_fitness()                                  # Get max & min fitness in population
            normalized_fitness = interp(agent.fitness, [i, j], [0, 10])        # Normalize fitness value --> [1 - 10]
            agent_number = int(normalized_fitness)
            for i in range(0, agent_number):                                   # Based on normalize fitness value,
                self.selection_pool.append(agent)                              # add agent into selection pool

    # Create new generation
    def next_generation(self, random_crossover=True):
        self.rank = 0
        self.generations += 1                                                   # Increase generation number
        for i in range(0, len(self.population)):
            parent_1 = random.choice(self.selection_pool)                       # Select two parent from selection_pool
            parent_2 = random.choice(self.selection_pool)
            child_dna = parent_1.DNA.crossover(parent_2.DNA, random_crossover)  # Crossover two parent's DNA

            child_dna.mutate(self.mutation_rate)                                # Mutate child DNA
            child = Agent(child_dna, Position(setup.start[0], setup.start[1]))  # And create child from that DNA
            self.population[i] = child                                          # Fill population with new child
        print("[INFO] : Generation: ", self.generations)

    # Live the whole population
    def live(self, obstacles, lifetime):
        i = 0
        for agent in self.population:
            life_circle = 0
            while not agent.stopped and life_circle < lifetime:
                agent.move(obstacles)                                          # Move agent until stop, or
                life_circle += 1                                               # reach lifetime
            if agent.target_reached():                                         # Control if agent is finished
                setup.winner.append(agent)
                i += 1
        print("[INFO] : Winners: ", i, "\n")

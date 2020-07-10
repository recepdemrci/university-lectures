#!/usr/bin/env python

from sys import exit
from time import sleep

import random
import math
import numpy as np
import matplotlib.pyplot as plt

from selenium import webdriver
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.keys import Keys

game_url = "chrome://dino"


class Game:
    def __init__(self):
        chrome_options = Options()
        chrome_options.add_argument("disable-infobars")
        chrome_options.add_argument("--mute-audio")
        self.browser = webdriver.Chrome(ChromeDriverManager().install(), chrome_options=chrome_options)
        self.browser.set_window_position(x=-10, y=0)
        self.browser.get('chrome://dino')
        self.browser.execute_script("Runner.config.ACCELERATION=0")
        self.browser.implicitly_wait(30)
        self.browser.maximize_window()

    def get_crashed(self):
        return self.browser.execute_script("return Runner.instance_.crashed")

    def get_playing(self):
        return self.browser.execute_script("return Runner.instance_.playing")

    def restart(self):
        self.browser.execute_script("Runner.instance_.restart()")

    def press_up(self):
        self.browser.find_element_by_tag_name("body").send_keys(Keys.ARROW_UP)

    def press_down(self):
        self.browser.find_element_by_tag_name("body").send_keys(Keys.ARROW_DOWN)

    def press_right(self):
        self.browser.find_element_by_tag_name("body").send_keys(Keys.ARROW_RIGHT)

    def get_score(self):
        score_array = self.browser.execute_script("return Runner.instance_.distanceMeter.digits")
        score = ''.join(score_array)
        return int(score)

    def get_highscore(self):
        score_array = self.browser.execute_script("return Runner.instance_.distanceMeter.highScore")
        for i in range(len(score_array)):
            if score_array[i] == '':
                break
        score_array = score_array[i:]
        score = ''.join(score_array)
        return int(score)

    def get_state(self):
        obstacles = self.browser.execute_script("return Runner.instance_.horizon.obstacles")
        obstacle_width = 0
        obstacle_height = 0
        distance = 0
        if len(obstacles):
            t_rex = self.browser.execute_script("return Runner.instance_.tRex")
            obstacle = obstacles[0]
            distance = obstacle["xPos"] - t_rex["xPos"] - t_rex["config"]["WIDTH"]
            obstacle_width = obstacle["width"]
            obstacle_height = obstacle["typeConfig"]["height"]
        return normalize((distance, obstacle_height, obstacle_width))

    def pause(self):
        return self.browser.execute_script("return Runner.instance_.stop()")

    def resume(self):
        return self.browser.execute_script("return Runner.instance_.play()")

    def end(self):
        self.browser.close()


class DNA:
    def __init__(self, item):
        self.weights = []
        if type(item) == int:
            for i in range(0, item):
                self.weights.append(random.random())
        else:
            self.weights = item

    def get_weights(self):
        return self.weights

    def crossover(self, partner):
        child_weights = []
        crossover_point = random.randint(1, len(self.weights))
        for i in range(0, len(self.weights)):
            if i < crossover_point:
                child_weights.append(self.weights[i])
            else:
                child_weights.append(partner.weights[i])
        child_dna = DNA(child_weights)
        return child_dna

    def mutate(self, _mutation_rate):
        if random.random() < _mutation_rate:
            for i in range(0, len(self.weights)):
                if random.random() < _mutation_rate:
                    self.weights[i] = random.random()


class DinoAgent:
    def __init__(self, game, dna, network_model):
        self.dinoGame = game
        self.DNA = dna
        self.network_model = network_model
        self.fitness = 0
        self.jump_counter = 0

    def is_running(self):
        return self.dinoGame.get_playing()

    def is_crashed(self):
        return self.dinoGame.get_crashed()

    def jump(self):
        self.dinoGame.press_up()

    def duck(self):
        self.dinoGame.press_down()

    def do_nothing(self):
        self.dinoGame.press_right()

    def start_life(self):
        self.dinoGame.restart()
        self.jump()
        sleep(3)

    def move(self):
        weights = self.DNA.get_weights()
        input_values = self.dinoGame.get_state()
        result = self.network_model.predict(weights, input_values)
        if result == 1:
            self.jump()
            self.jump_counter += 1
        elif result == -1:
            self.duck()
            self.jump_counter = 0
        else:
            self.do_nothing()
            self.jump_counter = 0

    def calculate_fitness(self):
        self.fitness = self.dinoGame.get_score()
        if self.jump_counter > 15:
            self.fitness = 43


class Population:
    def __init__(self, game, _nn_model, _mutation_rate, _dna_size, _population_size=12):
        self.game = game
        self.nn_model = _nn_model
        self.mutation_rate = _mutation_rate
        self.population = []
        self.selection_pool = []
        self.generations = 0

        for i in range(0, _population_size):
            dino_agent = DinoAgent(game, DNA(_dna_size), self.nn_model)
            self.population.append(dino_agent)

    def get_best(self):
        best_fitness = 0
        best_dino = None
        for dino in self.population:
            if best_fitness < dino.fitness:
                best_fitness = dino.fitness
                best_dino = dino
        return best_dino

    def max_and_min_fitness(self):
        max_fitness = self.game.get_highscore()
        min_fitness = 0
        for dino_agent in self.population:
            if max_fitness < dino_agent.fitness:
                max_fitness = dino_agent.fitness
            if min_fitness > dino_agent.fitness:
                min_fitness = dino_agent.fitness
        return min_fitness, max_fitness

    def natural_selection(self):
        self.selection_pool.clear()                                               # Clear the pool for selection
        for dino_agent in self.population:
            i, j = self.max_and_min_fitness()                                     # Get max & min fitness in population
            normalized_fitness = np.interp(dino_agent.fitness, [i, j], [0, 10])   # Normalize fitness value --> [1 - 10]
            agent_number = int(normalized_fitness)
            for i in range(0, agent_number):                                      # Based on normalize fitness value,
                self.selection_pool.append(dino_agent)                            # add agent into selection pool

    def next_generation(self):
        best = self.get_best()
        self.generations += 1                                                    # Increase generation number
        for i in range(0, len(self.population) - 1):
            parent_1 = random.choice(self.selection_pool)                        # Select two parent from selection_pool
            parent_2 = random.choice(self.selection_pool)
            child_dna = parent_1.DNA.crossover(parent_2.DNA)                     # Crossover two parent's DNA
            child_dna.mutate(self.mutation_rate)                                 # Mutate child DNA
            child = DinoAgent(self.game, child_dna, self.nn_model)               # And create child from that DNA
            self.population[i] = child                                           # Fill population with new child
        self.population.pop()
        self.population.append(best)
        print("-------------------------------------------")
        print("[INFO] : GENERATION: ", self.generations)
        print("-------------------------------------------")

    def live(self):
        for idx, dino_agent in enumerate(self.population):
            dino_agent.start_life()
            while not dino_agent.is_crashed():
                dino_agent.move()
            dino_agent.calculate_fitness()
            print("-------------------------------------------")
            print("[INFO] : tRex ", idx + 1, "/", len(self.population))
            print("[INFO] : Fitness: ", dino_agent.fitness)
            print("-------------------------------------------")


class Model:
    def __init__(self, input_size):
        self.last_layer_size = input_size
        self.weights_shapes = []
        self.weights_size = 0

    def get_weight_size(self):
        return self.weights_size

    def add_layer(self, number_of_neurons):
        self.weights_size += number_of_neurons * self.last_layer_size
        self.weights_shapes.append((number_of_neurons, self.last_layer_size))
        self.last_layer_size = number_of_neurons

    def predict(self, weights, input_values):
        w_index = 0
        neuron_values = input_values
        for current_shape in self.weights_shapes:
            new_neuron_values = []
            for i in range(0, current_shape[0]):
                new_neuron_value = 0
                for j in range(0, current_shape[1]):
                    new_neuron_value += (weights[w_index] * neuron_values[j])
                    w_index += 1
                new_neuron_values.append(self.activation_function("sigmoid",
                                                                  new_neuron_value))
            neuron_values = new_neuron_values

        # For output neuron number = 1
        result = self.activation_function("step", neuron_values[0])
        self.display(input_values, neuron_values, result)
        return result

    def activation_function(self, f_type, x):
        if f_type == "sigmoid":
            return 1 / (1 + math.exp(-x + 1.1))
        elif f_type == "relu":
            return np.maximum(0, x)
        elif f_type == "softmax":
            expo = np.exp(x)
            expo_sum = np.sum(np.exp(x))
            return expo / expo_sum
        elif f_type == "step":
            if x < 0.5:
                return 1
            else:
                return 0
        else:
            print("[ERROR] : Wrong activation function")
            exit()

    def display(self, input_values, neuron_values, result):
        print("\nDISTANCE: ", input_values[0], "    HEIGHT: ", input_values[1], "    WIDTH: ", input_values[2])
        if result == 1:
            print(neuron_values[0], "  >  [JUMP]")
        else:
            print(neuron_values[0], "  >  [DO NOTHING]")


def normalize(x):
    distance = x[0] / 600
    height = x[1] / 200
    width = x[2] / 200

    if distance < 0:
        distance = 0.9
    return distance, height, width


def draw_graphic(_winners):
    y = _winners
    x = []
    for i in range(0, len(y)):
        x.append(i)
    
    plt.ylim(0, 1000)
    plt.xlim(0, 50)
    plt.plot(x, y, label="best")
    plt.xlabel('Generation')
    plt.ylabel('Fitness')
    plt.title('Change of Fitness')
    plt.savefig("ChangeOfFitnes.png")


if __name__ == '__main__':
    mutation_rate = 0.15
    population_size = 20
    generation_size = 120
    winners = []

    the_game = Game()
    nn_model = Model(3)
    nn_model.add_layer(6)
    nn_model.add_layer(6)
    nn_model.add_layer(6)
    nn_model.add_layer(1)
    dna_size = nn_model.get_weight_size()

    population = Population(the_game, nn_model, mutation_rate, dna_size, population_size)

    population.live()
    for k in range(0, generation_size):
        min_fit, max_fit = population.max_and_min_fitness()
        winners.append(max_fit)

        population.natural_selection()
        population.next_generation()
        population.live()

    draw_graphic(winners)
    # Get best dino
    best_trex = population.get_best()
    print(best_trex.DNA.get_weights())

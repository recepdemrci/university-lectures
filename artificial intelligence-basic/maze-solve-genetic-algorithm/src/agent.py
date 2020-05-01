import src.setup as setup
from time import sleep

# This class represent a path
class Agent:
    def __init__(self, dna, position):
        self.DNA = dna
        self.fitness = 0
        self.position = position
        self.min_distance = setup.maze_size * 2                                # Distance between agent and target
        self.stopped = False                                                   # True, if stuck or finish or dead

# FUNCTIONS ------------------------------------------------------------------------------------------------------------

    # Fitness Function
    def calculate_fitness(self):
        self.fitness = (1 / self.DNA.gen_index)         # (1 / path which is gone) - (how close to target)
        self.fitness -= self.min_distance               # Which get closer, gets more fitness
        # if self.target_reached():
        #     self.fitness *= 10

    # Control if agent hit the frame or not
    def hit_frame(self):
        if not self.position.in_frame():
            self.display()
            return True
        else:
            return False

    # Control if agent hit an obstacle or not
    def hit_obstacle(self, obstacles):
        for obstacle in obstacles:
            if self.position.hit_obstacle(obstacle):
                self.display()
                return True
        return False

    # Update the position of agent
    def update(self):
        if not self.target_reached():
            (x, y) = self.DNA.get_next()                                       # Get next gen as a direction
            self.position.x += x
            self.position.y += y

    # Move agent to the target until hits any obstacle
    def move(self, obstacles):
        if not self.stopped:
            self.update()

            if self.hit_obstacle(obstacles) or self.hit_frame():
                self.stopped = True

    #  Control if agent reached to the target
    def target_reached(self):
        distance = self.position.distance()                                    # Calculate distance to target
        if distance < self.min_distance:                                       # Control if it is smaller than min_dist
            self.min_distance = distance

        if self.position == setup.target:
            self.stopped = True
            self.display()
            return True
        else:
            return False

    # For visualize the agent
    def display(self, delete=True):
        if not setup.screen:
            return

        agents = []
        (x, y) = (1, 1)
        for i in range(0, self.DNA.gen_index):
            if self.DNA.genes[i] == 1:
                tmp_agent = setup.canvas.create_line(y * setup.grid, x * setup.grid,
                                                     (y-1) * setup.grid, x * setup.grid, fill='red')
                (x, y) = (x, y - 1)
            elif self.DNA.genes[i] == 2:
                tmp_agent = setup.canvas.create_line(y * setup.grid, x * setup.grid,
                                                     y * setup.grid, (x - 1) * setup.grid, fill='red')
                (x, y) = (x - 1, y)
            elif self.DNA.genes[i] == 3:
                tmp_agent = setup.canvas.create_line(y * setup.grid, x * setup.grid,
                                                     (y + 1) * setup.grid, x * setup.grid, fill='red')
                (x, y) = (x, y + 1)
            else:
                tmp_agent = setup.canvas.create_line(y * setup.grid, x * setup.grid,
                                                     y * setup.grid, (x + 1) * setup.grid, fill='red')
                (x, y) = (x + 1, y)
            # sleep(0.15)
            agents.append(tmp_agent)
            setup.canvas.update()

        if delete:
            for i in agents:
                setup.canvas.delete(i)
        else:
            setup.canvas.mainloop()

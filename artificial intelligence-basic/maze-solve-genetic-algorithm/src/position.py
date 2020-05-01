import src.setup as setup


class Position:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.n = setup.maze_size

    def __eq__(self, other):
        if self.x == other.x and self.y == other.y:
            return True
        else:
            return False

    def distance(self):
        dx = self.x - setup.target.x
        dy = self.y - setup.target.y
        dist = abs(dy) + abs(dx)
        return dist

    # Create obstacle in the given position
    def create_obstacle(self, vertical):
        if vertical:
            obstacle = [(self.x, self.y), (self.x + 1, self.y), (self.x + 2, self.y), (self.x + 3, self.y), (self.x + 4, self.y),
                        (self.x, self.y + 1), (self.x + 1, self.y + 1), (self.x + 2, self.y + 1), (self.x + 3, self.y + 1), (self.x + 4, self.y + 1)]
        else:
            obstacle = [(self.x, self.y), (self.x, self.y + 1), (self.x, self.y + 2), (self.x, self.y + 3), (self.x, self.y + 4),
                        (self.x + 1, self.y), (self.x + 1, self.y + 1), (self.x + 1, self.y + 2), (self.x + 1, self.y + 3), (self.x + 1, self.y + 4)]

        self.display(vertical)
        return obstacle

    # Control if position is in the frame or not
    def in_frame(self):
        if (self.x <= 0) or (self.y <= 0) or (self.x >= self.n) or (self.y >= self.n):
            return False
        else:
            return True

    # Control if position hit an obstacle or not
    def hit_obstacle(self, obstacle):
        if obstacle is None:
            return False
        else:
            for item in obstacle:
                if (self.x, self.y) == item:
                    return True
            return False

    # For visualize the maze
    def display(self, vertical):
        x = setup.target.x
        y = setup.target.y
        setup.canvas.create_oval(x * setup.grid - 5, y * setup.grid - 5,
                                 x * setup.grid + 5, y * setup.grid + 5, fill='blue')
        x = setup.start[0]
        y = setup.start[1]
        setup.canvas.create_oval(x * setup.grid - 5, y * setup.grid - 5,
                                 x * setup.grid + 5, y * setup.grid + 5, fill='green')

        if vertical:
            setup.canvas.create_rectangle(self.y * setup.grid, self.x * setup.grid,
                                          (self.y + 1) * setup.grid, (self.x + 4) * setup.grid,
                                          fill='black')
        else:
            setup.canvas.create_rectangle(self.y * setup.grid, self.x * setup.grid,
                                          (self.y + 4) * setup.grid, (self.x + 1) * setup.grid,
                                          fill='black')

import position
import settings


# About Node class
class Node:
    def __init__(self, parent=None, pos=None):
        self.f = 0
        self.g = 0
        self.h = 0

        self.parent = parent
        self.position = pos

    def __eq__(self, other):
        return self.position == other.position

    # Calculate cost of the node based on red value of the pixel
    def get_cost(self):

        r_value = settings.image.item(self.position.x, self.position.y, 2)         # Get red value of the pixel
        return (255 - r_value) + 1                                                 # If r:255, cost:1 | If r:0, cost:256

    # Set f_g_h values of given node for A* Algorithm
    def set_f_g_h_values(self, end_node):
        self.g = self.parent.g + self.get_cost()
        self.h = self.position.get_distance(end_node.position) * self.get_cost()   # Heuristic function can be optimize.( And in always optimistic)
        self.f = self.g + self.h

    # Set f_h values of given node for Best First Algorithm
    def set_f_h_values(self, end_node):
        self.h = self.position.get_distance(end_node.position) * self.get_cost()
        self.f = self.h

    # Generate 8 adjacent child
    def generate_child(self):
        children = []

        for new_position in [(-1, -1), (-1, 1), (1, 1), (1, -1), (0, -1), (-1, 0), (0, 1), (1, 0)]:
            node_position = position.Position(self.position.x + new_position[0], self.position.y + new_position[1])
            if node_position.is_in_range():
                child = Node(self, node_position)
                children.append(child)
        return children

    # Get path as an array by looking of nodes parents
    def get_path(self):
        path = []

        current_node = self
        while current_node is not None:                                        # Append nodes until you reach the root
            path.append(current_node)
            current_node = current_node.parent
        return path
# -------------------------------------------------------------------------------------------------------------------

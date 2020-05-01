import math
import settings


# About Position class
class Position:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __eq__(self, other):
        return (self.x == other.x) and (self.y == other.y)

    # Calculate Euclidean distance between two node
    def get_distance(self, end):
        dx = end.x - self.x
        dy = end.y - self.y
        distance = math.sqrt(dx**2 + dy**2)
        return distance

    # Control if Position is in the image
    def is_in_range(self):
        image_height = settings.image.shape[0]
        image_width = settings.image.shape[1]
        if (self.y < 0) or (self.y >= image_width) or (self.x < 0) or (self.x >= image_height):
            return False
        else:
            return True
# ----------------------------------------------------------------------------------------------------------------------

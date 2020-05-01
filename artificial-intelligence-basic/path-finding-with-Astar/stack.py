# About Stack class. It is a stack implementation for our algorithms
class Stack:
    def __init__(self):
        self.items = []
        self.count_pop = 0
        self.count_max = 0

    # Push the item into end of the stack
    def push(self, item):
        self.items.append(item)
        if self.size() > self.count_max:
            self.count_max = self.size()

    # Find the item which has minimum f value
    def get_minimum(self):
        return min(self.items, key=lambda item: item.f)

    # Pop the item which has minimum f value
    def pop_minimum(self):
        if self.is_empty():
            raise ValueError('Empty stack')
        else:
            self.count_pop += 1
            index = self.items.index(self.get_minimum())
            return self.items.pop(index)

    # Return current size of the stack
    def size(self):
        return len(self.items)

    # Control if stack is empty
    def is_empty(self):
        return self.size() == 0

    # Control if give node is in the stack
    def contains(self, node):
        flag = False
        i = 0
        while not flag and i < len(self.items):
            if node == self.items[i] and node.g >= self.items[i].g:
                flag = True
            else:
                i += 1

        return flag

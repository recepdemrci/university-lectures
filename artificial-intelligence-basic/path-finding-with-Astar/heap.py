# About Heap class. It is a heap implementation for our algorithms
class Heap:
    def __init__(self):
        self.items = []
        self.count_pop = 0
        self.count_max = 0

    # Push the item into the heap
    def push(self, node):
        node_index = len(self.items)
        parent_index = int((node_index - 1) / 2)

        self.items.append(node)                                                                # Add node into the list then
        while (self.items[node_index].f < self.items[parent_index].f) and (node_index >= 1):   # Swap parent and child
            self.swap(node_index, parent_index)                                                # until find right place for node
            node_index = parent_index
            parent_index = int((node_index - 1) / 2)

        if len(self.items) > self.count_max:
            self.count_max = len(self.items)

    # Pop the root item which has minimum f value
    def pop(self):
        if self.is_empty():
            raise ValueError('Empty stack')
        else:
            self.swap(0, len(self.items)-1)                                        # Swap first and last element
            min_node = self.items.pop()                                            # Take the last node of the heap (which is minimum)
            self.heapify()                                                         # Find the right place for root node (heapify)
            self.count_pop += 1
            return min_node

    # Swap the the given two nodes
    def swap(self, first_index, second_index):
        tmp = self.items[first_index]
        self.items[first_index] = self.items[second_index]
        self.items[second_index] = tmp

    # Find minimum child of given node and return it's index
    def get_min_child(self, parent_index):
        left_index = 2 * parent_index + 1                                          # index of node's left child
        right_index = 2 * parent_index + 2                                         # index of node's right child

        if len(self.items) > right_index:                                          # If node has 2 child
            if self.items[right_index].f > self.items[left_index].f:
                return left_index
            else:
                return right_index
        elif len(self.items) == right_index:                                       # If node has 1 child
            return left_index
        else:                                                                      # If node has no child
            return 0

    # Find the right place for first node (heapify)
    def heapify(self):
        parent_index = 0                                                           # Start from root
        child_index = self.get_min_child(parent_index)

        while (child_index != 0) and (self.items[parent_index].f > self.items[child_index].f):
            self.swap(child_index, parent_index)                                   # Swap parent and child
            parent_index = child_index                                             # until find right place for node
            child_index = self.get_min_child(parent_index)

    # Control if stack is empty
    def is_empty(self):
        return len(self.items) == 0

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

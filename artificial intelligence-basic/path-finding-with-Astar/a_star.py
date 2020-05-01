from stack import Stack
from heap import Heap
from node import Node


# Find path with A* Algorithm using heap
def a_star_with_heap(start, end):

    if not start.is_in_range() or not end.is_in_range():                       # Control if nodes are in the image
        return None

    start_node = Node(None, start)                                             # Create start node
    start_node.g = 0
    start_node.h = 0
    start_node.f = 0
    end_node = Node(None, end)                                                 # Create end node
    end_node.g = 0
    end_node.h = 0
    end_node.f = 0

    heap = Heap()                                                              # Create the heap class
    visited = []                                                               # Create the list for visited nodes

    heap.push(start_node)                                                      # Add start_node into the heap
    while not heap.is_empty():
        current_node = heap.pop()                                              # Pop from the heap(which has min f value
        visited.append(current_node)                                           # Add current_node into the visited list

        if current_node == end_node:                                           # If we arrive end_point then return path
            return current_node.get_path(), heap.count_max, heap.count_pop

        children = current_node.generate_child()                               # Generate children based on 8 adjacent
        for child in children:
            if not visited.__contains__(child):                                # Control if child is not visited
                child.set_f_g_h_values(end_node)

                if not heap.contains(child):                                   # Control if child is not in the heap
                    heap.push(child)                                           # push child into the heap
    return None                                                                # path is not found then return error


# Find path with A* Algorithm using stack
def a_star_with_stack(start, end):

    if not start.is_in_range() or not end.is_in_range():                       # Control if nodes are in the image
        return None

    start_node = Node(None, start)                                             # Create start node
    start_node.g = 0
    start_node.h = 0
    start_node.f = 0
    end_node = Node(None, end)                                                 # Create end node
    end_node.g = 0
    end_node.h = 0
    end_node.f = 0

    stack = Stack()                                                            # Create the stack class
    visited = []                                                               # Create the list for visited nodes

    stack.push(start_node)                                                     # Add start_node into the stack
    while not stack.is_empty():
        current_node = stack.pop_minimum()                                     # Pop from stack(which has min f value)
        visited.append(current_node)                                           # Add current_node into the visited list

        if current_node == end_node:                                           # If we arrive end_point then return path
            return current_node.get_path(), stack.count_max, stack.count_pop

        children = current_node.generate_child()                               # Generate children based on 8 adjacent
        for child in children:
            if not visited.__contains__(child):                                # Control if child is not visited
                child.set_f_g_h_values(end_node)

                if not stack.contains(child):                                  # Control if child is not in the stack
                    stack.push(child)                                          # push child into the stack
    return None                                                                # path is not found then return error

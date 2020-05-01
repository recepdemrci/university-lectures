import cv2
import time
from datetime import timedelta
import position
import settings
import a_star
import best_first


# Draws the given path on the image and shows it
def write_path(path):

    while len(path) > 0:
        node = path.pop()
        x = node.position.x
        y = node.position.y

        settings.image[x, y] = [255, 255, 255]

    cv2.imshow("Path", settings.image)
    cv2.waitKey(0)


# Manage all program
def main():

    image_name = input("\n> Write image name : ")                              # Take image path from user
    settings.image = cv2.imread(image_name)                                    # Read the image
    if settings.image is None:                                                 # Control if image was found
        print(" Image not found ")
        return -1

    print("\n> Select the algorithm you want to use")
    selection = input("  1) A* with heap\n  2) A* with stack\n  3) BestFirst with heap\n  4) BestFirst with stack\n  >")

    # Define coordinate of start and end node
    print("\n  Image size is (", settings.image.shape[0]-1, ", ", settings.image.shape[1]-1, ")")
    x = int(input("> Enter the x coordinates of starting position : "))
    y = int(input("> Enter the y coordinates of starting position : "))
    start = position.Position(x, y)
    x = int(input("> Enter the x coordinates of ending position : "))
    y = int(input("> Enter the y coordinates of ending position : "))
    end = position.Position(x, y)

    star = time.time()
    if selection == "1":
        path, count_max, count_pop = a_star.a_star_with_heap(start, end)     # Find the optimum path with A* using heap
    elif selection == "2":
        path, count_max, count_pop = a_star.a_star_with_stack(start, end)    # Find the optimum path with A* using stack
    elif selection == "3":
        path, count_max, count_pop = best_first.bfs_with_heap(start, end)    # Find the path with Best First using heap
    elif selection == "4":
        path, count_max, count_pop = best_first.bfs_with_stack(start, end)   # Find the path with Best First using stack
    else:
        print(" Invalid Selection ")
        path, count_max, count_pop = None, None, None
    elapsed = (time.time() - star)

    if path is None:
        print(" ERROR: Path not found or start/end coordinates not given correctly")
    else:
        print("\nExecution took: %s secs" % timedelta(seconds=round(elapsed)))
        print("Maximum number of heap/stack elements : ", count_max)
        print("Number of pop() called : ", count_pop)
        print(" > Image printed on your screen! ")
        write_path(path)                                                       # draw the path on the image and show it


if __name__ == '__main__':
    main()

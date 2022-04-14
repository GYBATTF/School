# Alexander Harms
# Set of functions to run Conaway's Game of Life in python


# Using sys for input and time for watchability delay
import sys
import time


def empty_plane(x, y):
    '''''
    Initializes and empty plane for creating the playing field and
    loops through creating the list since if I try to do it a quick and
    easy way it creates a mess of a linked list that cant have changed
    parts without effecting the rest of the list
    '''''
    plane = []
    for r in range(y):
        plane.append([False]*x)
    return plane


def seed_plane_position(plane, x, y):
    '''''
    Checks and makes sure that the given coordinates exists within the array
    and if not returns an empty list, else it flips that location to true and
    then returns the modified list
    '''''
    if len(plane) - 1 < y or len(plane[0]) - 1 < x:
        return plane
    plane[y][x] = True
    return plane


def seed_plane(plane):
    '''''
    Prompts for an x and y value on a single line and splits it and uses
    those values to feed into seed_plane_position(). When EOF or an empty line
    is returned it returns a plane with the given positions seeded
    '''''
    line = sys.stdin.readline()
    while line != "":
        x = int(line.split()[0])
        y = int(line.split()[1])
        plane = seed_plane_position(plane, x, y)
        line = sys.stdin.readline()
    return plane


def print_plane(plane):
    '''''
    Loops though each "x" line printing without a newline, prints a newline
    when the "y" value changes and then when the end of the plane is reached
    a blank newline is printed. Nothing is returned.
    '''''
    for y in plane:
        for x in y:
            if x:
                print("o", end="")
            else:
                print(" ", end="")
        print()
    print()


def count_neighbors(plane, x, y):
    '''''
    Finds the x and y
    values surrounding a cell either alive or dead and checks to see if it
    is first in range of the plane, second checks to make sure we arent
    couting the cell we're couting the neighbors for, and then checks to see
    if the neighbor is alive or dead and if its alive incriments the counter.
    When finished counting returns the number of neighbors,
    '''''
    neighbors = 0
    # Find the range of y neighbors
    for r in range(y - 1, y + 2):
        # Find the range of x neighbors
        for c in range(x - 1, x + 2):
            # Checks to make sure we are within bounds
            if (r > -1 and r < len(plane)) and (c > -1 and c < len(plane[0])):
                # Checks that we are not ourself
                if r != y or c != x:
                    # Checks if we are alive
                    if plane[r][c]:
                        neighbors += 1
    return neighbors


def run_timestep(plane):
    '''''
    Applies the rules for the game of life. After rules are applied returns
    a new plane with the updating cells.  Only need to specify rules that
    keep the cell alive because the blank plane that we are fetching already
    will show that, and all, cells as dead.
    '''''
    newplane = empty_plane(len(plane[0]), len(plane))
    # Loops for running through the plane
    for yloc in range(len(plane)):
        for xloc in range(len(plane[0])):
            # Count the neighbors
            neighbors = count_neighbors(plane, xloc, yloc)
            # If the cell is dead but has 3 neighbors bring to life
            if not plane[yloc][xloc] and neighbors == 3:
                newplane[yloc][xloc] = True
            # If the cell is alive and has 3 neighbors keep alive
            if plane[yloc][xloc] and neighbors == 3:
                newplane[yloc][xloc] = True
            # If the cell is alive and has 2 neighbors keep alive
            if plane[yloc][xloc] and neighbors == 2:
                newplane[yloc][xloc] = True
    return newplane


def play_life():
    '''''
    Prompts for the x size, y size, and number of iterations. Creates a new
    seeded plane the size that was entered. Prints the plane and initializes
    a count. Loops through updating the plane with the rules applied each
    iteration, increases the counter and pauses briefly. If the counter hits
    what was entered we are done else if the user enters -1 it will loop
    indefinately
    '''''
    x = sys.stdin.readline()
    y = sys.stdin.readline()
    iter = sys.stdin.readline()
    plane = seed_plane(empty_plane(int(x), int(y)))
    print_plane(plane)
    ct = 0
    while ct != int(iter):
        plane = run_timestep(plane)
        print_plane(plane)
        ct += 1
        time.sleep(.1)

# global variables
player_turn = 1
grid = [[]]

# initialize 2d array to represent game board
def init_grid():
    grid = [[1,1,1,1],
            [0,0,0,0],
            [0,0,0,0],
            [0,0,0,0],
            [2,2,2,2]]
    return grid

# choose which player goes first
def next_turn():
    if player_turn is 1:
        return 2
    else:
        return 1

# print out board in console
def display_board():
    for row in grid:
        print(row)
    print("\n")

# move player piece from one location to desired location
def move(from_location, to_location):
    from_row = from_location[0]
    from_col = from_location[1]
    to_row = to_location[0]
    to_col = to_location[1]

    print(grid[from_row][from_col])
    print(player_turn)

    if player_turn is 1 and grid[from_row][from_col] is 1:
        grid[from_row][from_col] = 0
        grid[to_row][to_col] = 1
    
    elif player_turn is 2 and grid[from_row][from_col] is 2:
        grid[from_row][from_col] = 0
        grid[to_row][to_col] = 2

    else:
        print("Invalid move!")

def main():
    global player_turn
    global grid
    
    grid = init_grid()
    display_board()
    move([0,0], [1,1])
    player_turn = next_turn()
    display_board()
    move([4,0], [3,1])
    player_turn = next_turn()
    display_board()

if __name__ == '__main__':
    main()


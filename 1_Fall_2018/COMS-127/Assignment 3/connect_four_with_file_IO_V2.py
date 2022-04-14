# Alexander Harms
# Script to play the game connect four
# This version includes 2 additional functions for creating/reading save files
from sys import exit


def clear_board():
    '''
    Creates and returns an empty board with 7 columns and 6 rows.

    A board is empty if it contains only zeros.  During play, cells
    controlled by player 1 will get the value 1, and cells controlled by
    player 2 will get the value 2.
    '''
    board = []
    for r in range(6):
        board.append([0] * 7)
    return board


def print_board(board):
    '''
    Prints board with column headers above.  Player 1's tokens should be
    rendered with an 'x', player 2's tokens should be rendered with an 'o',
    and unplayed cells should be rendered with a space.  The column headers
    should be a-g, left to right.  The following is an example of a printed
    board and column headers in a drawn final position:

    abcdefg
    xxooxxx
    oxxxoxo
    oxooooo
    xoxoxox
    xooxxoo
    xxoxoxo
    '''
    b = [" ", "x", "o"]
    print("abcdefg")
    for r in board:
        for c in r:
            print(b[c], end="")
        print()


def is_valid_move(board, column):
    '''
    Returns True if and only if there is an open cell in column.
    '''
    validity = False
    if board[0][column] == 0:
        validity = True
    return validity


def read_move(player):
    '''
    Prompts player for a move.  Reads a column name, a-g, from the
    terminal.  Retries in the case of erroneous input.  Returns an
    integer in the range [0,6] corresponding to a-g respectively.

    Hint: Use ord() to convert a letter to an integer.
          ord(column) - ord('a') will give a "distance from 'a'".
    '''
    col = ""
    while col.lower().strip() not in {"a", "b", "c", "d", "e", "f", "g"}:
        col = input(f"Player {player} enter where you would like to drop your token,\nor press Ctrl+C to exit.\n>> ")
    return ord(col.lower().strip()) - ord("a")


def is_open_position(board, column, row):
    '''
    Returns True if and only if there is no token in the position indexed
    by column and row.
    '''
    validity = True
    if board[row][column] != 0:
        validity = False
    return validity


def board_has_open_position(board):
    '''
    Returns True if and only if there is at least one open position on
    board (i.e., there is at least one more legal move to play).
    '''
    validity = True
    if 0 not in board[0]:
        validity = False
    return validity


def drop_token(board, player, column):
    '''
    Returns board identical to the input except that the lowest open
    cell in column is set to player.

    It is an error to call this function if a play in column is not a
    valid move.  You do not have to account for that error.  play_turn()
    will ensure that drop_token() is called in a valid fashion.
    '''
    for r in reversed(range(0, 6)):
        if board[r][column] == 0:
            board[r][column] = player
            return board


def play_turn(board, player):
    '''
    Reads player's move from the terminal (calls read_move()).  If the
    move is valid, calls drop_token() to update the board, otherwise
    read_move() is called until the move is valid.  Returns the updated
    board.
    '''
    try:
        move = read_move(player)
        while not is_valid_move(board, move):
            print("Sorry! You cannot place a token in this column.")
            move = read_move(player)
    except KeyboardInterrupt:
        save = ""
        while save not in ["y", "n"]:
            save = input("\nWould you like to save? (y/n)\n>> ").strip().lower()
            if save == "y":
                print("Saving...")
                makesave(board, turn)
                print("Your game will automatically resume next time you play.")
                print("Thank you for playing!")
                exit()
            if save == "n":
                makesave(clear_board(), 0)
                print("Thanks for playing!")
                exit()

    return drop_token(board, player, move)


def player_wins_at(board, column, row):
    '''
    Returns True if and only if there is a four-in-a-row passing
    through the position at column and row in any direction.

    This function is complete.  You should not modify it.
    '''

    sum = 0
    c = column - 1
    r = row - 1
    while c >= 0 and r >= 0 and board[r][c] == board[row][column]:
        c -= 1
        r -= 1
        sum += 1
    c = column + 1
    r = row + 1
    while c < 7 and r < 6 and board[r][c] == board[row][column]:
        c += 1
        r += 1
        sum += 1
    if sum >= 3:
        return True

    sum = 0
    c = column - 1
    r = row + 1
    while c >= 0 and r < 6 and board[r][c] == board[row][column]:
        c -= 1
        r += 1
        sum += 1
    c = column + 1
    r = row - 1
    while c < 7 and r >= 0 and board[r][c] == board[row][column]:
        c += 1
        r -= 1
        sum += 1
    if sum >= 3:
        return True

    sum = 0
    c = column - 1
    while c >= 0 and board[row][c] == board[row][column]:
        c -= 1
        sum += 1
    c = column + 1
    while c < 7 and board[row][c] == board[row][column]:
        c += 1
        sum += 1
    if sum >= 3:
        return True

    sum = 0
    r = row - 1
    while r >= 0 and board[r][column] == board[row][column]:
        r -= 1
        sum += 1
    r = row + 1
    while r < 6 and board[r][column] == board[row][column]:
        r += 1
        sum += 1
    if sum >= 3:
        return True

    return False


def player_wins(board, player):
    '''
    Returns True if and only if player has won the game (i.e., player
    has four tokens in a row somewhere on the board.

    Hint: Call player_wins_at().
          It's only necessary to test for wins starting with the top
          token in each column, because any win involving a token not
          at the top of its column must either include the top token
          or it would have already been winning in an earlier turn.
    '''
    validity = False
    for r in reversed(range(0, 6)):
        for c in reversed(range(0, 7)):
            if board[r][c] == player:
                if player_wins_at(board, c, r):
                    validity = True
    return validity


def play_game():
    '''
    Configures and plays one game of Connect 4.

    This function is complete.  You should not modify it.
    '''
    global turn
    board = resumesave("b")
    turn = resumesave("t")
    players = [ 1, 2 ]
    print_board(board)
    while (not player_wins(board, players[(turn + 1) % 2]) and
           board_has_open_position(board)):
        board = play_turn(board, players[turn % 2])
        print_board(board)
        turn += 1
    if player_wins(board, 1):
        print("Player 1 wins!")
    elif player_wins(board, 2):
        print("Player 2 wins!")
    else:
        print("It's a draw.")
    makesave(clear_board(), 0)


def resumesave(state):
    '''
    Resumes from a save file and returns the board from the file if entered
    state is 'b' and return the current turn if the entered state is 't'
    Save file format is the first line is the current turn which is used to
    determine which player goes followed by each row on a line to rebuild
    the game board
    '''
    board = []
    try:
        b = open("connect_four_save", 'r')
        lines = b.readlines()
        b.close()
        if state == 'b':
            for line in lines[1:]:
                row = []
                for r in line.strip("\n"):
                    row.extend([int(r)])
                board.append(row)
            returner = board
        elif state == 't':
            returner = int(lines[0])
    except FileNotFoundError:
        if state == 'b':
            returner = clear_board()
        elif state == 't':
            returner = 0
    return returner


def makesave(board, turn):
    '''
    Opens the save file and writes the current turn and board layout
    First line of the save file is the current turn with each following
    line being a row of the game board
    '''
    b = open("connect_four_save", 'w')
    b.write(str(turn) + "\n")
    for r in board:
        for c in r:
            b.write(str(c))
        b.write("\n")
    b.close()

play_game()

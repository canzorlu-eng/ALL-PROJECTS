import sys
def print_board(board, score):
    """Prints the board and score."""
    for row in board:
        print(*row)
    print("Your score is:", score)

def find_neighbors(board, row, col, value, delet):
    """
    Recursively finds neighboring cells with the same value.
    Args:
        board: The game board.
        row: The current row.
        col: The current column.
        value: The value to match.
        delet: A list to store coordinates of cells to be destroyed.
    Returns:
        None
    """
    # Check if within board 
    if 0 <= row < len(board) and 0 <= col < len(board[0]):
        # Check if current cell value matches and hasn't been visited
        if board[row][col] == value and (row, col) not in delet:
            delet.append((row, col))
            # Recursively check neighbors
            find_neighbors(board, row-1, col, value, delet)
            find_neighbors(board, row, col-1, value, delet)
            find_neighbors(board, row+1, col, value, delet)
            find_neighbors(board, row, col+1, value, delet)

def destroy_cells(board, delet):
    """
    Destroys cells in the delet list and updates the board.
    Args:
        board: The game board.
        delet: A list of coordinates of cells to be destroyed.
    Returns:
        int: The number of cells destroyed.
    """

    cells_destroyed = 0
    for row, col in delet:
        a=board[row][col]
        board[row][col] =" "
        cells_destroyed += 1    
    return cells_destroyed , a

def update_board(board):
    rows = len(board)
    cols = len(board[0])
    for col in range(cols):
        for row in range(rows):
            if board[row][col] == " ":
                    for i in range(row-1,-1,-1):
                        tmp = board[i+1][col]
                        board[i+1][col] = board[i][col]
                        board [i][col] = tmp
    # Find empty columns
    empty_columns = [col for col in range(cols) if all(board[row][col] == " " for row in range(rows))]

    # Remove empty columns
    for empty_col in reversed(empty_columns):
        for row in range(rows):
            del board[row][empty_col]

    # Find empty rows
    empty_rows = [row for row in range(rows) if all(board[row][col] == " " for col in range(cols))]

    # Remove empty rows
    for empty_row in reversed(empty_rows):
        for col in range(cols):
            del board[empty_row][col]




def is_game_over(board):
    rows = len(board)
    cols = len(board[0])

    for r in range(rows):
        for c in range(cols):
            if board[r][c] != " ":
                svalue = board[r][c]
                neg = [[r + 1, c], [r - 1, c], [r, c - 1], [r, c + 1]]
                for i in neg:
                    if 0 <= i[0] < rows and 0 <= i[1] < cols and board[i[0]][i[1]] == svalue:
                        return False

    return True

def main():
    input_file = open(sys.argv[1], "r")
    board = []
    for line in input_file:
        row = list(map(int, line.strip().split()))
        board.append(row)
    score = 0
    while True:
        print_board(board, score)
        while True:
            try:
                row, col = map(int, input("Please enter a row and a column number: ").split())
                rows = len(board)
                cols = len(board[0])
                if not (1 <= row <= rows and 1 <= col <= cols):
                    raise ValueError
                break
            except ValueError:
                print("Please enter a correct size!")
        delet = []
        find_neighbors(board, row-1, col-1, board[row-1][col-1], delet)
        if len(delet)==1:
            print("No movement happened try again")

        elif len(delet) > 1:
            cells_destroyed , a = destroy_cells(board, delet)
            score += cells_destroyed * a 
            update_board(board)

        if is_game_over(board):
            print_board(board, score)
            print("Game over")
            break

if __name__ == "__main__":
    main()






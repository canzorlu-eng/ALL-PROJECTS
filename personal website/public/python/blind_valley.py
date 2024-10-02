import sys
def is_valid(board, row, col, tile, constraints):
    if tile == 'HB':
        a=board[row][col] != 'B'
        b=board[row][col - 1] != 'H'
        c=board[row - 1][col] != 'H'
        if col<2:
          d=board[row][col+2]!="B"
        else:
            d=True
        return a and b and c and d
        
    elif tile == 'BH':
        a=board[row][col] != 'H'
        b=board[row][col - 1] != 'B'
        c=board[row - 1][col] != 'B'
        if col<2:
          d=board[row][col+2]!="H"
        else:
            d=True
        return a and b and c and d
    else:
        return True

def check(board, constraints):
    def checkrowforbase(board, constraints):
        for i in range(len(board)):
            a = 0
            for j in range(len(board[i])):
                if board[i][j] == 'B':
                    a += 1
            if constraints[i] != -1 and a != constraints[i]:
                return False
        return True

    def checkrowforhigh(board, constraints):
        for i in range(len(board)):
            a = 0
            for j in range(len(board[0])):
                if board[i][j] == 'H':
                    a += 1
            if constraints[i] != -1 and a != constraints[i]:
                return False
        return True

    def checkcolforhigh(board, constraints):
        for i in range(len(board[0])):
            b = 0
            for j in range(len(board)):
                if board[j][i] == 'H':
                    b += 1
            if constraints[i] != -1 and b != constraints[i]:
                return False
        return True

    def checkcolforbase(board, constraints):
        for i in range(len(board[0])):
            b = 0
            for j in range(len(board)):
                if board[j][i] == 'B':
                    b += 1
            if constraints[i] != -1 and b != constraints[i]:
                return False
        return True

    d = checkrowforhigh(board, constraints[0])  # b
    c = checkrowforbase(board, constraints[1])  # a
    b = checkcolforhigh(board, constraints[2])  # c
    a = checkcolforbase(board, constraints[3])  # d
    return a and b and c and d

def solve(template, board, row, col, constraints):
    if row == len(board):
        if check(board, constraints):
            return True  # Solution found
    next_row = row + (col + 1) // len(board[0])
    next_col = (col + 1) % len(board[0])

    if row < len(template):  # Check if row index is within template bounds
        if template[row][col] == "L":  # Horizontal tile
            for tile in ["HB", "BH", "NN"]:
                if is_valid(board, row, col, tile, constraints):
                    board[row][col:col + 2] = tile
                    if solve(template, board, next_row, next_col, constraints):
                        return True
                    board[row][col:col + 2] = [" ", " "]

        elif template[row][col] == "U":  # Vertical tile
            for tile in ["HB", "BH", "NN"]:
                if is_valid(board, row, col, tile, constraints):
                    board[row][col] = tile[0]
                    board[row + 1][col] = tile[1]
                    if solve(template, board, next_row, next_col, constraints):
                        return True
                    board[row][col] = " "
                    board[row + 1][col] = " "

        else:
            return solve(template, board, next_row, next_col, constraints)

    return False

def print_board(board,out):
    b=1
    for row in board:
        a=" ".join(row)
        if b==len(board):
            out.write(f"{a}")
        else:
            out.write(f"{a}\n")
            b+=1

def blind_valley_solver(constraints, template,out):
    rows, cols = len(template), len(template[0])
    board = [[' ' for _ in range(cols)] for _ in range(rows)]
    if solve(template, board, 0, 0, constraints):
        if check(board, constraints):
            print_board(board,out)
    else:
        out.write("No solution!")

def main():
    # Read input from a file
    file=open(sys.argv[1], "r")
    out=open(sys.argv[2], "w")
    # Read constraints
    constraints = [list(map(int, file.readline().split())) for _ in range(4)]
    # Read template
    template = [list(file.readline().split()) for _ in range(len(constraints[0]))]

    # Solve the puzzle
    blind_valley_solver(constraints, template,out)

if __name__ == "__main__":
    main()

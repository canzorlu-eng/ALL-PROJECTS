import sys

def write_output(board, row, col, num,output_file,step):
    step += 1
    output_file.write("------------------\n")
    output_file.write("Step {} - {} @ R{}C{}\n".format(step, num, row + 1, col + 1))
    output_file.write("------------------\n")
    for row in board:
        a = (" ".join(map(str, row)))
        output_file.write("{}\n".format(a))


def control(board, row, col, num):
    # For row control
    if num in board[row]:
        return False
    # For col control
    elif num in [board[i][col] for i in range(9)]:
        return False
    # For square control
    first_row, first_col = 3 * (row // 3), 3 * (col // 3)
    for i in range(3):
        for j in range(3):
            if board[first_row + i][first_col + j] == num:
                return False
    return True


def find_suit_cells(board):
    suit_cells = []
    for row in range(9):
        for col in range(9):
            if board[row][col] == 0:
                possible_values = []
                for num in range(1, 10):
                    if control(board, row, col, num):
                        possible_values.append(num)

                if len(possible_values) == 1:
                    suit_cells.append((row, col))

    return suit_cells


def solve_suit_cells(board, suit_cells,output_file,step):
    for row, col in suit_cells:
        for num in range(1, 10):
            if control(board, row, col, num):
                board[row][col] = num
                write_output(board, row, col, num,output_file,step)
                if solving(board,output_file,step):
                    return True
                # For turning
                board[row][col] = 0


def solving(board,output_file,step):
    empty_cell = find_empty_cell(board)
    if not empty_cell:
        return True  # completed
    row, col = empty_cell

    suit_cells = find_suit_cells(board)
    solve_suit_cells(board, suit_cells,output_file,step)

    if suit_cells:
        return True

    for num in range(1, 10):
        if control(board, row, col, num):
            board[row][col] = num
            write_output(board, row, col, num,output_file)
            if solving(board):
                return True
        # For turning
        board[row][col] = 0
    return False


def find_empty_cell(board):
    for i in range(9):
        for j in range(9):
            if board[i][j] == 0:
                return (i, j)
    return None


def read_file(input_file):
    lines = input_file.readlines()
    sudoku_board = [[int(num) for num in line.split()] for line in lines]
    return sudoku_board


def main():
    step = 0
    input_file = open(sys.argv[1], "r")
    output_file = open(sys.argv[2], "w")
    sudoku_board = read_file(input_file)
    solving(sudoku_board,output_file,step)
    output_file.write("------------------")
    input_file.close()
    output_file.close()
if __name__ == "__main__":
    main()
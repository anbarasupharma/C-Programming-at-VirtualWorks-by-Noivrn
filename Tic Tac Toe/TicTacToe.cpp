#include <iostream>
#include <limits>
using namespace std;

// ---------- Game board ----------
char board[3][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'}
};

char currentPlayer = 'X';

// ---------- Display the board ----------
void displayBoard() {
    cout << "\n";
    for (int i = 0; i < 3; ++i) {
        cout << " " << board[i][0] << " | " << board[i][1] << " | " << board[i][2] << "\n";
        if (i < 2) cout << "---+---+---\n";
    }
    cout << "\n";
}

// ---------- Check for a winner ----------
// Returns 'X' or 'O' if someone won, 'D' for draw, or ' ' if game continues.
char checkResult() {
    // Rows
    for (int i = 0; i < 3; ++i)
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];

    // Columns
    for (int j = 0; j < 3; ++j)
        if (board[0][j] == board[1][j] && board[1][j] == board[2][j])
            return board[0][j];

    // Diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];

    // Check for empty cells (game still going)
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] != 'X' && board[i][j] != 'O')
                return ' ';   // game continues

    return 'D';  // draw
}

// ---------- Place a mark ----------
bool placeMove(int cell) {
    if (cell < 1 || cell > 9) return false;

    int row = (cell - 1) / 3;
    int col = (cell - 1) % 3;

    if (board[row][col] == 'X' || board[row][col] == 'O')
        return false;          // cell already taken

    board[row][col] = currentPlayer;
    return true;
}

// ---------- Main game loop ----------
int main() {
    cout << "=== Tic-Tac-Toe ===\n";
    cout << "Player 1 = X   |   Player 2 = O\n";
    cout << "Enter a number (1-9) to place your mark.\n";

    char result = ' ';

    while (result == ' ') {
        displayBoard();
        cout << "Player " << currentPlayer << ", enter your move: ";

        int move;
        if (!(cin >> move)) {
            // Handle non-integer input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 9.\n";
            continue;
        }

        if (!placeMove(move)) {
            cout << "Invalid move! Try again.\n";
            continue;
        }

        result = checkResult();

        if (result == ' ')
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    // Final board and result
    displayBoard();

    if (result == 'D')
        cout << "It's a draw!\n";
    else
        cout << "Player " << result << " wins! Congratulations!\n";

    return 0;
}

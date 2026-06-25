#include <iostream>
using namespace std;

char board[3][3];

void initBoard() {
    int count = 1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = '0' + count++;
}

void displayBoard() {
    cout << "\n";
    for (int i = 0; i < 3; i++) {
        cout << " " << board[i][0] << " | " << board[i][1] << " | " << board[i][2] << "\n";
        if (i < 2)
            cout << "---+---+---\n";
    }
    cout << "\n";
}

bool checkWin(char player) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
            return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
            return true;
    }
    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return true;

    return false;
}

bool checkDraw() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 'X' && board[i][j] != 'O')
                return false;
    return true;
}

bool makeMove(int pos, char player) {
    if (pos < 1 || pos > 9) {
        cout << "Invalid input! Enter a number between 1 and 9.\n";
        return false;
    }

    int row = (pos - 1) / 3;
    int col = (pos - 1) % 3;

    if (board[row][col] == 'X' || board[row][col] == 'O') {
        cout << "That cell is already taken! Try again.\n";
        return false;
    }

    board[row][col] = player;
    return true;
}

int main() {
    char playAgain;

    cout << "=============================\n";
    cout << "   Welcome to Tic Tac Toe!   \n";
    cout << "=============================\n";

    do {
        initBoard();

        char currentPlayer = 'X';
        int totalMoves = 0;
        bool gameOver = false;

        cout << "\nPositions are numbered 1-9 as shown:\n";
        displayBoard();

        while (!gameOver) {
            int pos;
            cout << "Player " << currentPlayer << ", enter your move (1-9): ";
            cin >> pos;

            if (!makeMove(pos, currentPlayer))
                continue;

            totalMoves++;
            displayBoard();

            if (checkWin(currentPlayer)) {
                cout << "🎉 Player " << currentPlayer << " wins! Congratulations!\n";
                gameOver = true;
            } else if (checkDraw()) {
                cout << "It's a draw! Well played by both!\n";
                gameOver = true;
            } else {
                // Switch player
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        }

        cout << "\nDo you want to play again? (y/n): ";
        cin >> playAgain;

    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "\nThanks for playing! Goodbye.\n";

    return 0;
}

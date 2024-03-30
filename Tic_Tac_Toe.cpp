#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class Initialise {
public:
    static void initialise(vector<vector<char>>& board) {
        int n = board.size();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                board[i][j] = '*';
            }
        }
    }
};

class CrossedCheck {
public:
    static bool rowCrossed(vector<vector<char>>& board) {
        for (int i = 0; i < board.size(); i++) {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][1] != '*')
                return true;
        }
        return false;
    }

    static bool columnCrossed(vector<vector<char>>& board) {
        for (int i = 0; i < board.size(); i++) {
            if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != '*')
                return true;
        }
        return false;
    }

    static bool diagonalCrossed(vector<vector<char>>& board) {
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != '*')
            return true;
        if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != '*')
            return true;
        return false;
    }
};

class Algorithm {
public:
    static int miniMax(vector<vector<char>>& board, int depth, bool isAI);

    static int bestMove(vector<vector<char>>& board, int moveIndex);
};

class PlayGame {
public:
    static void whoWin(int n);

    static bool gameOver(vector<vector<char>>& board);

    static void playTicTacToe(int who);
};

class Instruction {
public:
    static void showBoard(vector<vector<char>>& board);

    static void showInstructions();
};

class Game {
public:
    static const int Computer = 1;
    static const int Human = 2;
    static const int Side = 3;
    static const char ComputerMove = 'O';
    static const char HumanMove = 'X';

    static void start();
};

int Algorithm::miniMax(vector<vector<char>>& board, int depth, bool isAI) {
    int bestScore;
    if (CrossedCheck::rowCrossed(board) || CrossedCheck::columnCrossed(board) || CrossedCheck::diagonalCrossed(board)) {
        return isAI ? -10 : +10;
    } else {
        if (depth < 9) {
            if (isAI) {
                bestScore = -999;
                for (int i = 0; i < Game::Side; i++) {
                    for (int j = 0; j < Game::Side; j++) {
                        if (board[i][j] == '*') {
                            board[i][j] = Game::ComputerMove;
                            int score = miniMax(board, depth + 1, false);
                            board[i][j] = '*';
                            bestScore = max(score, bestScore);
                        }
                    }
                }
                return bestScore;
            } else {
                bestScore = 999;
                for (int i = 0; i < Game::Side; i++) {
                    for (int j = 0; j < Game::Side; j++) {
                        if (board[i][j] == '*') {
                            board[i][j] = Game::HumanMove;
                            int score = miniMax(board, depth + 1, true);
                            board[i][j] = '*';
                            bestScore = min(score, bestScore);
                        }
                    }
                }
                return bestScore;
            }
        }
    }
    return 0;
}

int Algorithm::bestMove(vector<vector<char>>& board, int moveIndex) {
    int x = -1, y = -1;
    int score, bestScore = -999;
    for (int i = 0; i < Game::Side; i++) {
        for (int j = 0; j < Game::Side; j++) {
            if (board[i][j] == '*') {
                board[i][j] = Game::ComputerMove;
                score = miniMax(board, moveIndex + 1, false);
                board[i][j] = '*';
                if (score > bestScore) {
                    bestScore = score;
                    x = i;
                    y = j;
                }
            }
        }
    }
    return x * 3 + y;
}

void PlayGame::whoWin(int n) {
    if (n == 1)
        cout << "Computer Win the match!!!" << endl;
    else
        cout << "You Win the match!!!" << endl;
}

bool PlayGame::gameOver(vector<vector<char>>& board) {
    return (CrossedCheck::rowCrossed(board) || CrossedCheck::columnCrossed(board) || CrossedCheck::diagonalCrossed(board));
}

void PlayGame::playTicTacToe(int who) {
    vector<vector<char>> board(3, vector<char>(3));
    int moveIndex = 0, x = 0, y = 0;
    Initialise::initialise(board);
    Instruction::showInstructions();
    while (!gameOver(board) && moveIndex != Game::Side * Game::Side) {
        int n;
        if (who == Game::Computer) {
            n = Algorithm::bestMove(board, moveIndex);
            x = n / Game::Side;
            y = n % Game::Side;
            board[x][y] = Game::ComputerMove;
            cout << "Computer has put a " << Game::ComputerMove << " in cell " << (n + 1) << "\n" << endl;
            Instruction::showBoard(board);
            moveIndex++;
            who = Game::Human;
        } else if (who == Game::Human) {
            cout << "You can insert the following position:" << endl;
            for (int i = 0; i < Game::Side; i++) {
                for (int j = 0; j < Game::Side; j++) {
                    if (board[i][j] == '*')
                        cout << (i * 3 + j) + 1 << " ";
                }
            }
            cout << "\nEnter the Position: " << endl;
            cin >> n;
            n--;
            x = n / Game::Side;
            y = n % Game::Side;
            if (board[x][y] == '*' && n < 9 && n >= 0) {
                board[x][y] = Game::HumanMove;
                cout << "Human has put a " << Game::HumanMove << " in cell " << (n + 1) << "\n" << endl;
                Instruction::showBoard(board);
                moveIndex++;
                who = Game::Computer;
            } else if (board[x][y] != '*' && n < 9 && n >= 0)
                cout << "\nPosition is occupied, select the correct place from available place!!" << endl;
            else if (n < 0 || n > 8)
                cout << "Invalid Place" << endl;
        }
    }
    if (!gameOver(board) && moveIndex == Game::Side * Game::Side)
        cout << "It is Draw!!" << endl;
    else {
        if (who == Game::Computer)
            who = Game::Human;
        else
            who = Game::Computer;
        whoWin(who);
    }
}

void Instruction::showBoard(vector<vector<char>>& board) {
    cout << "\t\t\t" << board[0][0] << "|" << board[0][1] << "|" << board[0][2] << endl;
    cout << "\t\t\t" << "-----" << endl;
    cout << "\t\t\t" << board[1][0] << "|" << board[1][1] << "|" << board[1][2] << endl;
    cout << "\t\t\t" << "-----" << endl;
    cout << "\t\t\t" << board[2][0] << "|" << board[2][1] << "|" << board[2][2] << endl;
}

void Instruction::showInstructions() {
    cout << "Choose a cell number from 1 to 9 as below and play\n" << endl;
    cout << "\t\t\t" << "1" << "|" << "2" << "|" << "3" << endl;
    cout << "\t\t\t" << "4" << "|" << "5" << "|" << "6" << endl;
    cout << "\t\t\t" << "7" << "|" << "8" << "|" << "9" << "\n" << endl;
}

void Game::start() {
    cout << "Tic-Tac-Toe Game" << endl;
    cout << "Welcome!!" << endl;
    cout << "This is a simple Tic-Tac-Toe Game created by Suman Das\n" << endl;
    cout << "There is 3X3 board where you put the 'X' or 'O'" << endl;

    char count = 'y';
    do {
        cout << "Do you want to start first?(y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
            PlayGame::playTicTacToe(Human);
        else if (choice == 'n' || choice == 'N')
            PlayGame::playTicTacToe(Computer);
        else
            cout << "Invalid Choice" << endl;

        cout << "Do you want to quit(y/n): ";
        cin >> count;
    } while (count == 'n' || count == 'N');

    cout << "Thanks For Using" << endl;
}

int main() {
    Game::start();
    return 0;
}

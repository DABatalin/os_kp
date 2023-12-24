#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int BOARD_SIZE = 10;

class Player {
public:
    vector< vector<char> > board;

    Player() {
        board = vector< vector<char> >(BOARD_SIZE, vector<char>(BOARD_SIZE, ' '));
    }

    void placeShips() {
        // Расставляем корабли
        cout << "Расставьте ваши корабли (формат: x, y и ориентация (H или V) через пробелы):" << endl;
        for (int i = 1; i <= 5; ++i) {
			char orientation;
            cout << "Разместите корабль " << i << " (1x" << i << "): ";
            int x, y;
            cin >> x >> y >> orientation;

            if (isValidPlacement(x, y, i, orientation)) {
                placeShip(x, y, i, orientation);
            } else {
                cout << "Неверное местоположение! Попробуйте еще раз." << endl;
                --i;
            }

            printBoard();
        }
    }

    bool isValidPlacement(int x, int y, int size, char orientation) const {
        if (orientation == 'V') {
            if (x + size - 1 >= BOARD_SIZE) {
                return false;
            }

            for (int i = x; i < x + size; ++i) {
                if (board[i][y] != ' ') {
                    return false;
                }
            }
        } else if (orientation == 'H') {
            if (y + size - 1 >= BOARD_SIZE) {
                return false;
            }

            for (int j = y; j < y + size; ++j) {
                if (board[x][j] != ' ') {
                    return false;
                }
            }
        }

        return true;
    }

    void placeShip(int x, int y, int size, char orientation) {
        if (orientation == 'V') {
            for (int i = x; i < x + size; ++i) {
                board[i][y] = 'O';
            }
        } else if (orientation == 'H') {
            for (int j = y; j < y + size; ++j) {
                board[x][j] = 'O';
            }
        }
    }

    void printBoard() const {
        cout << "  0 1 2 3 4 5 6 7 8 9" << endl;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            cout << i << " ";
            for (int j = 0; j < BOARD_SIZE; ++j) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

class Game {
public:
    Player player1, player2;

    void play() {
        cout << "Игра \"Морской бой\" началась!" << endl;

        // Расставляем корабли для каждого игрока
        player1.placeShips();
        player2.placeShips();

        // Начинаем игру
        while (!gameOver()) {
            cout << "Ход игрока 1:" << endl;
            playerTurn(player1, player2);
            if (gameOver()) break;

            cout << "Ход игрока 2:" << endl;
            playerTurn(player2, player1);
        }

        cout << "Игра завершена!" << endl;
    }

    bool gameOver() const {
        return allShipsSunk(player1) || allShipsSunk(player2);
    }

    bool allShipsSunk(const Player& player) const {
        for (const auto& row : player.board) {
            for (char cell : row) {
                if (cell == 'O') {
                    return false;
                }
            }
        }
        return true;
    }

    void playerTurn(Player& attacker, Player& defender) {
        int x, y;
        cout << "Введите координаты выстрела (формат: x y): ";
        cin >> x >> y;

        if (isValidMove(x, y, defender)) {
            if (defender.board[x][y] == 'O') {
                cout << "Попадание!" << endl;
                defender.board[x][y] = 'X';
            } else {
                cout << "Промах!" << endl;
                defender.board[x][y] = '*';
            }

            defender.printBoard();
        } else {
            cout << "Неверные координаты! Попробуйте еще раз." << endl;
            playerTurn(attacker, defender);
        }
    }

    bool isValidMove(int x, int y, const Player& defender) const {
        return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE &&
               (defender.board[x][y] == ' ' || defender.board[x][y] == 'O');
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    Game game;
    game.play();

    return 0;
}

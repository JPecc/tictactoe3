#include <iostream>
#include <string>
#include <fstream>

using namespace std;

bool checkWinNormal(const string cells[], const string mark) {
    for (int i = 0; i < 9; i += 3) {
        if (cells[i] == mark && cells[i + 1] == mark && cells[i + 2] == mark) {
            return true;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (cells[i] == mark && cells[i + 3] == mark && cells[i + 6] == mark) {
            return true;
        }
    }
    if ((cells[0] == mark && cells[4] == mark && cells[8] == mark) ||
        (cells[2] == mark && cells[4] == mark && cells[6] == mark)) {
        return true;
    }
    return false;
}

bool checkWinBattleSwarm(const string cells[], const string mark) {
    if (checkWinNormal(cells, mark)) {
        return true;
    }
    if (cells[0] == mark && cells[2] == mark && cells[6] == mark && cells[8] == mark) {
        return true;
    }
    return false;
}

bool checkTie(const string cells[]) {
    for (int i = 0; i < 9; i++) {
        if (cells[i] != "X" && cells[i] != "O") {
            return false;
        }
    }
    return true;
}

void displayBoard(const string cells[]) {
    string horizontal = "---+---+---";
    char vert = '|';
    char space = ' ';

    for (int i = 0; i < 7; i += 3) {
        cout << space << cells[i] << space << vert << space << cells[i + 1] << space << vert << space << cells[i + 2] << endl;
        if (i < 6) {
            cout << horizontal << endl;
        }
    }
}

void resetGame(string cells[], string &current_mark, bool &game_over) {
    for (int i = 0; i < 9; i++) {
        cells[i] = to_string(i + 1);
    }
    current_mark = "X";
    game_over = false;
    cout << "\nThe game has been reset.\n";
}

bool playAgain() {
    string response;
    cout << "Do you want to play again? (y/n): ";
    getline(cin, response);

    if (response == "y" || response == "Y" || response == "yes" || response == "Yes") {
        return true;
    }
    return false;
}

void createGameReport(int gamesPlayed, int player1Wins, int player2Wins, int ties) {
    ofstream reportFile("game_report.txt");

    if (reportFile.is_open()) {
        reportFile << "Game Report\n";
        reportFile << "-----------------\n";
        reportFile << "Games Played: " << gamesPlayed << endl;
        reportFile << "Player 1 Wins: " << player1Wins << endl;
        reportFile << "Player 2 Wins: " << player2Wins << endl;
        reportFile << "Ties: " << ties << endl;
        reportFile.close();
        cout << "Game report saved to game_report.txt\n";
    } else {
        cout << "Unable to create the report file.\n";
    }
}

int main() {
    bool keep_playing = true;
    int gamesPlayed = 0;
    int player1Wins = 0;
    int player2Wins = 0;
    int ties = 0;

    while (keep_playing) {
        string cells[9] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
        string mark_player1 = "X";
        string mark_player2 = "O";
        string current_mark = mark_player1;
        bool game_over = false;
        bool battle_mode = false;
        bool player1_choice_battle = false;
        bool player2_choice_battle = false;

        cout << "Welcome To The TIC-TAC-TOE simulator\n" << endl;

        cout << "Choose a game mode:\n";
        cout << "1) Regular Tic-Tac-Toe\n";
        cout << "2) Battle Tic-Tac-Toe\n";
        cout << "Enter your choice (1 or 2): ";
        int choice;
        cin >> choice;
        cin.ignore();

        battle_mode = (choice == 2);

        if (battle_mode) {
            cout << "Battle Tic-Tac-Toe mode selected!\n";

            cout << "\n-ARCHEYPES-\n" << endl;

            cout << "1) Swarm" << endl;
            cout << "Passive Ability: Can win the game by filling in all four corners of the board" << endl;
            cout << "Special Ability: None\n" << endl;

            cout << "2) Pyromancer" << endl;
            cout << "Passive Ability: None" << endl;
            cout << "Special Ability: Can clear the board one time per game. This ability acts as your turn.\n" << endl;

            cout << "-All archetypes have regular TIC-TAC-TOE win conditions unless stated otherwise-\n" << endl;

            int player1_choice;
            while (true) {
                cout << "Player 1 (" << "X" << "), do you want to use 'Swarm' (1) or 'Pyromancer' (2)? ";
                cin >> player1_choice;

                if (cin.fail() || (player1_choice != 1 && player1_choice != 2)) {
                    cin.clear(); 
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Try again." << endl;
                } else {
                    break;
                }
            }
            player1_choice_battle = (player1_choice == 1);

            int player2_choice;
            while (true) {
                cout << "Player 2 (" << "O" << "), do you want to use 'Swarm' (1) or 'Pyromancer' (2)? ";
                cin >> player2_choice;

                if (cin.fail() || (player2_choice != 1 && player2_choice != 2)) {
                    cin.clear();
                    cin.ignore(1000, '\n');  
                    cout << "Invalid input. Try again." << endl;
                } else {
                    break;
                }
            }
            player2_choice_battle = (player2_choice == 1);

            cout << "Battle Tic-Tac-Toe mode is now ready. Player 1: "
                 << (player1_choice_battle ? "'Swarm'" : "'Pyromancer'") << ", Player 2: "
                 << (player2_choice_battle ? "'Swarm'" : "'Pyromancer'") << endl;
        } else {
            cout << "Normal Tic-Tac-Toe mode selected\n";
        }

        bool player1_used_special = false;
        bool player2_used_special = false;

        while (!game_over) {
            displayBoard(cells);

            string move_choice;
            if (battle_mode) {
                cout << "Player " << (current_mark == mark_player1 ? "1" : "2") << " (" << current_mark << "), choose your move:\n";
                cout << "r) Regular move\n";
                if ((current_mark == mark_player1 && !player1_choice_battle && !player1_used_special) ||
                    (current_mark == mark_player2 && !player2_choice_battle && !player2_used_special)) {
                    cout << "s) Special move\n";
                }
                cin >> move_choice;
                cin.ignore();

                if (move_choice == "s") {
                    if (current_mark == mark_player1 && !player1_used_special && !player1_choice_battle) {
                        resetGame(cells, current_mark, game_over);
                        player1_used_special = true;
                    } else if (current_mark == mark_player2 && !player2_used_special && !player2_choice_battle) {
                        resetGame(cells, current_mark, game_over);
                        player2_used_special = true;
                    }
                    continue;
                }
            }

            string cell_choice;
            cout << "Make your move (1-9): ";
            getline(cin, cell_choice);

            if (cell_choice == "exit") {
                game_over = true;
                break;
            }

            int cell = stoi(cell_choice) - 1;
            if (cell >= 0 && cell < 9 && cells[cell] != mark_player1 && cells[cell] != mark_player2) {
                cells[cell] = current_mark;

                bool player_wins = false;
                if (battle_mode) {
                    if ((current_mark == mark_player1 && player1_choice_battle) || (current_mark == mark_player2 && player2_choice_battle)) {
                        player_wins = checkWinBattleSwarm(cells, current_mark);
                    } else {
                        player_wins = checkWinNormal(cells, current_mark);
                    }
                } else {
                    player_wins = checkWinNormal(cells, current_mark);
                }

                if (player_wins) {
                    displayBoard(cells);
                    cout << "Player " << (current_mark == mark_player1 ? "1" : "2") << " wins" << endl;
                    game_over = true;
                    if (current_mark == mark_player1) {
                        player1Wins++;
                    } else {
                        player2Wins++;
                    }
                } else if (checkTie(cells)) {
                    displayBoard(cells);
                    cout << "It's a tie" << endl;
                    game_over = true;
                    ties++;
                } else {
                    current_mark = (current_mark == mark_player1) ? mark_player2 : mark_player1;
                }
            } else {
                cout << "Invalid move. Try again." << endl;
            }
        }

        cout << "Game Over!" << endl;
        gamesPlayed++;

        keep_playing = playAgain();
    }

    createGameReport(gamesPlayed, player1Wins, player2Wins, ties);

    cout << "Thanks for playing" << endl;
    return 0;
}

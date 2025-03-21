#include <iostream>
#include <fstream>
#include <cstring>
#include "clui.h"

using namespace std;

// global variables:

struct game
{
    char name[100];
    int sudoku[9][9];
};
struct player
{
    char username[100];
    int password;
    bool winner;
    int score = 0;
    int time;
    int right = 0;
    int wrong = 0;
    int loose;
    int win;
    game game;
    int saved_sudoku[9][9];
};

int level = 0, players_count = 0, player_index = 0, full_homes = 0;
bool game_over = false;
player players_array[300];
int row = get_window_rows();
int column = get_window_cols();
int cursor_row = 1, cursor_col = 1;
int users_sudoku[9][9];
int solved_sudoku[9][9];
player now_player;
game now_game;

// functions:
void read_prepared_sudokus();
void start_game();
void login();
void check_username();
void count_full_homes();
void save();
void play_savedgame();
void is_paused();
void edit();
void show_eror();
void game_control();
void choose_level();
void sudoku();
void scoreboard();
void menu();

int main()
{
    check_username();
    menu();

    return 0;
}

void check_username()
{
    int input_password;
    char input_username[100]; // username that user entered to login
    bool found = false;
    players_count = 0;
    change_color_rgb(252, 3, 111);
    cursor_to_pos(row / 2, column / 2);
    cout << "Enter Your Username:" << endl;

    ifstream file_u("usernames.txt", ios::app);

    // Store usernames in array
    while (file_u >> players_array[players_count].username)
    {
        players_count++;
    }
    file_u.close();

    cursor_to_pos(row / 2 + 1, column / 2);
    cin >> input_username;

    // Check if username exists
    for (int i = 0; i < players_count; i++)
    {
        if (strcmp(players_array[i].username, input_username) == 0)
        {
            found = true;
            player_index = i;
            break;
        }
    }

    if (found)
    {
        login();
    }
    else // signup
    {
        delay(1500);
        reset_color();
        clear_screen();
        strcpy(players_array[players_count].username, input_username);

        change_color_rgb(247, 236, 106);
        cursor_to_pos(row / 2 + 2, column / 2);
        cout << "Choose Your Password:" << endl;

        cursor_to_pos(row / 2 + 3, column / 2);
        cin >> players_array[players_count].password;

        ofstream file_u("usernames.txt", ios::app);
        if (file_u.is_open())
        {
            file_u << players_array[players_count].username << endl;
            // file_u << input_username << endl;
        }
        else
        {
            change_color_rgb(173, 10, 7);
            cursor_to_pos(row / 2 + 4, column / 2);
            cout << "Unable to open usernames.txt" << endl;
        }
        file_u.close();

        ofstream file_p("passwords.txt", ios::app);
        if (file_p.is_open())
        {
            file_p << players_array[players_count].password << endl;
            file_p.close();
        }
        else
        {
            change_color_rgb(173, 10, 7);
            cursor_to_pos(row / 2 + 5, column / 2);
            cout << "Unable to open passwords.txt" << endl;
        }

        change_color_rgb(136, 242, 75);
        cursor_to_pos(row / 2 + 4, column / 2);
        cout << "Account successfully created." << endl;
        players_count++; // Increment player count
    }
    delay(1500);
    reset_color();
    clear_screen();
}

void login()
{
    reset_color();
    clear_screen();
    bool found = false;
    int input_password; // password that user entered to login

    change_color_rgb(252, 3, 111);
    cursor_to_pos(row / 2, column / 2);
    cout << "Enter Your Password:" << endl;

    ifstream file_p("passwords.txt");

    for (int i = 0; i < players_count; i++)
    {
        file_p >> players_array[i].password;
    }
    file_p.close();

    cursor_to_pos(row / 2 + 1, column / 2);
    cin >> input_password;

    if (players_array[player_index].password == input_password)
    {
        change_color_rgb(136, 242, 75);
        cursor_to_pos(row / 2 + 2, column / 2);
        cout << "You logged in successfully." << endl;
        delay(2000);
        reset_color();
        clear_screen();
    }
    else
    {
        change_color_rgb(173, 10, 7);
        cursor_to_pos(row / 2 + 2, column / 2);
        cout << "Password is wrong!" << endl;
        change_color_rgb(247, 236, 106);
        cursor_to_pos(row / 2 + 3, column / 2);
        cout << " You can try again :)" << endl;
        delay(1500);
        reset_color();
        clear_screen();
        login();
    }
}
void choose_level()
{
    int choice;
    cursor_to_pos(row / 2, column / 2);
    change_color_rgb(3, 177, 252);
    cout << "choose your level" << endl;
    cursor_to_pos(row / 2 + 1, column / 2);
    cout << "The difficulty level affects the game duration" << endl;
    cursor_to_pos(row / 2 + 2, column / 2);
    cout << "1.Easy level : 5 minutes" << endl;
    cursor_to_pos(row / 2 + 3, column / 2);
    cout << "2.Medium level : 3 minutes" << endl;
    cursor_to_pos(row / 2 + 4, column / 2);
    cout << "3.Hard level : 2 minutes" << endl;
    cursor_to_pos(row / 2 + 5, column / 2);
    change_color_rgb(136, 3, 252);
    cout << "Enter your choice(a number from 1 to 3):" << endl;
    cursor_to_pos(row / 2 + 6, column / 2 + 5);
    cin >> choice;
    delay(1500);
    reset_color();
    clear_screen();
    switch (choice)
    {
    case 1:
        level = 1;
        break;
    case 2:
        level = 2;
        break;
    case 3:
        level = 3;
        break;
    default:
        change_color_rgb(179, 7, 7);
        cursor_to_pos(row / 2 + 7, column / 2);
        cout << "invalid number:(";
        delay(1500);
        reset_color();
        clear_screen();
        break;
    }
}
void start_game()
{
    change_color_rgb(250, 169, 115);
    cursor_to_pos(row / 2, column / 2);
    cout << "Game will be started soon" << endl;
    cursor_to_pos(row / 2 + 1, column / 2);
    cout << "You can use these keys while playing:" << endl;
    cursor_to_pos(row / 2 + 2, column / 2);
    cout << "j:left k:down l:right i:up" << endl;
    cursor_to_pos(row / 2 + 3, column / 2);
    cout << "HAVE FUN :D" << endl;
    getch();
    reset_color();
    clear_screen();
    read_prepared_sudokus();
    sudoku();
}
void menu()
{
    int choice;
    cursor_to_pos(row / 2, column / 2);
    change_color_rgb(3, 252, 223);
    cout << "1.Start a New GAME" << endl;
    cursor_to_pos(row / 2 + 1, column / 2);
    cout << "2.Play a Saved GAME" << endl;
    cursor_to_pos(row / 2 + 2, column / 2);
    cout << "3.LeaderBoard" << endl;
    cursor_to_pos(row / 2 + 3, column / 2);
    cout << "4.Exit the Game" << endl;
    cursor_to_pos(row / 2 + 4, column / 2);
    cout << "Enter your choice(a number from 1 to 4):" << endl;
    cursor_to_pos(row / 2 + 6, column / 2 + 5);
    change_color_rgb(136, 3, 252);
    cin >> choice;
    delay(1500);
    reset_color();
    clear_screen();

    switch (choice)
    {
    case 1:
        choose_level();
        start_game();
        break;
    case 2:
        play_savedgame();

        break;
    case 3:
        scoreboard();
        break;
    case 4:
    {
        check_username();
        menu();
        choose_level();
        start_game();
    }
    break;
    default:
        change_color_rgb(179, 7, 7);
        cursor_to_pos(row / 2 + 6, column / 2);
        cout << "invalid number:(";
        delay(1500);
        reset_color();
        clear_screen();
        break;
    }
}
void read_prepared_sudokus() // if not new game
{
    ifstream file_prepared("prepared sudokus.txt");
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            file_prepared >> users_sudoku[i][j];

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            file_prepared >> solved_sudoku[i][j];
    file_prepared.close();
}
void sudoku()
{
    for (int i = 0; i < 9; i++) // print sudoku that user should solve
    {
        for (int j = 0; j < 9; j++)
        {
            cout << users_sudoku[i][j] << " ";
            if ((j + 1) % 3 == 0 && j != 8)
            {
                cout << "| ";
            }
        }
        cout << endl;
        if ((i + 1) % 3 == 0 && i != 8)
            cout << "-----------------" << endl;
    }

    cursor_to_pos(1, 1);
    while (!game_over)
    {
        game_control();
        if (now_player.wrong >= 5)
            game_over = true;
        else if (full_homes >= 81)
            game_over = true;
    }

    if (now_player.wrong >= 5) // loser
    {
        clear_screen();
        change_color_rgb(179, 7, 7);
        cout << "sorry...:( " << endl
             << "you can try again";
        reset_color();
        now_player.loose++;
    }
    if (full_homes >= 81) // winnner
    {
        clear_screen();
        change_color_rgb(247, 183, 237);
        cout << "YOU WON ଘ(੭ˊᵕˋ)੭* ੈ✩‧₊ " << endl;
        reset_color();
        now_player.win++;
    }

    ofstream file_score("scoreboard", ios::app);
    file_score << now_player.username << endl;
    // saved_infos << now_player.time << endl;
    file_score << now_player.score << endl;
    file_score << now_player.right << endl;
    file_score << now_player.wrong << endl;
    file_score << now_player.win << endl;
    file_score << now_player.loose << endl;
    file_score.close();
}

void game_control()
{
    char input;
    input = getch();
    switch (input)
    {
    case 'i':
        if (cursor_row > 0)
        {
            cursor_up(1);
            cursor_row--;
        }
        break;
    case 'k':
        if (cursor_row < 21)
        {
            cursor_row++;
            cursor_down(1);
        }
        break;
    case 'j':
        if (cursor_col > 0)
        {
            cursor_col--;
            cursor_backward(1);
        }
        break;
    case 'l':
        if (cursor_col < 21)
        {
            cursor_col++;
            cursor_forward(1);
        }
        break;
    case 'e':
        if (users_sudoku[cursor_row][cursor_col] != 0)
        {
            play_beep();
            show_eror();
        }
        else
            edit();
        break;
    case 27:
        is_paused();
        break;
    case 's':
        save();
        break;
    case 'q':
        game_over = true;
        clear_screen();
        change_color_rgb(179, 7, 7);
        cout << "LOOSER:(";
        now_player.loose++;
        reset_color();
        clear_screen();
    }
}
void edit()
{
    count_full_homes();
    int input_number;
    cin >> input_number;
    if (input_number == solved_sudoku[cursor_row][cursor_col])
    {
        users_sudoku[cursor_row][cursor_col] == input_number;
        now_player.score++;
        full_homes++;
    }
    else
    {
        play_beep();
        change_color_rgb(179, 7, 7);
        cursor_to_pos(25, 0);
        cout << "Wrong Answer";
        getch();
        reset_color();
        cursor_to_pos(25, 0);
        cout << "                       ";
        now_player.score--;
        now_player.wrong++;
    }
}

void show_eror()
{
    change_color_rgb(179, 7, 7);
    cursor_to_pos(25, 0);
    cout << "This home is fixed!!";
    getch();
    reset_color();
    cursor_to_pos(25, 0);
    cout << "                       ";
}
void count_full_homes()
{
    full_homes = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (users_sudoku[i][j] != 0)
                full_homes++;
}

void is_paused()
{
    char input;
    clear_screen();
    change_color_rgb(247, 246, 183);
    cout << "Game is paused" << endl;
    cout << "Press Esc to continue" << endl;

    do
    {
        input = getch();
        if (input == 27)
        {
            clear_screen();
            cout << "Game is starting again" << endl;
            sudoku();
            reset_color();
        }
    } while (input != 27);
}

void timer()
{
    for (int i = 1; i <= 100; i++)
    {
        cursor_to_pos(5, get_window_cols() / 2);
        change_color_rgb(237, 66, 14);
        cout << i << endl;
        delay(100);
        clear_screen();
    }
}

void save()
{
    char input_gamename[100];
    bool found = false;

    ifstream file_gamenames("game names.txt");
    for (int i = 0; i < players_count; i++)
    {
        file_gamenames >> players_array[i].game.name;
    }
    do
    {
        change_color_rgb(252, 3, 111);
        cout << "Chooes a name for game:" << endl;
        reset_color;
        cin >> input_gamename;
        for (int i = 0; i < players_count; i++)
        {
            if (strcmp(players_array[i].game.name, input_gamename) == 0)
            {
                found = true;
                break;
            }
        }
        clear_screen();
        if (found)
        {
            change_color_rgb(247, 236, 163);
            cout << "try another name" << endl;
            reset_color;
        }

        else
        {
            players_array[players_count].game.name;
            ofstream file_gamenames("usernames.txt", ios::app);
            file_gamenames << players_array[players_count].game.name << endl;
        }
    } while (found);

    file_gamenames.close();

    ofstream saved_infos("saved infos", ios::app);
    saved_infos << now_player.username << endl;
    saved_infos << now_player.game.name << endl;
    saved_infos << now_player.time << endl;
    saved_infos << now_player.right << endl;
    saved_infos << now_player.wrong << endl;
    saved_infos.close();

    ofstream file_saved_sudokus("saved sudokus", ios::app);
    file_saved_sudokus << players_array[players_count].game.name;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            file_saved_sudokus << users_sudoku[i][j];
    file_saved_sudokus.close();
    change_color_rgb(206, 247, 183);
    cout << "Your Game Saved Successfully" << endl;
    delay(1500);
    reset_color();
    clear_screen();
}

void play_savedgame()
{
    string input_gn;
    char reading_username[100];
    bool found = false;
    ifstream saved_infos("saved infos");
    do
    {
        saved_infos >> reading_username;
        if (strcmp(reading_username, players_array[players_count].username) == 0)
            found = true;
    } while (!found);
    saved_infos >> players_array[players_count].game.name;

    change_color_rgb(149, 245, 227);
    cout << "Your game name is:" << endl;
    cout << players_array[players_count].game.name;
    reset_color();
    saved_infos.close();

    ifstream file_saved_sudokus("saved sudokus.txt");
    while (file_saved_sudokus >> reading_username)
        if (strcmp(players_array[players_count].game.name, reading_username) == 0)
        {
            for (int i = 0; i < 9; i++)
                for (int j = 0; j < 9; j++)
                    file_saved_sudokus >> users_sudoku[i][j];

            file_saved_sudokus.ignore();
        }

    file_saved_sudokus.close();

    int **temp = new int *[9];
    for (int i = 0; i < 9; i++)
    {
        temp[i] = new int[9];
    }
    ifstream file_prepared("prepared sudokus.txt");
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            file_prepared >> temp[i][j];
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            file_prepared >> solved_sudoku[i][j];

    file_prepared.close();
    for (int i = 0; i < 9; i++)
    {
        delete[] temp[i];
    }
    delete[] temp;
}

void scoreboard()
{
    char input;
    clear_screen();
    ifstream file_score("scoreboard");
    for (int i = 0; i < players_count; i++)
    {
        file_score >> players_array[i].username;
        // saved_infos >> players_array[i].time;
        file_score >> players_array[i].score;
        file_score >> players_array[i].right;
        file_score >> players_array[i].wrong;
        file_score >> players_array[i].win;
        file_score >> players_array[i].loose;
    }
    file_score.close();
    change_color_rgb(26, 232, 81);
    cout << " username" << " ";
    // cout<< "time:"<< " ";
    cout << "score" << " ";
    cout << "right answers " << " ";
    cout << "wrong answers" << " ";
    cout << "wins" << " ";
    cout << "looses" << " ";
    cout << endl;

    change_color_rgb(255, 10, 157);
    for (int i = 0; i < players_count; i++)
    {
        cout << players_array[i].username << " ";
        // cout<< players_array[i].time<< " ";
        cout << players_array[i].score << " ";
        cout << players_array[i].right << " ";
        cout << players_array[i].wrong << " ";
        cout << players_array[i].win << " ";
        cout << players_array[i].loose << " ";
        cout << endl;
    }
    change_color_rgb(252, 246, 129);
    cout << "click m to back to menu ";

    input = getch();
    if (input == 'm')
        menu();
    reset_color();
    clear_screen();
}

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <iomanip>
#include <algorithm>

using namespace std;

#define KB_UpArrow 72
#define KB_DownArrow 80
#define KB_LeftArrow 75
#define KB_RightArrow 77
#define KB_SPACE 32
#define KB_ESC 27
#define KB_r 114
#define KB_h 104

struct gameHistory{
    string name;
    int score;
    int time;
};

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

int count_of_game = 0;

void menu(int** game, int row, int column, gameHistory leaderboard[100]){
    cout << "Hi ! Welcome to Tetris";
    sleep(1000);
    system("cls");
    while (true){
        cout << "1 - New Game" << "\n";
        cout << "2 - Leaderboard" << "\n";
        cout << "3 - How to playing Tetris" << "\n";
        cout << "4 - Exit" << "\n";
        int section;
        cin >> section;
        if(section == 1){
            system("cls");
            cout << "Welcome to new game";
            sleep(1000);
            system("cls");
            cout << "Please enter your name : ";
            cin >> leaderboard[count_of_game].name;
            system("cls");
            cout << "Please enter the number of rows : ";
            cin >> row;
            sleep(500);
            cout << "\n" << "Please enter the number of columns : ";
            cin >> column;
            system("cls");
            cout << "GAME Loading ....";
            sleep(1000);
            system("cls");
        }
        else if(section == 2){

        }
        else if(section == 3){

        }
        else if(section == 4){

        }
        else{

        }
    }
}

int main() {

}

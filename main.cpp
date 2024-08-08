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

using namespace std;

struct gameHistory{
    string name;
    int score;
    int time;
};

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

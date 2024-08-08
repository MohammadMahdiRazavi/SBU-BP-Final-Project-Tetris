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

void menu(int** game, int row, int column, gameHistory leaderboard[100]){
    cout << "Hi ! Welcome to Tetris";
    sleep(1000);
    system("cls");
    while (true){
        cout << "1 - New Game";
        cout << "2 - Leaderboard";
        cout << "3 - How to playing Tetris";
        cout << "4 - Exit";
        int section;
        cin >> section;
        if(section == 1){

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

#include <iostream>
#include <vector>
#include <windows.h>
#include <chrono>
#include <conio.h>
#include <random>
#include <thread>
#include <fstream>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;
using namespace chrono;
//----------------------------------------------------
enum colors
{
    BLACK, BLUE, GREEN, CYAN, RED, PURPLE, YELLOW, GREY,
    LIGHTGREY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED,
    LIGHTPURPLE, LIGHTYELLOW, WHITE
};//an int value for every string in it
struct Block{
    int id = 0;
    //space id = 0 , table edges = 1 , other shapes are greater than 1
    bool freeze = false;
};//containing blocks information
struct LeaderBoardMembers{
    string name;
    int score;
    int time;
};
int row , column , x_Center , y_Center , currentShapeId, Score = 0;//x for row and y for column
//2 for S_Shape, 3 for Z_Shape, 4 for L_Shape, 5 for J_Shape, 6 for O_Shape, 7 for I_Shape, 8 for E_Shape and
//second number for their rotation id, zero id means there is no shape that can move(so another shape will be
//created)
int thirdPreviousShapeId = 0, secondPreviousShapeId = 0, previousShapeId = 0, nextShapeId = 0;
int durationTime = 0;
int shapeFallTime = 0 , shapeFreezeTime = 0;
int difficultyLevel = 0;//1 for easy, 2 for normal and 3 for hard
vector<vector<Block>> table;//table for saving and showing shapes
vector<LeaderBoardMembers> leaderboard;//table for storing leader board members data from file

time_point<steady_clock> startOfGame, endOfGame; //for calculating play duration time

string block = "\u25A0"; //block shape string

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ESC 27

bool blockHasMoved = true;//for updating screen(whenever it's true, display will be refreshed)
bool shapeIsFreezed = false;//another shape will be spawn
bool shapeIsGoingDown = false;//indicates when shape should go down
bool delayForFreezeShapeIsOn = true;//a flag for its method
//----------------------------------------------------
void printBlock(int);
void generateTable();
void initializeTable();
void setConsoleColor(int);
void setBlockColor(int);
void spawnShape();
int generateRandomNumber(int , int);
void spawn_S_Shape(int);
void spawn_Z_Shape(int);
void spawn_L_Shape(int);
void spawn_J_Shape(int);
void spawn_O_Shape(int);
void spawn_I_Shape(int);
void spawn_E_Shape(int);
void shapeFall();
void rotateShape();
void moveShapeRight();
void moveShapeLeft();
void S_Shape_Down();
void Z_Shape_Down();
void L_Shape_Down();
void J_Shape_Down();
void O_Shape_Down();
void I_Shape_Down();
void E_Shape_Down();
void S_Shape_Horizontal_Left();
void Z_Shape_Horizontal_Left();
void L_Shape_Horizontal_Left();
void J_Shape_Horizontal_Left();
void O_Shape_Horizontal_Left();
void I_Shape_Horizontal_Left();
void E_Shape_Horizontal_Left();
void S_Shape_Horizontal_Right();
void Z_Shape_Horizontal_Right();
void L_Shape_Horizontal_Right();
void J_Shape_Horizontal_Right();
void O_Shape_Horizontal_Right();
void I_Shape_Horizontal_Right();
void E_Shape_Horizontal_Right();
void S_Shape_Rotate();
void Z_Shape_Rotate();
void L_Shape_Rotate();
void J_Shape_Rotate();
void I_Shape_Rotate();
void E_Shape_Rotate();
bool checkIfMovementIsValid(char);
bool checkIfShapeRotationIsValid();
bool S_Shape_Check_If_Movement_Is_Valid(char);
bool Z_Shape_Check_If_Movement_Is_Valid(char);
bool L_Shape_Check_If_Movement_Is_Valid(char);
bool J_Shape_Check_If_Movement_Is_Valid(char);
bool O_Shape_Check_If_Movement_Is_Valid(char);
bool I_Shape_Check_If_Movement_Is_Valid(char);
bool E_Shape_Check_If_Movement_Is_Valid(char);
bool checkIfBlockIsEmpty(int, int);
bool S_Shape_Check_If_Rotation_Is_Valid();
bool Z_Shape_Check_If_Rotation_Is_Valid();
bool L_Shape_Check_If_Rotation_Is_Valid();
bool J_Shape_Check_If_Rotation_Is_Valid();
bool I_Shape_Check_If_Rotation_Is_Valid();
bool E_Shape_Check_If_Rotation_Is_Valid();
void getArrowKey();
void makeDelayForShapeFall(int);
void makeDelayForFreezeShape(int);
void showConsoleCursor(bool);
void freezeShape();
void unFreezeShape();
void emptyRow(int);
void searchForFullRows();
void calculateScore(int);
int findLastEmptyRow();
void moveRowsDown(int &, int &);
void ignoreBufferInputs();
void gameIsOver();
bool checkIds();
void distributeIds();
void updateIds();
void showNextShape();
void displayMainMenu();
void displayScore();
void startGame();
void gameCore();
void newGame();
void displayHowToPlay();
void displayPauseMenu();
void Exit();
int calculateTime(time_point<steady_clock>, time_point<steady_clock>);
void loadScoreDataFromFile(const string& fileAddress);
void saveScoreDataToFile(const LeaderBoardMembers &data, const string &filename);
int compare(const LeaderBoardMembers& one , const LeaderBoardMembers& two);
void displayLeaderBoard();
void getPlayersInfo();
string outputTime(int time);
void findAndReplacePlayer();
void resetGame();
void saveGameToFile();
void loadGameFromFile();
void startLoadedGame();
void displayOptionAndSettings();
void displayCredit();
void changeBlocksSkin();
//----------------------------------------------------
int main()
{
    displayMainMenu();
}
//----------------------------------------------------
void initializeTable()
{
    //resetting table
    if (!table.empty())
        table.clear();
    //pushing back table
    for (int i = 0 ; i < row + 1 ; i++){
        table.emplace_back(column + 2);

    }
    //making grid for table
    for (int i = 0 ; i < row + 1 ; i++){
        if (i == row){
            for (int j = 0 ; j < column + 2; j++){
                table[i][j].id = 1;
                table[i][j].freeze = true;
            }
            continue;
        }
        table[i][0].id = 1;
        table[i][0].freeze = true;
        table[i][column + 1].id = 1;
        table[i][column + 1].freeze = true;
    }
}
//----------------------------------------------------
void generateTable()
{
    system("cls");
    //for each loop for iterating on members
    for (const vector<Block> &vector1D: table) {
        for (Block x: vector1D) {
            printBlock(x.id);
        }
        cout << "\n";
    }
}
//----------------------------------------------------
void printBlock(int id)
{
    //inspecting blocks id
    if (id == 0){
        cout << " ";
    }
    else {
        setBlockColor(id);
        cout << block;
    }
}
//----------------------------------------------------
void setBlockColor(int id)
{
    //setting block color based on the id
    if (id == 1)
        setConsoleColor(WHITE);
    else if (id == 2)
        setConsoleColor(CYAN);
    else if (id == 3)
        setConsoleColor(LIGHTYELLOW);
    else if (id == 4)
        setConsoleColor(RED);
    else if (id == 5)
        setConsoleColor(BLUE);
    else if (id == 6)
        setConsoleColor(PURPLE);
    else if (id == 7)
        setConsoleColor(LIGHTGREEN);
    else if (id == 8)
        setConsoleColor(LIGHTPURPLE);
}
//----------------------------------------------------
void setConsoleColor(int textColor)
{
    //changing output color
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor));
}
//----------------------------------------------------
void spawnShape()
{
    //spawning shape based on the current id
    int center = column / 2;
    //using center to specify the spawn coord
    if (column == 5)
        center++;
    switch (currentShapeId / 10) {
        case 2://S-Shape
            spawn_S_Shape(center);
            break;
        case 3://Z-Shape
            spawn_Z_Shape(center);
            break;
        case 4://L-Shape
            spawn_L_Shape(center);
            break;
        case 5://J-Shape
            spawn_J_Shape(center);
            break;
        case 6://O-Shape
            spawn_O_Shape(center);
            break;
        case 7://I-Shape
            spawn_I_Shape(center);
            break;
        case 8://E-Shape
            spawn_E_Shape(center);
            break;
    }
}
//----------------------------------------------------
void spawn_S_Shape(int center)
{
    //another random number for its rotation
    switch (currentShapeId % 10) {
        case 1://vertical ,id : 1
            if (checkIfBlockIsEmpty(0 , center) && checkIfBlockIsEmpty(1, center) &&
                checkIfBlockIsEmpty(1 , center + 1) && checkIfBlockIsEmpty(2, center + 1)) {
                table[0][center].id = 2;
                table[1][center].id = 2;
                table[1][center + 1].id = 2;
                table[2][center + 1].id = 2;
                x_Center = 1;
                y_Center = center + 1;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
        case 2://horizontal ,id : 2
            if (checkIfBlockIsEmpty(0 , center) && checkIfBlockIsEmpty(0, center + 1) &&
                checkIfBlockIsEmpty(1 , center) && checkIfBlockIsEmpty(1, center - 1)) {
                table[0][center].id = 2;
                table[0][center + 1].id = 2;
                table[1][center].id = 2;
                table[1][center - 1].id = 2;
                x_Center = 0;
                y_Center = center;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
    }
}
//----------------------------------------------------
void spawn_Z_Shape(int center)
{
    switch (currentShapeId % 10) {
        case 1://vertical ,id : 1
            if (checkIfBlockIsEmpty(0 , center) && checkIfBlockIsEmpty(1, center) &&
                checkIfBlockIsEmpty(1 , center - 1) && checkIfBlockIsEmpty(2, center - 1)) {
                table[0][center].id = 3;
                table[1][center].id = 3;
                table[1][center - 1].id = 3;
                table[2][center - 1].id = 3;
                x_Center = 1;
                y_Center = center - 1;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
        case 2://horizontal ,id : 2
            if (checkIfBlockIsEmpty(0 , center) && checkIfBlockIsEmpty(0, center - 1) &&
                checkIfBlockIsEmpty(1 , center) && checkIfBlockIsEmpty(1, center + 1)) {
                table[0][center].id = 3;
                table[0][center - 1].id = 3;
                table[1][center].id = 3;
                table[1][center + 1].id = 3;
                x_Center = 0;
                y_Center = center;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
    }
}
//----------------------------------------------------
void spawn_L_Shape(int center)
{
    switch (currentShapeId % 10) {
        case 1://vertical-stand ,id : 1
            if (checkIfBlockIsEmpty(0 , center) && checkIfBlockIsEmpty(1, center) &&
                checkIfBlockIsEmpty(2 , center) && checkIfBlockIsEmpty(2, center + 1)) {
                table[0][center].id = 4;
                table[1][center].id = 4;
                table[2][center].id = 4;
                table[2][center + 1].id = 4;
                x_Center = 1;
//            y_Center = center;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
        case 2://vertical-upside down ,id : 2
            if (checkIfBlockIsEmpty(0 , center - 1) && checkIfBlockIsEmpty(0, center) &&
                checkIfBlockIsEmpty(1 , center) && checkIfBlockIsEmpty(2, center)) {
                table[0][center - 1].id = 4;
                table[0][center].id = 4;
                table[1][center].id = 4;
                table[2][center].id = 4;
                x_Center = 1;
//            y_Center = center;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
        case 3://horizontal 1 ,id : 3
            if (checkIfBlockIsEmpty(0 , center + 1) && checkIfBlockIsEmpty(1, center + 1) &&
                checkIfBlockIsEmpty(1 , center) && checkIfBlockIsEmpty(1, center - 1)) {
                table[0][center + 1].id = 4;
                table[1][center + 1].id = 4;
                table[1][center].id = 4;
                table[1][center - 1].id = 4;
                x_Center = 1;
//            y_Center = center;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
        case 4://horizontal 2 ,id : 4
            if (checkIfBlockIsEmpty(0 , center + 1) && checkIfBlockIsEmpty(0, center) &&
                checkIfBlockIsEmpty(0 , center - 1) && checkIfBlockIsEmpty(1, center - 1)) {
                table[0][center + 1].id = 4;
                table[0][center].id = 4;
                table[0][center - 1].id = 4;
                table[1][center - 1].id = 4;
                x_Center = 0;
//            y_Center = center;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
    }
    y_Center = center;
}
//----------------------------------------------------
void spawn_J_Shape(int center)
{
    switch (currentShapeId % 10) {
        case 1://vertical-stand, id : 1
            if (checkIfBlockIsEmpty(0 , center) && checkIfBlockIsEmpty(1, center) &&
                checkIfBlockIsEmpty(2 , center) && checkIfBlockIsEmpty(2, center - 1)) {
                table[0][center].id = 5;
                table[1][center].id = 5;
                table[2][center].id = 5;
                table[2][center - 1].id = 5;
                x_Center = 1;
//            y_Center = center;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
        case 2://vertical-upside down, id : 2
            if (checkIfBlockIsEmpty(0 , center + 1) && checkIfBlockIsEmpty(0, center) &&
                checkIfBlockIsEmpty(1 , center) && checkIfBlockIsEmpty(2, center)) {
                table[0][center + 1].id = 5;
                table[0][center].id = 5;
                table[1][center].id = 5;
                table[2][center].id = 5;
                x_Center = 1;
//            y_Center = center;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
        case 3://horizontal 1, id : 3
            if (checkIfBlockIsEmpty(0 , center - 1) && checkIfBlockIsEmpty(1, center - 1) &&
                checkIfBlockIsEmpty(1 , center) && checkIfBlockIsEmpty(1, center + 1)) {
                table[0][center - 1].id = 5;
                table[1][center - 1].id = 5;
                table[1][center].id = 5;
                table[1][center + 1].id = 5;
                x_Center = 1;
//            y_Center = center;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
        case 4://horizontal 2, id : 4
            if (checkIfBlockIsEmpty(0 , center - 1) && checkIfBlockIsEmpty(0, center) &&
                checkIfBlockIsEmpty(0 , center + 1) && checkIfBlockIsEmpty(1, center + 1)) {
                table[0][center - 1].id = 5;
                table[0][center].id = 5;
                table[0][center + 1].id = 5;
                table[1][center + 1].id = 5;
                x_Center = 0;
//            y_Center = center;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
    }
    y_Center = center;
}
//----------------------------------------------------
void spawn_O_Shape(int center)
{
    //does not need a randomized rotation, id : 1
    if (checkIfBlockIsEmpty(0 , center - 1) && checkIfBlockIsEmpty(0, center) &&
        checkIfBlockIsEmpty(1 , center) && checkIfBlockIsEmpty(1, center - 1)) {
        table[0][center - 1].id = 6;
        table[0][center].id = 6;
        table[1][center].id = 6;
        table[1][center - 1].id = 6;
        x_Center = 0;
        y_Center = center;
    }
    else {
        gameIsOver();
        getPlayersInfo();
        displayMainMenu();
    }
}
//----------------------------------------------------
void spawn_I_Shape(int center)
{
    switch (currentShapeId % 10) {
        case 1://horizontal, id : 1
            if (checkIfBlockIsEmpty(0 , center - 2) && checkIfBlockIsEmpty(0, center - 1) &&
                checkIfBlockIsEmpty(0 , center) && checkIfBlockIsEmpty(0, center + 1)) {
                table[0][center - 2].id = 7;
                table[0][center - 1].id = 7;
                table[0][center].id = 7;
                table[0][center + 1].id = 7;
                x_Center = 0;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
        case 2://vertical, id : 2
            if (checkIfBlockIsEmpty(0 , center) && checkIfBlockIsEmpty(1, center) &&
                checkIfBlockIsEmpty(2 , center) && checkIfBlockIsEmpty(3, center)) {
                table[0][center].id = 7;
                table[1][center].id = 7;
                table[2][center].id = 7;
                table[3][center].id = 7;
                x_Center = 1;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
    }
    y_Center = center;
}
//----------------------------------------------------
void spawn_E_Shape(int center)
{
    switch (currentShapeId % 10) {
        case 1://horizontal pointing up, id : 1
            if (checkIfBlockIsEmpty(0 , center) && checkIfBlockIsEmpty(1, center) &&
                checkIfBlockIsEmpty(1 , center - 1) && checkIfBlockIsEmpty(1, center + 1)) {
                table[0][center].id = 8;
                table[1][center].id = 8;
                table[1][center - 1].id = 8;
                table[1][center + 1].id = 8;
                x_Center = 1;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
        case 2://horizontal pointing down, id : 2
            if (checkIfBlockIsEmpty(0 , center - 1) && checkIfBlockIsEmpty(0, center) &&
                checkIfBlockIsEmpty(0 , center + 1) && checkIfBlockIsEmpty(1, center)) {
                table[0][center - 1].id = 8;
                table[0][center].id = 8;
                table[0][center + 1].id = 8;
                table[1][center].id = 8;
                x_Center = 0;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
        case 3://vertical pointing left, id : 3
            if (checkIfBlockIsEmpty(0 , center) && checkIfBlockIsEmpty(1, center) &&
                checkIfBlockIsEmpty(1 , center - 1) && checkIfBlockIsEmpty(2, center)) {
                table[0][center].id = 8;
                table[1][center].id = 8;
                table[1][center - 1].id = 8;
                table[2][center].id = 8;
                x_Center = 1;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;
        case 4://vertical pointing right, id : 4
            if (checkIfBlockIsEmpty(0 , center) && checkIfBlockIsEmpty(1, center) &&
                checkIfBlockIsEmpty(1 , center + 1) && checkIfBlockIsEmpty(2, center)) {
                table[0][center].id = 8;
                table[1][center].id = 8;
                table[1][center + 1].id = 8;
                table[2][center].id = 8;
                x_Center = 1;
            }
            else {
                gameIsOver();
                getPlayersInfo();
                displayMainMenu();
            }
            break;

    }
    y_Center = center;
}
//----------------------------------------------------
int generateRandomNumber(int N, int P)
{
    //generate random number from N to P
    random_device rd;//creating an object from random_device class
    mt19937 gen(rd());//creating an object from class Mersenne Twister, a Mersenne Twister pseudo-random number generator.
    uniform_int_distribution<> dist(N, P);//Creates an object dist of the uniform_int_distribution class, which generates integers within a specified range.
    return dist(gen);//Returns a random integer generated by the distribution dist, using the pseudo-random engine gen
}
//----------------------------------------------------
void shapeFall()
{
    switch (currentShapeId/10) {
        case 2:
            if (S_Shape_Check_If_Movement_Is_Valid('D')) {
                S_Shape_Down();
                blockHasMoved = true;
            }
            break;
        case 3:
            if (Z_Shape_Check_If_Movement_Is_Valid('D')) {
                Z_Shape_Down();
                blockHasMoved = true;
            }
            break;
        case 4:
            if (L_Shape_Check_If_Movement_Is_Valid('D')) {
                L_Shape_Down();
                blockHasMoved = true;
            }
            break;
        case 5:
            if (J_Shape_Check_If_Movement_Is_Valid('D')) {
                J_Shape_Down();
                blockHasMoved = true;
            }
            break;
        case 6:
            if (O_Shape_Check_If_Movement_Is_Valid('D')) {
                O_Shape_Down();
                blockHasMoved = true;
            }
            break;
        case 7:
            if (I_Shape_Check_If_Movement_Is_Valid('D')) {
                I_Shape_Down();
                blockHasMoved = true;
            }
            break;
        case 8:
            if (E_Shape_Check_If_Movement_Is_Valid('D')) {
                E_Shape_Down();
                blockHasMoved = true;
            }
            break;
    }

}
//----------------------------------------------------
void moveShapeLeft()
{
    switch (currentShapeId/10) {
        case 2:
            S_Shape_Horizontal_Left();
            break;
        case 3:
            Z_Shape_Horizontal_Left();
            break;
        case 4:
            L_Shape_Horizontal_Left();
            break;
        case 5:
            J_Shape_Horizontal_Left();
            break;
        case 6:
            O_Shape_Horizontal_Left();
            break;
        case 7:
            I_Shape_Horizontal_Left();
            break;
        case 8:
            E_Shape_Horizontal_Left();
            break;
    }
}
//----------------------------------------------------
void moveShapeRight()
{
    switch (currentShapeId/10) {
        case 2:
            S_Shape_Horizontal_Right();
            break;
        case 3:
            Z_Shape_Horizontal_Right();
            break;
        case 4:
            L_Shape_Horizontal_Right();
            break;
        case 5:
            J_Shape_Horizontal_Right();
            break;
        case 6:
            O_Shape_Horizontal_Right();
            break;
        case 7:
            I_Shape_Horizontal_Right();
            break;
        case 8:
            E_Shape_Horizontal_Right();
            break;
    }
}
//----------------------------------------------------
void rotateShape()
{
    switch (currentShapeId/10) {
        case 2:
            S_Shape_Rotate();
            break;
        case 3:
            Z_Shape_Rotate();
            break;
        case 4:
            L_Shape_Rotate();
            break;
        case 5:
            J_Shape_Rotate();
            break;
        case 6:
            break;
        case 7:
            I_Shape_Rotate();
            break;
        case 8:
            E_Shape_Rotate();
            break;
    }
}
//----------------------------------------------------
void S_Shape_Down()
{
    switch(currentShapeId){
        case 21:
            swap(table[x_Center + 1][y_Center], table[x_Center + 2][y_Center]);
            swap(table[x_Center][y_Center], table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center - 1], table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center - 1][y_Center - 1], table[x_Center][y_Center - 1]);
            break;
        case 22:
            swap(table[x_Center + 1][y_Center - 1], table[x_Center + 2][y_Center - 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 2][y_Center]);
            swap(table[x_Center][y_Center], table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center + 1], table[x_Center + 1][y_Center + 1]);
            break;
    }
    x_Center++;
}
//----------------------------------------------------
void Z_Shape_Down()
{
    switch(currentShapeId){
        case 31:
            swap(table[x_Center + 1][y_Center], table[x_Center + 2][y_Center]);
            swap(table[x_Center][y_Center], table[x_Center + 1][y_Center ]);
            swap(table[x_Center][y_Center + 1], table[x_Center + 1][y_Center + 1]);
            swap(table[x_Center - 1][y_Center + 1], table[x_Center][y_Center + 1]);
            break;
        case 32:
            swap(table[x_Center + 1][y_Center + 1], table[x_Center + 2][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 2][y_Center]);
            swap(table[x_Center][y_Center], table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center - 1], table[x_Center + 1][y_Center - 1]);
            break;
    }
    x_Center++;
}
//----------------------------------------------------
void L_Shape_Down()
{
    switch (currentShapeId) {
        case 41:
            swap(table[x_Center + 1][y_Center + 1],table[x_Center + 2][y_Center + 1]);
            swap(table[x_Center + 1][y_Center],table[x_Center + 2][y_Center]);
            swap(table[x_Center][y_Center],table[x_Center + 1][y_Center]);
            swap(table[x_Center - 1][y_Center],table[x_Center][y_Center]);
            break;
        case 42:
            swap(table[x_Center + 1][y_Center], table[x_Center + 2][y_Center]);
            swap(table[x_Center][y_Center],table[x_Center + 1][y_Center]);
            swap(table[x_Center - 1][y_Center],table[x_Center][y_Center]);
            swap(table[x_Center - 1][y_Center - 1], table[x_Center][y_Center - 1]);
            break;
        case 43:
            swap(table[x_Center][y_Center - 1],table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center][y_Center], table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center + 1], table[x_Center + 1][y_Center + 1]);
            swap(table[x_Center - 1][y_Center + 1], table[x_Center][y_Center + 1]);
            break;
        case 44:
            swap(table[x_Center + 1][y_Center - 1],table[x_Center + 2][y_Center - 1]);
            swap(table[x_Center][y_Center - 1],table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center][y_Center],table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center + 1],table[x_Center + 1][y_Center + 1]);
            break;
    }
    x_Center++;
}
//----------------------------------------------------
void J_Shape_Down()
{
    switch (currentShapeId) {
        case 51:
            swap(table[x_Center + 1][y_Center - 1],table[x_Center + 2][y_Center - 1]);
            swap(table[x_Center + 1][y_Center],table[x_Center + 2][y_Center]);
            swap(table[x_Center][y_Center],table[x_Center + 1][y_Center]);
            swap(table[x_Center - 1][y_Center],table[x_Center][y_Center]);
            break;
        case 52:
            swap(table[x_Center + 1][y_Center], table[x_Center + 2][y_Center]);
            swap(table[x_Center][y_Center],table[x_Center + 1][y_Center]);
            swap(table[x_Center - 1][y_Center],table[x_Center][y_Center]);
            swap(table[x_Center - 1][y_Center + 1], table[x_Center][y_Center + 1]);
            break;
        case 53:
            swap(table[x_Center][y_Center - 1],table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center][y_Center], table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center + 1], table[x_Center + 1][y_Center + 1]);
            swap(table[x_Center - 1][y_Center - 1], table[x_Center][y_Center - 1]);
            break;
        case 54:
            swap(table[x_Center + 1][y_Center + 1],table[x_Center + 2][y_Center + 1]);
            swap(table[x_Center][y_Center - 1],table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center][y_Center],table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center + 1],table[x_Center + 1][y_Center + 1]);
            break;
    }
    x_Center++;
}
//----------------------------------------------------
void O_Shape_Down()
{
    swap(table[x_Center + 1][y_Center],table[x_Center + 2][y_Center]);
    swap(table[x_Center + 1][y_Center - 1],table[x_Center + 2][y_Center - 1]);
    swap(table[x_Center][y_Center],table[x_Center + 1][y_Center]);
    swap(table[x_Center][y_Center - 1],table[x_Center + 1][y_Center - 1]);
    x_Center++;
}
//----------------------------------------------------
void I_Shape_Down()
{
    switch (currentShapeId) {
        case 71:
            swap(table[x_Center][y_Center - 2], table[x_Center + 1][y_Center - 2]);
            swap(table[x_Center][y_Center - 1], table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center][y_Center], table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center + 1], table[x_Center + 1][y_Center + 1]);
            break;
        case 72:
            swap(table[x_Center + 2][y_Center], table[x_Center + 3][y_Center]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 2][y_Center]);
            swap(table[x_Center][y_Center], table[x_Center + 1][y_Center]);
            swap(table[x_Center - 1][y_Center], table[x_Center][y_Center]);
            break;
    }
    x_Center++;
}
//----------------------------------------------------
void E_Shape_Down()
{
    switch (currentShapeId) {
        case 81:
            swap(table[x_Center][y_Center - 1],table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center][y_Center],table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center + 1],table[x_Center + 1][y_Center + 1]);
            swap(table[x_Center - 1][y_Center],table[x_Center][y_Center]);
            break;
        case 82:
            swap(table[x_Center + 1][y_Center],table[x_Center + 2][y_Center]);
            swap(table[x_Center][y_Center - 1],table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center][y_Center],table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center + 1],table[x_Center + 1][y_Center + 1]);
            break;
        case 83:
            swap(table[x_Center + 1][y_Center],table[x_Center + 2][y_Center]);
            swap(table[x_Center][y_Center],table[x_Center + 1][y_Center]);
            swap(table[x_Center - 1][y_Center],table[x_Center][y_Center]);
            swap(table[x_Center][y_Center - 1],table[x_Center + 1][y_Center - 1]);
            break;
        case 84:
            swap(table[x_Center + 1][y_Center],table[x_Center + 2][y_Center]);
            swap(table[x_Center][y_Center],table[x_Center + 1][y_Center]);
            swap(table[x_Center - 1][y_Center],table[x_Center][y_Center]);
            swap(table[x_Center][y_Center + 1],table[x_Center + 1][y_Center + 1]);
            break;
    }
    x_Center++;
}
//----------------------------------------------------
void S_Shape_Horizontal_Left()
{
    switch(currentShapeId){
        case 21:
            swap(table[x_Center - 1][y_Center - 1], table[x_Center - 1][y_Center - 2]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center - 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center - 1]);
            break;
        case 22:
            swap(table[x_Center + 1][y_Center - 1], table[x_Center + 1][y_Center - 2]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center]);
            break;
    }
    y_Center--;
}
//----------------------------------------------------
void Z_Shape_Horizontal_Left()
{
    switch(currentShapeId){
        case 31:
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center]);
            swap(table[x_Center - 1][y_Center + 1], table[x_Center - 1][y_Center]);
            break;
        case 32:
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center - 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center + 1][y_Center + 1], table[x_Center + 1][y_Center]);
            break;
    }
    y_Center--;
}
//----------------------------------------------------
void L_Shape_Horizontal_Left()
{
    switch (currentShapeId) {
        case 41:
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center - 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center + 1][y_Center + 1], table[x_Center + 1][y_Center]);
            break;
        case 42:
            swap(table[x_Center - 1][y_Center - 1], table[x_Center - 1][y_Center - 2]);
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center - 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center - 1]);
            break;
        case 43:
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center - 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center]);
            swap(table[x_Center - 1][y_Center + 1], table[x_Center - 1][y_Center]);
            break;
        case 44:
            swap(table[x_Center + 1][y_Center - 1], table[x_Center + 1][y_Center - 2]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center - 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center]);
            break;
    }
    y_Center--;
}
//----------------------------------------------------
void J_Shape_Horizontal_Left()
{
    switch (currentShapeId) {
        case 51:
            swap(table[x_Center + 1][y_Center - 1], table[x_Center + 1][y_Center - 2]);
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center - 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center - 1]);
            break;
        case 52:
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center - 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center - 1][y_Center + 1], table[x_Center - 1][y_Center]);
            break;
        case 53:
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center - 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center]);
            swap(table[x_Center - 1][y_Center - 1], table[x_Center - 1][y_Center - 2]);
            break;
        case 54:
            swap(table[x_Center + 1][y_Center + 1], table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center - 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center]);
            break;
    }
    y_Center--;
}
//----------------------------------------------------
void O_Shape_Horizontal_Left()
{
    swap(table[x_Center][y_Center - 1], table[x_Center][y_Center - 2]);
    swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
    swap(table[x_Center + 1][y_Center - 1], table[x_Center + 1][y_Center - 2]);
    swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center - 1]);
    y_Center--;
}
//----------------------------------------------------
void I_Shape_Horizontal_Left()
{
    switch (currentShapeId) {
        case 71:
            swap(table[x_Center][y_Center - 2], table[x_Center][y_Center - 3]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center - 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center]);
            break;
        case 72:
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center - 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center + 2][y_Center], table[x_Center + 2][y_Center - 1]);
            break;
    }
    y_Center--;
}
//----------------------------------------------------
void E_Shape_Horizontal_Left()
{
    switch (currentShapeId) {
        case 81:
            swap(table[x_Center - 1][y_Center] , table[x_Center - 1][y_Center - 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center - 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center]);
            break;
        case 82:
            swap(table[x_Center + 1][y_Center] , table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center - 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center]);
            break;
        case 83:
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center - 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center - 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center - 1]);
            break;
        case 84:
            swap(table[x_Center][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center - 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center - 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center]);
            break;

    }
    y_Center--;
}
//----------------------------------------------------
void S_Shape_Horizontal_Right()
{
    switch(currentShapeId){
        case 21:
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center + 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center]);
            swap(table[x_Center - 1][y_Center - 1], table[x_Center - 1][y_Center]);
            break;
        case 22:
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center + 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center + 1]);
            swap(table[x_Center + 1][y_Center - 1], table[x_Center + 1][y_Center]);
            break;
    }
    y_Center++;
}
//----------------------------------------------------
void Z_Shape_Horizontal_Right()
{
    switch(currentShapeId){
        case 31:
            swap(table[x_Center - 1][y_Center + 1], table[x_Center - 1][y_Center + 2]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center + 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center + 1]);
            break;
        case 32:
            swap(table[x_Center + 1][y_Center + 1], table[x_Center + 1][y_Center + 2]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center + 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center]);
            break;
    }
    y_Center++;
}
//----------------------------------------------------
void L_Shape_Horizontal_Right()
{
    switch (currentShapeId) {
        case 41:
            swap(table[x_Center + 1][y_Center + 1], table[x_Center + 1][y_Center + 2]);
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center + 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center + 1]);
            break;
        case 42:
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center + 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center + 1]);
            swap(table[x_Center - 1][y_Center - 1], table[x_Center - 1][y_Center]);
            break;
        case 43:
            swap(table[x_Center - 1][y_Center + 1], table[x_Center - 1][y_Center + 2]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center + 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center]);
            break;
        case 44:
            swap(table[x_Center + 1][y_Center - 1], table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center + 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center]);
            break;
    }
    y_Center++;
}
//----------------------------------------------------
void J_Shape_Horizontal_Right()
{
    switch (currentShapeId) {
        case 51:
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center + 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center + 1]);
            swap(table[x_Center + 1][y_Center - 1], table[x_Center + 1][y_Center]);
            break;
        case 52:
            swap(table[x_Center - 1][y_Center + 1], table[x_Center - 1][y_Center + 2]);
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center + 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center + 1]);
            break;
        case 53:
            swap(table[x_Center - 1][y_Center - 1], table[x_Center - 1][y_Center]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center + 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center]);
            break;
        case 54:
            swap(table[x_Center + 1][y_Center + 1], table[x_Center + 1][y_Center + 2]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center + 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center]);
            break;
    }
    y_Center++;
}
//----------------------------------------------------
void O_Shape_Horizontal_Right()
{
    swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
    swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center + 1]);
    swap(table[x_Center][y_Center - 1], table[x_Center][y_Center]);
    swap(table[x_Center + 1][y_Center - 1], table[x_Center + 1][y_Center]);
    y_Center++;
}
//----------------------------------------------------
void I_Shape_Horizontal_Right()
{
    switch (currentShapeId) {
        case 71:
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center + 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center]);
            swap(table[x_Center][y_Center - 2], table[x_Center][y_Center - 1]);
            break;
        case 72:
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center + 1]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center + 1]);
            swap(table[x_Center + 2][y_Center], table[x_Center + 2][y_Center + 1]);
            break;
    }
    y_Center++;
}
//----------------------------------------------------
void E_Shape_Horizontal_Right()
{
    switch (currentShapeId) {
        case 81:
            swap(table[x_Center - 1][y_Center] , table[x_Center - 1][y_Center + 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center + 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center]);
            break;
        case 82:
            swap(table[x_Center + 1][y_Center] , table[x_Center + 1][y_Center + 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center + 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center]);
            break;
        case 83:
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center + 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center][y_Center]);
            break;
        case 84:
            swap(table[x_Center][y_Center + 1], table[x_Center][y_Center + 2]);
            swap(table[x_Center][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center - 1][y_Center], table[x_Center - 1][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center + 1][y_Center + 1]);
            break;
    }
    y_Center++;
}
//----------------------------------------------------
void S_Shape_Rotate()
{
    switch (currentShapeId) {
        case 21:
            swap(table[x_Center + 1][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center + 1][y_Center]);
            swap(table[x_Center - 1][y_Center - 1], table[x_Center + 1][y_Center - 1]);
            currentShapeId = 22;
            break;
        case 22:
            swap(table[x_Center + 1][y_Center - 1], table[x_Center - 1][y_Center - 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center + 1][y_Center]);
            currentShapeId = 21;
            break;
    }
}
//----------------------------------------------------
void Z_Shape_Rotate()
{
    switch (currentShapeId) {
        case 31:
            swap(table[x_Center + 1][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center][y_Center + 1], table[x_Center + 1][y_Center]);
            swap(table[x_Center - 1][y_Center + 1], table[x_Center + 1][y_Center + 1]);
            currentShapeId = 32;
            break;
        case 32:
            swap(table[x_Center + 1][y_Center + 1], table[x_Center - 1][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center][y_Center - 1], table[x_Center + 1][y_Center]);
            currentShapeId = 31;
            break;
    }
}
//----------------------------------------------------
void L_Shape_Rotate()
{
    switch (currentShapeId) {
        case 41:
            swap(table[x_Center - 1][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center + 1][y_Center + 1], table[x_Center + 1][y_Center - 1]);
            currentShapeId = 44;
            break;
        case 42:
            swap(table[x_Center + 1][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center - 1][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center - 1][y_Center - 1], table[x_Center - 1][y_Center + 1]);
            currentShapeId = 43;
            break;
        case 43:
            swap(table[x_Center][y_Center - 1], table[x_Center - 1][y_Center]);
            swap(table[x_Center][y_Center + 1], table[x_Center + 1][y_Center]);
            swap(table[x_Center - 1][y_Center + 1], table[x_Center + 1][y_Center + 1]);
            currentShapeId = 41;
            break;
        case 44:
            swap(table[x_Center][y_Center + 1], table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center - 1], table[x_Center - 1][y_Center]);
            swap(table[x_Center + 1][y_Center - 1], table[x_Center - 1][y_Center - 1]);
            currentShapeId = 42;
            break;
    }
}
//----------------------------------------------------
void J_Shape_Rotate()
{
    switch(currentShapeId){
        case 51:
            swap(table[x_Center - 1][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center + 1][y_Center - 1], table[x_Center - 1][y_Center - 1]);
            currentShapeId = 53;
            break;
        case 52:
            swap(table[x_Center + 1][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center - 1][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center - 1][y_Center + 1], table[x_Center + 1][y_Center + 1]);
            currentShapeId = 54;
            break;
        case 53:
            swap(table[x_Center][y_Center + 1], table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center - 1], table[x_Center - 1][y_Center]);
            swap(table[x_Center - 1][y_Center - 1], table[x_Center - 1][y_Center + 1]);
            currentShapeId = 52;
            break;
        case 54:
            swap(table[x_Center][y_Center - 1], table[x_Center - 1][y_Center]);
            swap(table[x_Center][y_Center + 1], table[x_Center + 1][y_Center]);
            swap(table[x_Center + 1][y_Center + 1], table[x_Center + 1][y_Center - 1]);
            currentShapeId = 51;
            break;
    }
}
//----------------------------------------------------
void I_Shape_Rotate()
{
    switch (currentShapeId) {
        case 71:
            swap(table[x_Center][y_Center + 1], table[x_Center - 1][y_Center]);
            swap(table[x_Center][y_Center - 1], table[x_Center + 1][y_Center]);
            swap(table[x_Center][y_Center - 2], table[x_Center + 2][y_Center]);
            currentShapeId = 72;
            break;
        case 72:
            swap(table[x_Center - 1][y_Center], table[x_Center][y_Center + 1]);
            swap(table[x_Center + 1][y_Center], table[x_Center][y_Center - 1]);
            swap(table[x_Center + 2][y_Center], table[x_Center][y_Center - 2]);
            currentShapeId = 71;
            break;
    }
}
//----------------------------------------------------
void E_Shape_Rotate()
{
    switch (currentShapeId) {
        case 81:
            swap(table[x_Center][y_Center - 1], table[x_Center + 1][y_Center]);
            currentShapeId = 84;
            break;
        case 82:
            swap(table[x_Center][y_Center + 1], table[x_Center - 1][y_Center]);
            currentShapeId = 83;
            break;
        case 83:
            swap(table[x_Center + 1][y_Center], table[x_Center][y_Center + 1]);
            currentShapeId = 81;
            break;
        case 84:
            swap(table[x_Center - 1][y_Center], table[x_Center][y_Center - 1]);
            currentShapeId = 82;
            break;
    }
}
//----------------------------------------------------
void getArrowKey()
{
    if (_kbhit()) {
        switch (getch()) {
            case KEY_UP://this key rotates shape
                if (checkIfShapeRotationIsValid()) {
                    rotateShape();
                    blockHasMoved = true;//set the flag true so the display can refresh
                }
                break;
            case KEY_DOWN://moves shape downward
                if (checkIfMovementIsValid('D')) {
                    shapeFall();
                    blockHasMoved = true;//set the flag true so the display can refresh
                }
                break;
            case KEY_LEFT://moves shape left
                if (checkIfMovementIsValid('L')) {
                    moveShapeLeft();
                    blockHasMoved = true;//set the flag true so the display can refresh
                }
                break;
            case KEY_RIGHT://moves shape right
                if (checkIfMovementIsValid('R')) {
                    moveShapeRight();
                    blockHasMoved = true;//set the flag true so the display can refresh
                }
                break;
            case ESC:
                displayPauseMenu();
            default:
                break;
        }
    }
}
//----------------------------------------------------
bool checkIfMovementIsValid(char direction)
{
    //this function checks if the shape can move through the provided direction
    switch (currentShapeId/10) {
        case 2:
            return S_Shape_Check_If_Movement_Is_Valid(direction);
            break;
        case 3:
            return Z_Shape_Check_If_Movement_Is_Valid(direction);
            break;
        case 4:
            return L_Shape_Check_If_Movement_Is_Valid(direction);
            break;
        case 5:
            return J_Shape_Check_If_Movement_Is_Valid(direction);
            break;
        case 6:
            return O_Shape_Check_If_Movement_Is_Valid(direction);
            break;
        case 7:
            return I_Shape_Check_If_Movement_Is_Valid(direction);
            break;
        case 8:
            return E_Shape_Check_If_Movement_Is_Valid(direction);
            break;

    }
}
//----------------------------------------------------
bool S_Shape_Check_If_Movement_Is_Valid(char direction)
{
    switch (currentShapeId) {
        case 21:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center - 1 , y_Center - 2) && checkIfBlockIsEmpty(x_Center, y_Center - 2)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center - 1 , y_Center) && checkIfBlockIsEmpty(x_Center, y_Center + 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 2 , y_Center) && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                        return true;
                    break;
            }
            break;
        case 22:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center , y_Center - 1) && checkIfBlockIsEmpty(x_Center + 1, y_Center - 2))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center + 1 , y_Center + 1) && checkIfBlockIsEmpty(x_Center, y_Center + 2))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 2 , y_Center) && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1)
                        && checkIfBlockIsEmpty(x_Center + 2 , y_Center - 1))
                        return true;
                    break;
                default:
                    break;
            }
            break;
    }
    return false;
}
//----------------------------------------------------
bool Z_Shape_Check_If_Movement_Is_Valid(char direction)
{
    switch (currentShapeId) {
        case 31:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center - 1 , y_Center) && checkIfBlockIsEmpty(x_Center, y_Center - 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center - 1 , y_Center + 2) && checkIfBlockIsEmpty(x_Center, y_Center + 2)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 2 , y_Center) && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
            }
            break;
        case 32:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center , y_Center - 2) && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center + 1 , y_Center + 2) && checkIfBlockIsEmpty(x_Center, y_Center + 1))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 2 , y_Center) && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1)
                        && checkIfBlockIsEmpty(x_Center + 2 , y_Center + 1))
                        return true;
                    break;
            }
            break;
    }
    return false;
}
//----------------------------------------------------
bool L_Shape_Check_If_Movement_Is_Valid(char direction)
{
    switch (currentShapeId) {
        case 41:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center - 1) && checkIfBlockIsEmpty(x_Center, y_Center - 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center + 1) && checkIfBlockIsEmpty(x_Center, y_Center + 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center + 2))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 2, y_Center) && checkIfBlockIsEmpty(x_Center + 2, y_Center + 1))
                        return true;
                    break;
            }
            break;
        case 42:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center - 2) && checkIfBlockIsEmpty(x_Center, y_Center - 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center + 1) && checkIfBlockIsEmpty(x_Center, y_Center + 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 2, y_Center) && checkIfBlockIsEmpty(x_Center, y_Center - 1))
                        return true;
                    break;
            }
            break;
        case 43:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center, y_Center - 2) && checkIfBlockIsEmpty(x_Center - 1, y_Center))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center, y_Center + 2) && checkIfBlockIsEmpty(x_Center - 1, y_Center + 2))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 1, y_Center - 1) && checkIfBlockIsEmpty(x_Center + 1, y_Center) &&
                        checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
            }
            break;
        case 44:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center, y_Center - 2) && checkIfBlockIsEmpty(x_Center + 1, y_Center - 2))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center, y_Center + 2) && checkIfBlockIsEmpty(x_Center + 1, y_Center))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 2, y_Center - 1) && checkIfBlockIsEmpty(x_Center + 1, y_Center) &&
                        checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;

            }
            break;

    }
    return false;
}
//----------------------------------------------------
bool J_Shape_Check_If_Movement_Is_Valid(char direction)
{
    switch (currentShapeId) {
        case 51:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center - 1) && checkIfBlockIsEmpty(x_Center, y_Center - 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center - 2))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center + 1) && checkIfBlockIsEmpty(x_Center, y_Center + 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 2, y_Center) && checkIfBlockIsEmpty(x_Center + 2, y_Center - 1))
                        return true;
                    break;
            }
            break;
        case 52:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center - 1) && checkIfBlockIsEmpty(x_Center, y_Center - 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center + 2) && checkIfBlockIsEmpty(x_Center, y_Center + 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 2, y_Center) && checkIfBlockIsEmpty(x_Center, y_Center + 1))
                        return true;
                    break;
            }
            break;
        case 53:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center, y_Center - 2) && checkIfBlockIsEmpty(x_Center - 1, y_Center - 2))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center, y_Center + 2) && checkIfBlockIsEmpty(x_Center - 1, y_Center))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 1, y_Center - 1) && checkIfBlockIsEmpty(x_Center + 1, y_Center) &&
                        checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
            }
            break;
        case 54:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center, y_Center - 2) && checkIfBlockIsEmpty(x_Center + 1, y_Center))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center, y_Center + 2) && checkIfBlockIsEmpty(x_Center + 1, y_Center + 2))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 2, y_Center + 1) && checkIfBlockIsEmpty(x_Center + 1, y_Center) &&
                        checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                        return true;
                    break;
            }
            break;
    }
    return false;
}
//----------------------------------------------------
bool O_Shape_Check_If_Movement_Is_Valid(char direction)
{
    switch (direction) {
        case 'L':
            if (checkIfBlockIsEmpty(x_Center, y_Center - 2) && checkIfBlockIsEmpty(x_Center + 1, y_Center - 2))
                return true;
            break;
        case 'R':
            if (checkIfBlockIsEmpty(x_Center, y_Center + 1) && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                return true;
            break;
        case 'D':
            if (checkIfBlockIsEmpty(x_Center + 2, y_Center) && checkIfBlockIsEmpty(x_Center + 2, y_Center - 1))
                return true;
            break;
    }
    return false;
}
//----------------------------------------------------
bool I_Shape_Check_If_Movement_Is_Valid(char direction)
{
    switch (currentShapeId) {
        case 71:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center, y_Center - 3))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center, y_Center + 2))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 1, y_Center - 2 ) && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center) && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
            }
            break;
        case 72:
            switch (direction) {
                case 'L':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center - 1 ) && checkIfBlockIsEmpty(x_Center, y_Center - 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1) && checkIfBlockIsEmpty(x_Center + 2, y_Center - 1))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center + 1 ) && checkIfBlockIsEmpty(x_Center, y_Center + 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1) && checkIfBlockIsEmpty(x_Center + 2, y_Center + 1))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 3, y_Center))
                        return true;
                    break;
            }
            break;
    }
    return false;
}
//----------------------------------------------------
bool E_Shape_Check_If_Movement_Is_Valid(char direction)
{
    switch (currentShapeId) {
        case 81:
            switch (direction) {
                case'L':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center - 1) && checkIfBlockIsEmpty(x_Center, y_Center - 2))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center + 1) && checkIfBlockIsEmpty(x_Center, y_Center + 2))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 1, y_Center - 1) && checkIfBlockIsEmpty(x_Center + 1, y_Center)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
            }
            break;
        case 82:
            switch (direction) {
                case'L':
                    if (checkIfBlockIsEmpty(x_Center + 1, y_Center - 1) && checkIfBlockIsEmpty(x_Center, y_Center - 2))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center + 1, y_Center + 1) && checkIfBlockIsEmpty(x_Center, y_Center + 2))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 1, y_Center - 1) && checkIfBlockIsEmpty(x_Center + 2, y_Center)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
            }
            break;
        case 83:
            switch (direction) {
                case'L':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center - 1) && checkIfBlockIsEmpty(x_Center, y_Center - 2)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center + 1) && checkIfBlockIsEmpty(x_Center, y_Center + 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 2, y_Center) && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                        return true;
                    break;
            }
            break;
        case 84:
            switch (direction) {
                case'L':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center - 1) && checkIfBlockIsEmpty(x_Center, y_Center - 1)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                        return true;
                    break;
                case 'R':
                    if (checkIfBlockIsEmpty(x_Center - 1, y_Center + 1) && checkIfBlockIsEmpty(x_Center, y_Center + 2)
                        && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
                case 'D':
                    if (checkIfBlockIsEmpty(x_Center + 2, y_Center) && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                        return true;
                    break;
            }
    }
    return false;
}
//----------------------------------------------------
bool S_Shape_Check_If_Rotation_Is_Valid()
{
    switch (currentShapeId) {
        case 21:
            if (checkIfBlockIsEmpty(x_Center, y_Center + 1) && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                return true;
            if (checkIfBlockIsEmpty(x_Center + 1, y_Center - 1) && checkIfBlockIsEmpty(x_Center + 1, y_Center - 2) &&
                S_Shape_Check_If_Movement_Is_Valid('L')){
                S_Shape_Horizontal_Left();
                return true;
            }
            break;
        case 22:
            if (checkIfBlockIsEmpty(x_Center, y_Center - 1) && checkIfBlockIsEmpty(x_Center - 1, y_Center - 1))
                return true;
            if (x_Center - 1 < 0 && checkIfBlockIsEmpty(x_Center + 2, y_Center && checkIfBlockIsEmpty(x_Center + 2, y_Center - 1) )){
                S_Shape_Down();
                return true;
            }
            break;
    }
    return false;
}
//----------------------------------------------------
bool Z_Shape_Check_If_Rotation_Is_Valid()
{
    switch (currentShapeId) {
        case 31:
            if (checkIfBlockIsEmpty(x_Center, y_Center - 1) && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                return true;
            if (checkIfBlockIsEmpty(x_Center + 1, y_Center + 1) && checkIfBlockIsEmpty(x_Center + 1, y_Center + 2) &&
                Z_Shape_Check_If_Movement_Is_Valid('R')){
                Z_Shape_Horizontal_Right();
                return true;
            }
            break;
        case 32:
            if (checkIfBlockIsEmpty(x_Center, y_Center + 1) && checkIfBlockIsEmpty(x_Center - 1, y_Center + 1))
                return true;
            if (x_Center - 1 < 0 && checkIfBlockIsEmpty(x_Center + 2, y_Center && checkIfBlockIsEmpty(x_Center + 2, y_Center + 1))){
                Z_Shape_Down();
                return true;
            }
            break;
    }
    return false;
}
//----------------------------------------------------
bool L_Shape_Check_If_Rotation_Is_Valid()
{
    switch (currentShapeId) {
        case 41:
            if (checkIfBlockIsEmpty(x_Center, y_Center + 1) && checkIfBlockIsEmpty(x_Center, y_Center - 1)
                && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                return true;
            if (checkIfBlockIsEmpty(x_Center, y_Center + 2) && checkIfBlockIsEmpty(x_Center, y_Center + 1) &&
                L_Shape_Check_If_Movement_Is_Valid('R')){
                L_Shape_Horizontal_Right();
                return true;
            }
            break;
        case 42:
            if (checkIfBlockIsEmpty(x_Center, y_Center - 1) && checkIfBlockIsEmpty(x_Center, y_Center + 1)
                && checkIfBlockIsEmpty(x_Center - 1, y_Center + 1))
                return true;
            if (checkIfBlockIsEmpty(x_Center, y_Center - 2) && checkIfBlockIsEmpty(x_Center, y_Center - 1) &&
                L_Shape_Check_If_Movement_Is_Valid('L')){
                L_Shape_Horizontal_Left();
                return true;
            }
            break;
        case 43:
            if (checkIfBlockIsEmpty(x_Center - 1, y_Center) && checkIfBlockIsEmpty(x_Center + 1, y_Center)
                && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                return true;
            break;
        case 44:
            if (checkIfBlockIsEmpty(x_Center + 1, y_Center) && checkIfBlockIsEmpty(x_Center - 1, y_Center)
                && checkIfBlockIsEmpty(x_Center - 1, y_Center - 1))
                return true;
            if (x_Center - 1 < 0 && checkIfBlockIsEmpty(x_Center + 2, y_Center + 1) && checkIfBlockIsEmpty(x_Center + 2, y_Center) &&
                checkIfBlockIsEmpty(x_Center + 1, y_Center)){
                L_Shape_Down();
                return true;
            }
            break;
    }
    return false;
}
//----------------------------------------------------
bool J_Shape_Check_If_Rotation_Is_Valid()
{
    switch (currentShapeId) {
        case 51:
            if (checkIfBlockIsEmpty(x_Center, y_Center + 1) && checkIfBlockIsEmpty(x_Center, y_Center - 1)
                && checkIfBlockIsEmpty(x_Center - 1, y_Center - 1))
                return true;
            if (checkIfBlockIsEmpty(x_Center, y_Center - 2) && checkIfBlockIsEmpty(x_Center, y_Center - 1) &&
                checkIfBlockIsEmpty(x_Center - 1, y_Center - 2) && J_Shape_Check_If_Movement_Is_Valid('L')){
                J_Shape_Horizontal_Left();
                return true;
            }
            break;
        case 52:
            if (checkIfBlockIsEmpty(x_Center, y_Center - 1) && checkIfBlockIsEmpty(x_Center, y_Center + 1)
                && checkIfBlockIsEmpty(x_Center + 1, y_Center + 1))
                return true;
            if (checkIfBlockIsEmpty(x_Center, y_Center + 2) && checkIfBlockIsEmpty(x_Center + 1, y_Center + 2) &&
                checkIfBlockIsEmpty(x_Center, y_Center + 1) && J_Shape_Check_If_Movement_Is_Valid('R')){
                J_Shape_Horizontal_Right();
                return true;
            }
            break;
        case 53:
            if (checkIfBlockIsEmpty(x_Center + 1, y_Center) && checkIfBlockIsEmpty(x_Center - 1, y_Center)
                && checkIfBlockIsEmpty(x_Center - 1, y_Center + 1))
                return true;
            break;
        case 54:
            if (checkIfBlockIsEmpty(x_Center - 1, y_Center) && checkIfBlockIsEmpty(x_Center + 1, y_Center)
                && checkIfBlockIsEmpty(x_Center + 1, y_Center - 1))
                return true;
            if (x_Center - 1 < 0 && checkIfBlockIsEmpty(x_Center + 2, y_Center - 1) && checkIfBlockIsEmpty(x_Center + 2, y_Center) &&
                checkIfBlockIsEmpty(x_Center + 1, y_Center)){
                J_Shape_Down();
                return true;
            }
            break;
    }
    return false;
}
//----------------------------------------------------
bool I_Shape_Check_If_Rotation_Is_Valid()
{
    switch (currentShapeId) {
        case 71:
            if (checkIfBlockIsEmpty(x_Center - 1, y_Center) && checkIfBlockIsEmpty(x_Center + 1, y_Center) &&
                checkIfBlockIsEmpty(x_Center + 2, y_Center))
                return true;
            if (x_Center - 1 < 0 && checkIfBlockIsEmpty(x_Center + 1, y_Center) && checkIfBlockIsEmpty(x_Center + 2, y_Center)
                && checkIfBlockIsEmpty(x_Center + 3 , y_Center)) {
                I_Shape_Down();
                return true;
            }
            break;
        case 72:
            if (checkIfBlockIsEmpty(x_Center, y_Center + 1) && checkIfBlockIsEmpty(x_Center, y_Center - 1) &&
                checkIfBlockIsEmpty(x_Center, y_Center - 2))
                return true;
            if (I_Shape_Check_If_Movement_Is_Valid('L') && checkIfBlockIsEmpty(x_Center, y_Center - 2) && checkIfBlockIsEmpty(x_Center, y_Center - 3)){
                I_Shape_Horizontal_Left();
                return true;
            }
            if (I_Shape_Check_If_Movement_Is_Valid('R')){
                I_Shape_Horizontal_Right();
                if (I_Shape_Check_If_Movement_Is_Valid('R') && checkIfBlockIsEmpty(x_Center, y_Center + 1)){
                    I_Shape_Horizontal_Right();
                    return true;
                }
                I_Shape_Horizontal_Left();
            }
            break;
    }
    return false;
}
//----------------------------------------------------
bool E_Shape_Check_If_Rotation_Is_Valid()
{
    switch (currentShapeId) {
        case 81:
            if (checkIfBlockIsEmpty(x_Center + 1, y_Center)){
                return true;
            }
            break;
        case 82:
            if (checkIfBlockIsEmpty(x_Center - 1, y_Center)){
                return true;
            }
            if (E_Shape_Check_If_Movement_Is_Valid('D')){
                E_Shape_Down();
                return true;
            }
            break;
        case 83:
            if (checkIfBlockIsEmpty(x_Center, y_Center + 1)){
                return true;
            }
            if (E_Shape_Check_If_Movement_Is_Valid('L')){
                E_Shape_Horizontal_Left();
                return true;
            }
            break;
        case 84:
            if (checkIfBlockIsEmpty(x_Center, y_Center - 1)){
                return true;
            }
            if (E_Shape_Check_If_Movement_Is_Valid('R')){
                E_Shape_Horizontal_Right();
                return true;
            }
            break;
    }
    return false;
}
//----------------------------------------------------
bool checkIfShapeRotationIsValid()
{
    switch (currentShapeId/10) {
        case 2:
            return S_Shape_Check_If_Rotation_Is_Valid();
            break;
        case 3:
            return Z_Shape_Check_If_Rotation_Is_Valid();
            break;
        case 4:
            return L_Shape_Check_If_Rotation_Is_Valid();
            break;
        case 5:
            return J_Shape_Check_If_Rotation_Is_Valid();
            break;
        case 6:
            return false;
            break;
        case 7:
            return I_Shape_Check_If_Rotation_Is_Valid();
            break;
        case 8:
            return E_Shape_Check_If_Rotation_Is_Valid();
            break;
    }
}
//----------------------------------------------------
bool checkIfBlockIsEmpty(int x, int y)
{
    if (x < 0 || y < 0)
        return false;
    return table[x][y].id == 0;
}
//----------------------------------------------------
void makeDelayForFreezeShape(int milliseconds) {
    chrono::milliseconds duration = chrono::milliseconds(milliseconds);

    //making an object from thread class and including duration as its variable, duration is being captured as a copy
    thread delay_thread([duration]() {
        std::this_thread::sleep_for(duration);
        if (!checkIfMovementIsValid('D'))
            freezeShape();
        else
            delayForFreezeShapeIsOn = true;
    });
    // Detach the thread to run independently
    delay_thread.detach();
}
//----------------------------------------------------
void makeDelayForShapeFall(int milliseconds)
{
    chrono::milliseconds duration = chrono::milliseconds(milliseconds);
    //making an object from thread class and including duration as its variable, duration is being captured as a copy
    thread delay_thread([duration]() {
        std::this_thread::sleep_for(duration);
        shapeFall();
        shapeIsGoingDown = false;
    });
    // Detach the thread to run independently
    delay_thread.detach();
}
//----------------------------------------------------
void showConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
//----------------------------------------------------
void freezeShape()
{
    if (!checkIfMovementIsValid('D')) {
        switch (currentShapeId / 10) {
            case 2:
                switch (currentShapeId) {
                    case 21:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center][y_Center - 1].freeze = true;
                        table[x_Center - 1][y_Center - 1].freeze = true;
                        table[x_Center + 1][y_Center].freeze = true;
                        break;
                    case 22:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center][y_Center + 1].freeze = true;
                        table[x_Center + 1][y_Center].freeze = true;
                        table[x_Center + 1][y_Center - 1].freeze = true;
                        break;
                }
                break;
            case 3:
                switch (currentShapeId) {
                    case 31:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center][y_Center + 1].freeze = true;
                        table[x_Center + 1][y_Center].freeze = true;
                        table[x_Center - 1][y_Center + 1].freeze = true;
                        break;
                    case 32:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center][y_Center - 1].freeze = true;
                        table[x_Center + 1][y_Center].freeze = true;
                        table[x_Center + 1][y_Center + 1].freeze = true;
                        break;
                }
                break;
            case 4:
                switch (currentShapeId) {
                    case 41:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center - 1][y_Center].freeze = true;
                        table[x_Center + 1][y_Center].freeze = true;
                        table[x_Center + 1][y_Center + 1].freeze = true;
                        break;
                    case 42:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center - 1][y_Center].freeze = true;
                        table[x_Center - 1][y_Center - 1].freeze = true;
                        table[x_Center + 1][y_Center].freeze = true;
                        break;
                    case 43:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center][y_Center - 1].freeze = true;
                        table[x_Center][y_Center + 1].freeze = true;
                        table[x_Center - 1][y_Center + 1].freeze = true;
                        break;
                    case 44:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center][y_Center + 1].freeze = true;
                        table[x_Center][y_Center - 1].freeze = true;
                        table[x_Center + 1][y_Center - 1].freeze = true;
                        break;
                }
                break;
            case 5:
                switch (currentShapeId) {
                    case 51:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center - 1][y_Center].freeze = true;
                        table[x_Center + 1][y_Center].freeze = true;
                        table[x_Center + 1][y_Center - 1].freeze = true;
                        break;
                    case 52:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center + 1][y_Center].freeze = true;
                        table[x_Center - 1][y_Center].freeze = true;
                        table[x_Center - 1][y_Center + 1].freeze = true;
                        break;
                    case 53:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center][y_Center + 1].freeze = true;
                        table[x_Center][y_Center - 1].freeze = true;
                        table[x_Center - 1][y_Center - 1].freeze = true;
                        break;
                    case 54:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center][y_Center - 1].freeze = true;
                        table[x_Center][y_Center + 1].freeze = true;
                        table[x_Center + 1][y_Center + 1].freeze = true;
                        break;
                }
                break;
            case 6: {
                table[x_Center][y_Center].freeze = true;
                table[x_Center][y_Center - 1].freeze = true;
                table[x_Center + 1][y_Center - 1].freeze = true;
                table[x_Center + 1][y_Center].freeze = true;
            }
                break;
            case 7:
                switch (currentShapeId) {
                    case 71:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center][y_Center + 1].freeze = true;
                        table[x_Center][y_Center - 1].freeze = true;
                        table[x_Center][y_Center - 2].freeze = true;
                        break;
                    case 72:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center - 1][y_Center].freeze = true;
                        table[x_Center + 1][y_Center].freeze = true;
                        table[x_Center + 2][y_Center].freeze = true;
                        break;
                }
                break;
            case 8:
                switch (currentShapeId) {
                    case 81:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center][y_Center - 1].freeze = true;
                        table[x_Center][y_Center + 1].freeze = true;
                        table[x_Center - 1][y_Center].freeze = true;
                        break;
                    case 82:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center][y_Center - 1].freeze = true;
                        table[x_Center][y_Center + 1].freeze = true;
                        table[x_Center + 1][y_Center].freeze = true;
                        break;
                    case 83:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center][y_Center - 1].freeze = true;
                        table[x_Center - 1][y_Center].freeze = true;
                        table[x_Center + 1][y_Center].freeze = true;
                        break;
                    case 84:
                        table[x_Center][y_Center].freeze = true;
                        table[x_Center][y_Center + 1].freeze = true;
                        table[x_Center - 1][y_Center].freeze = true;
                        table[x_Center + 1][y_Center].freeze = true;
                        break;
                }
                break;
        }
        shapeIsFreezed = true;
    }
    else{
        delayForFreezeShapeIsOn = true;
    }
}
//----------------------------------------------------
void calculateScore(int fullRows)
{
    Score += ((column * fullRows)/(row/20.0)) * 10;
}
//----------------------------------------------------
void searchForFullRows()
{
    //this function iterates on members of the table vector and inspect whether rows are empty or not
    int countBlock = 0;//for conting full block, if this integer is equal to column, the row is full
    int countFullRows = 0;//for scoring
    int lastEmptyRow = findLastEmptyRow();
    //here we search for a full row
    for (int i = row - 1; i > lastEmptyRow ; i--) {
        for (int j = 1; j < column + 1; j++) {
            if (table[i][j].id == 0) {
                countBlock = 0;
                break;
            }
            else
                countBlock++;
        }
        if (countBlock == column){
            emptyRow(i);//setting all blocks id in a row to 0
            countFullRows++;
            moveRowsDown(lastEmptyRow, i);
            countBlock = 0;
        }
    }
    calculateScore(countFullRows);
}
//----------------------------------------------------
void emptyRow(int Row)
{
    //this section deletes the full row
    for (int i = 1; i < column + 1; ++i) {
        table[Row][i].id = 0;
        table[Row][i].freeze = false;
    }

}
//----------------------------------------------------
int findLastEmptyRow()
{
    int Row = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 1; j < column + 1; j++) {
            if (table[i][j].id != 0)
                break;
            if (j == column)
                Row = i;
        }
    }
    return Row;
}
//----------------------------------------------------
void moveRowsDown(int &lastEmptyRow, int &emptyRow)
{
    for (int i = emptyRow; i > lastEmptyRow; i--){
        swap(table[i], table[i - 1]);
    }
    lastEmptyRow++;
    emptyRow++;
}
//----------------------------------------------------
void ignoreBufferInputs()
{
    _flushall();//flushing buffer
}
//----------------------------------------------------
void gameIsOver()
{
    endOfGame = steady_clock::now();
    system("cls");
    string gameOver = "Game Over";
    for (int i = 0 ; i < gameOver.length() ; i++){
        if (gameOver[i] == '_')
            setConsoleColor(WHITE);
        else if (gameOver[i] == '/' )
            setConsoleColor(RED);
        else if (gameOver[i] == '\\') {
            if (gameOver[i - 1] == '_')
                setConsoleColor(RED);
            else
                setConsoleColor(LIGHTRED);
        }
        cout << gameOver[i];
    }
    Sleep(3000);

}
//----------------------------------------------------
void unFreezeShape()
{
    switch (currentShapeId / 10) {
        case 2:
            switch (currentShapeId) {
                case 21:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center][y_Center - 1].freeze = false;
                    table[x_Center - 1][y_Center - 1].freeze = false;
                    table[x_Center + 1][y_Center].freeze = false;
                    break;
                case 22:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center][y_Center + 1].freeze = false;
                    table[x_Center + 1][y_Center].freeze = false;
                    table[x_Center + 1][y_Center - 1].freeze = false;
                    break;
            }
            break;
        case 3:
            switch (currentShapeId) {
                case 31:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center][y_Center + 1].freeze = false;
                    table[x_Center + 1][y_Center].freeze = false;
                    table[x_Center - 1][y_Center + 1].freeze = false;
                    break;
                case 32:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center][y_Center - 1].freeze = false;
                    table[x_Center + 1][y_Center].freeze = false;
                    table[x_Center + 1][y_Center + 1].freeze = false;
                    break;

            }
            break;
        case 4:
            switch (currentShapeId) {
                case 41:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center - 1][y_Center].freeze = false;
                    table[x_Center + 1][y_Center].freeze = false;
                    table[x_Center + 1][y_Center + 1].freeze = false;
                    break;
                case 42:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center - 1][y_Center].freeze = false;
                    table[x_Center - 1][y_Center - 1].freeze = false;
                    table[x_Center + 1][y_Center].freeze = false;
                    break;
                case 43:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center][y_Center - 1].freeze = false;
                    table[x_Center][y_Center + 1].freeze = false;
                    table[x_Center - 1][y_Center + 1].freeze = false;
                    break;
                case 44:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center][y_Center + 1].freeze = false;
                    table[x_Center][y_Center - 1].freeze = false;
                    table[x_Center + 1][y_Center - 1].freeze = false;
                    break;
            }
            break;
        case 5:
            switch (currentShapeId) {
                case 51:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center - 1][y_Center].freeze = false;
                    table[x_Center + 1][y_Center].freeze = false;
                    table[x_Center + 1][y_Center - 1].freeze = false;
                    break;
                case 52:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center + 1][y_Center].freeze = false;
                    table[x_Center - 1][y_Center].freeze = false;
                    table[x_Center - 1][y_Center + 1].freeze = false;
                    break;
                case 53:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center][y_Center + 1].freeze = false;
                    table[x_Center][y_Center - 1].freeze = false;
                    table[x_Center - 1][y_Center - 1].freeze = false;
                    break;
                case 54:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center][y_Center - 1].freeze = false;
                    table[x_Center][y_Center + 1].freeze = false;
                    table[x_Center + 1][y_Center + 1].freeze = false;
                    break;
            }
            break;
        case 6: {
            table[x_Center][y_Center].freeze = false;
            table[x_Center][y_Center - 1].freeze = false;
            table[x_Center + 1][y_Center - 1].freeze = false;
            table[x_Center + 1][y_Center].freeze = false;
        }
            break;
        case 7:
            switch (currentShapeId) {
                case 71:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center][y_Center + 1].freeze = false;
                    table[x_Center][y_Center - 1].freeze = false;
                    table[x_Center][y_Center - 2].freeze = false;
                    break;
                case 72:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center - 1][y_Center].freeze = false;
                    table[x_Center + 1][y_Center].freeze = false;
                    table[x_Center + 2][y_Center].freeze = false;
                    break;
            }
            break;
        case 8:
            switch (currentShapeId) {
                case 81:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center][y_Center - 1].freeze = false;
                    table[x_Center][y_Center + 1].freeze = false;
                    table[x_Center - 1][y_Center].freeze = false;
                    break;
                case 82:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center][y_Center - 1].freeze = false;
                    table[x_Center][y_Center + 1].freeze = false;
                    table[x_Center + 1][y_Center].freeze = false;
                    break;
                case 83:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center][y_Center - 1].freeze = false;
                    table[x_Center - 1][y_Center].freeze = false;
                    table[x_Center + 1][y_Center].freeze = false;
                    break;
                case 84:
                    table[x_Center][y_Center].freeze = false;
                    table[x_Center][y_Center + 1].freeze = false;
                    table[x_Center - 1][y_Center].freeze = false;
                    table[x_Center + 1][y_Center].freeze = false;
                    break;
            }
            break;
    }
    shapeIsFreezed = false;
}
//----------------------------------------------------
int calculateTime(time_point<steady_clock> end, time_point<steady_clock> start)
{
    //calculate time from start to end
    duration<double> elapsed_seconds = end - start;
    return int(elapsed_seconds.count()) + durationTime; //for counting seconds and durationTime for loaded games
}
//----------------------------------------------------
void distributeIds()
{
    //we call it once to start generating ids
    currentShapeId = generateRandomNumber(2, 8);
    //here we set the rotation of the current shape
    if (currentShapeId == 2 || currentShapeId == 3 || currentShapeId == 7){
        currentShapeId = (currentShapeId * 10) + generateRandomNumber(1, 2);
    }
    else if (currentShapeId == 4 || currentShapeId == 5 || currentShapeId == 8){
        currentShapeId = (currentShapeId * 10) + generateRandomNumber(1, 4);
    }
    else{
        currentShapeId = 61;
    }
    nextShapeId = generateRandomNumber(2, 8);
    while(checkIds()){
        nextShapeId = generateRandomNumber(2, 8);
    }
    //here we set the rotation of the next shape
    if (nextShapeId == 2 || nextShapeId == 3 || nextShapeId == 7){
        nextShapeId = (nextShapeId * 10) + generateRandomNumber(1, 2);
    }
    else if (nextShapeId == 4 || nextShapeId == 5 || nextShapeId == 8){
        nextShapeId = (nextShapeId * 10) + generateRandomNumber(1, 4);
    }
    else{
        nextShapeId = 61;
    }
}
//----------------------------------------------------
bool checkIds()
{
    //here we check that shape are not the same with four shapes before
    return (nextShapeId == currentShapeId / 10 || nextShapeId == previousShapeId / 10 ||
            nextShapeId == secondPreviousShapeId / 10 || nextShapeId == thirdPreviousShapeId / 10);
}
//----------------------------------------------------
void updateIds()
{
    //here we pass ids to before ones
    thirdPreviousShapeId = secondPreviousShapeId;
    secondPreviousShapeId = previousShapeId;
    previousShapeId = currentShapeId;
    currentShapeId = nextShapeId;
    //after we passed next shape id to the current shape id, we generate the next one
    nextShapeId = generateRandomNumber(2, 8);
    while(checkIds()){
        nextShapeId = generateRandomNumber(2, 8);
    }
    //here we set the rotation of the next shape
    if (nextShapeId == 2 || nextShapeId == 3 || nextShapeId == 7){
        nextShapeId = (nextShapeId * 10) + generateRandomNumber(1, 2);
    }
    else if (nextShapeId == 4 || nextShapeId == 5 || nextShapeId == 8){
        nextShapeId = (nextShapeId * 10) + generateRandomNumber(1, 4);
    }
    else{
        nextShapeId = 61;
    }
}
//----------------------------------------------------
void startGame()
{
    //here we start a new game
    startOfGame = steady_clock::now();
    showConsoleCursor(false);
    initializeTable();
    distributeIds();
    spawnShape();
    generateTable();
    showNextShape();
    displayScore();
    blockHasMoved = false;
}
//----------------------------------------------------
void startLoadedGame()
{
    //here we start a loaded game from a file
    startOfGame = steady_clock::now();
    showConsoleCursor(false);
    spawnShape();
    generateTable();
    showNextShape();
    displayScore();
    blockHasMoved = false;
}
//----------------------------------------------------
void gameCore()
{
    //here we run game
    while (true) {
        if (!checkIfMovementIsValid('D')){
            if (delayForFreezeShapeIsOn){
                delayForFreezeShapeIsOn = false;
                makeDelayForFreezeShape(shapeFreezeTime);
            }
        }
        if(!shapeIsGoingDown && !shapeIsFreezed){
            makeDelayForShapeFall(shapeFallTime);
            shapeIsGoingDown = true;
        }
        if (shapeIsFreezed){
            if (!checkIfMovementIsValid('D')) {
                searchForFullRows();
                updateIds();
                spawnShape();
                ignoreBufferInputs();
                delayForFreezeShapeIsOn = true;
                shapeIsFreezed = false;
                generateTable();
                showNextShape();
                displayScore();
            }
            else{
                unFreezeShape();
            }
        }
        getArrowKey();
        if (blockHasMoved) {
            generateTable();
            showNextShape();
            displayScore();
            blockHasMoved = false;
        }

    }
}
//----------------------------------------------------
void displayMainMenu()
{
    showConsoleCursor(false);
    system("cls");
    //messages
    {
        setConsoleColor(RED);
        cout << "\n~~~~~~~~~~~~~~~~~ ";
        setConsoleColor(LIGHTYELLOW);
        cout << "Main menu";
        setConsoleColor(RED);
        cout << " ~~~~~~~~~~~~~~~~~\n";
        setConsoleColor(RED);
        cout << "1 - ";
        setConsoleColor(LIGHTYELLOW);
        cout << "New game";
        setConsoleColor(RED);
        cout << "\n2 - ";
        setConsoleColor(LIGHTYELLOW);
        cout << "Load game";
        setConsoleColor(RED);
        cout << "\n3 - ";
        setConsoleColor(LIGHTYELLOW);
        cout << "Leader board";
        setConsoleColor(RED);
        cout << "\n4 - ";
        setConsoleColor(LIGHTYELLOW);
        cout << "Options & settings";
        setConsoleColor(RED);
        cout << "\n5 - ";
        setConsoleColor(LIGHTYELLOW);
        cout << "Exit\n";
    }
    switch (getch()) {
        case 49:
            displayHowToPlay();
            newGame();
            resetGame();
            initializeTable();
            startGame();
            gameCore();
            break;
        case 50:
            resetGame();
            loadGameFromFile();
            startLoadedGame();
            gameCore();
            break;
        case 51:
            displayLeaderBoard();
            displayMainMenu();
            break;
        case 52:
            displayOptionAndSettings();
            displayMainMenu();
            break;
        case 53:
            Exit();
            break;
        default:
            setConsoleColor(CYAN);
            cout << "Invalid input!";
            Sleep(1000);
            displayMainMenu();
            break;
    }
}
//----------------------------------------------------
void displayHowToPlay()
{
    system("cls");
    setConsoleColor(LIGHTYELLOW);
    cout << "~~~~~~~~~~~~~ Welcome to tetris game! ~~~~~~~~~~~~~\n";
    setConsoleColor(CYAN );
    cout << "How to play: ";
    setConsoleColor(LIGHTPURPLE);
    cout << "Use" << " arrow up " << "to rotate shapes 90 degree to right , " << "arrow down" << " to move shapes downward\n" << "arrow left" << " to left and " << "arrow right" << " to right.\n";
    setConsoleColor(CYAN);
    cout << "Losing condition: ";
    setConsoleColor(LIGHTPURPLE);
    cout << "When there is not enough space to spawn next shape, YOU LOSE!\n";
    setConsoleColor(GREEN);
    system("pause");
}
//----------------------------------------------------
void displayPauseMenu()
{
    time_point<steady_clock> startOfTimeInPauseMenu , endOfTimeInPauseMenu;
    startOfTimeInPauseMenu = steady_clock::now();
    system("cls");
    setConsoleColor(RED);
    cout << "~~~~~~~~~~~~~ ";
    setConsoleColor(LIGHTYELLOW);
    cout << "Pause menu";
    setConsoleColor(RED);
    cout << " ~~~~~~~~~~~~~\n";
    setConsoleColor(RED);
    cout << "1 - ";
    setConsoleColor(LIGHTYELLOW);
    cout << "Continue";
    setConsoleColor(RED);
    cout << "\n2 - ";
    setConsoleColor(LIGHTYELLOW);
    cout << "Save game";
    setConsoleColor(RED);
    cout << "\n3 - ";
    setConsoleColor(LIGHTYELLOW);
    cout << "Change block skin";
    setConsoleColor(RED);
    cout << "\n4 - ";
    setConsoleColor(LIGHTYELLOW);
    cout << "Exit";
    switch (getch()) {
        case 49:
            endOfTimeInPauseMenu = steady_clock::now();
            startOfGame = startOfGame + (endOfTimeInPauseMenu - startOfTimeInPauseMenu);
            gameCore();
            break;
        case 50:
            endOfTimeInPauseMenu = steady_clock::now();
            startOfGame = startOfGame + (endOfTimeInPauseMenu - startOfTimeInPauseMenu);
            saveGameToFile();
            displayPauseMenu();
            break;
        case 51:
            endOfTimeInPauseMenu = steady_clock::now();
            startOfGame = startOfGame + (endOfTimeInPauseMenu - startOfTimeInPauseMenu);
            changeBlocksSkin();
            displayPauseMenu();
            break;
        case 52:
            endOfTimeInPauseMenu = steady_clock::now();
            startOfGame = startOfGame + (endOfTimeInPauseMenu - startOfTimeInPauseMenu);
            endOfGame = steady_clock::now();
            getPlayersInfo();
            displayMainMenu();
            break;
        default:
            setConsoleColor(CYAN);
            cout << "\nInvalid input!";
            Sleep(1000);
            endOfTimeInPauseMenu = steady_clock::now();
            startOfGame = startOfGame + (endOfTimeInPauseMenu - startOfTimeInPauseMenu);
            displayPauseMenu();
            break;
    }

}
//----------------------------------------------------
void displayLeaderBoard()
{
    system("cls");
    showConsoleCursor(false);
    setConsoleColor(LIGHTGREEN);
    cout << "Select games leaderboard difficulty";
    setConsoleColor(RED);
    cout << "\n[1] ";
    setConsoleColor(CYAN);
    cout << "Easy";
    setConsoleColor(RED);
    cout << "\n[2] ";
    setConsoleColor(CYAN);
    cout << "Normal";
    setConsoleColor(RED);
    cout << "\n[3] ";
    setConsoleColor(CYAN);
    cout << "Hard";
    switch (getch()) {
        case 49:
            loadScoreDataFromFile("easy_leaderboard.txt");
            break;
        case 50:
            loadScoreDataFromFile("normal_leaderboard.txt");
            break;
        case 51:
            loadScoreDataFromFile("hard_leaderboard.txt");
            break;
        default:
            loadScoreDataFromFile("normal_leaderboard.txt");
            break;
    }
    //sorting leaderboard vector
    sort(leaderboard.begin(), leaderboard.end(), compare);
    findAndReplacePlayer();
    Sleep(1000);
    system("cls");
    setConsoleColor(LIGHTCYAN);
    cout << "Rank" << "\t";
    cout << "Name" << "\t\t";
    cout << "Score" << "\t\t";
    cout << "Time\n";
    //outputting data from leaderboard vector
    for (int i  = 0 ; i < leaderboard.size() ; i++) {
        setConsoleColor(RED);
        cout << i + 1 << "\t";
        setConsoleColor(LIGHTPURPLE);
        cout << leaderboard[i].name << "\t\t";
        setConsoleColor(LIGHTGREEN);
        cout << leaderboard[i].score << "\t\t";
        setConsoleColor(LIGHTYELLOW);
        cout << outputTime(leaderboard[i].time) << "\n";
    }
    //in this line we reset the leaderboard vector
    leaderboard.clear();
    system("pause");
}
//----------------------------------------------------
void Exit()
{
    system("cls");
    {
        setConsoleColor(CYAN);
        cout << "Do you really want to exit?\n";
        setConsoleColor(RED);
        cout << "1 - ";
        setConsoleColor(GREEN);
        cout << "Yes\n";
        setConsoleColor(RED);
        cout << "2 - ";
        setConsoleColor(GREEN);
        cout << "No\n";
    }
    switch (getch())
    {
        case 49:
            system("cls");
            setConsoleColor(WHITE);
            exit(1);
        case 50:
            displayMainMenu();
            break;
        default:
            setConsoleColor(RED);
            cout << "Invalid input!";
            Sleep(1000);
            Exit();
            break;
    }
}
//----------------------------------------------------
void showNextShape()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 0});
    setConsoleColor(LIGHTPURPLE);
    cout << "Next Shape";
    switch (nextShapeId / 10) {
        case 2:
            setBlockColor(2);
            switch (nextShapeId % 10) {
                case 1:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 1});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 2});
                    cout << block << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 6), 3});
                    cout << block;
                    break;
                case 2:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 6), 1});
                    cout << block << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 2});
                    cout << block << block;
                    break;
            }
            break;
        case 3:
            setBlockColor(3);
            switch (nextShapeId % 10) {
                case 1:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 6), 1});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 2});
                    cout << block << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 3});
                    cout << block;
                    break;
                case 2:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 1});
                    cout << block << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 6), 2});
                    cout << block << block;
                    break;

            }
            break;
        case 4:
            setBlockColor(4);
            switch (nextShapeId % 10) {
                case 1:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 1});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 2});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 3});
                    cout << block << block;
                    break;
                case 2:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 1});
                    cout << block << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 6), 2});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 6), 3});
                    cout << block;
                    break;
                case 3:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 7), 1});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 2});
                    cout << block << block << block;
                    break;
                case 4:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 1});
                    cout << block << block << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 2});
                    cout << block;
                    break;

            }
            break;
        case 5:
            setBlockColor(5);
            switch (nextShapeId % 10) {
                case 1:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 6), 1});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 6), 2});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 3});
                    cout << block << block;
                    break;
                case 2:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 1});
                    cout << block << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 2});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 3});
                    cout << block;
                    break;
                case 3:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 1});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 2});
                    cout << block << block << block;
                    break;
                case 4:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 1});
                    cout << block << block << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 7), 2});
                    cout << block;
                    break;

            }
            break;
        case 6:
            setBlockColor(6);
            SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 1});
            cout << block << block;
            SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 2});
            cout << block << block;
            break;
        case 7:
            setBlockColor(7);
            switch (nextShapeId % 10) {
                case 1:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 1});
                    cout << block << block << block << block;
                    break;
                case 2:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 1});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 2});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 3});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 4});
                    cout << block;
                    break;

            }
            break;
        case 8:
            setBlockColor(8);
            switch (nextShapeId % 10) {
                case 1:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 6), 1});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 2});
                    cout << block << block << block;
                    break;
                case 2:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 1});
                    cout << block << block << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 6), 2});
                    cout << block;
                    break;
                case 3:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 6), 1});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 2});
                    cout << block << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 6), 3});
                    cout << block;
                    break;
                case 4:
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 1});
                    cout << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 2});
                    cout << block << block;
                    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 3});
                    cout << block;
                    break;
            }
            break;

    }

}
//----------------------------------------------------
void displayScore()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, {static_cast<SHORT>(column + 5), 5});
    setConsoleColor(LIGHTGREEN);
    cout << "Score: ";
    setConsoleColor(RED);
    cout << Score;
}
//----------------------------------------------------
void newGame()
{
    showConsoleCursor(true);
    system("cls");
    setConsoleColor(LIGHTGREEN);
    cout << "Please enter your games table size.\nTable size have to be at least 5x5.\nNote that original game have 20 rows and 10 columns.";
    cout << "\nRow: ";
    setConsoleColor(RED);
    cin >> row;
    if (row < 5){
        setConsoleColor(RED);
        cout << "Please enter a number equal to 5 or greater than it!";
        Sleep(1000);
        newGame();
    }
    setConsoleColor(LIGHTGREEN);
    cout << "Column: ";
    setConsoleColor(RED);
    cin >> column;
    if (column < 5){
        setConsoleColor(RED);
        cout << "Please enter a number equal to 5 or greater than it!";
        Sleep(1000);
        newGame();
    }
    showConsoleCursor(false);
    setConsoleColor(LIGHTGREEN);
    cout << "Select games difficulty";
    setConsoleColor(RED);
    cout << "\n[1] ";
    setConsoleColor(CYAN);
    cout << "Easy";
    setConsoleColor(RED);
    cout << "\n[2] ";
    setConsoleColor(CYAN);
    cout << "Normal";
    setConsoleColor(RED);
    cout << "\n[3] ";
    setConsoleColor(CYAN);
    cout << "Hard";
    switch (getch()) {
        case 49:
            difficultyLevel = 1;
            shapeFallTime = 3000;
            shapeFreezeTime = 2000;
            break;
        case 50:
            difficultyLevel = 2;
            shapeFallTime = 2000;
            shapeFreezeTime = 1500;
            break;
        case 51:
            difficultyLevel = 3;
            shapeFallTime = 1000;
            shapeFreezeTime = 1000;
            break;
        default:
            difficultyLevel = 2;
            shapeFallTime = 2000;
            shapeFreezeTime = 1500;
            break;
    }
}
//----------------------------------------------------
void saveScoreDataToFile(const LeaderBoardMembers &data, const string &filename)
{
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << "\n" << data.name << " " << data.score << " " << data.time;
        file.close();
        setConsoleColor(LIGHTGREEN);
        cout << "\nPlayer data has been saved!";
    } else {
        cout << "\nUnable to save the player data!";
    }
    Sleep(1000);
}
//----------------------------------------------------
void loadScoreDataFromFile(const string& fileAddress)
{
    ifstream file(fileAddress);
    LeaderBoardMembers data;
    if (file.peek() == file.eof())
        return;
    if (file.is_open()) {
        while (!file.eof()) {
            file >> data.name >> data.score >> data.time;
            leaderboard.emplace_back(data);
        }
        file.close();
        setConsoleColor(LIGHTGREEN);
        cout << "\nLeaderboard data have been loaded from the file.";
    } else {
        setConsoleColor(RED);
        cout << "\nUnable to open the file for loading leaderboard data. \nPlease play games and then come and see leader board.";
        Sleep(1000);
        displayMainMenu();
    }

}
//----------------------------------------------------
void saveGameToFile()
{
    endOfGame = steady_clock::now();
    showConsoleCursor(true);
    string filename;
    system("cls");
    setConsoleColor(CYAN);
    cout << "Enter the name of the file you want save the game(Example: gameSave.txt): ";
    setConsoleColor(RED);
    cin >> filename;
    ofstream file(filename, ios::trunc | ios::out);
    if (file.is_open()) {
        file << thirdPreviousShapeId << " " << secondPreviousShapeId << " " << previousShapeId << " " << currentShapeId << " " << nextShapeId << "\n";
        file << Score << "\n";
        file << block << "\n";
        file << calculateTime(endOfGame, startOfGame) << "\n";
        file << difficultyLevel << " " << shapeFallTime << " " << shapeFreezeTime << "\n";
        file << row << " " << column << "\n";
        for (int i = 0; i < row + 1 ; i++){
            for (int j = 0; j < column + 2 ; j++){
                if (table[i][j].freeze || table[i][j].id == 0) {
                    file << table[i][j].id << " ";
                }
                else if (!table[i][j].freeze && table[i][j].id != 0){
                    file << 0 << " ";
                }
            }
            file << "\n";
        }
        file.close();
        setConsoleColor(LIGHTGREEN);
        cout << "\nYour game have been saved successfully.";
    } else {
        setConsoleColor(RED);
        cout << "\nUnable to save the player data!";
    }
    showConsoleCursor(false);
    Sleep(1000);
}
//----------------------------------------------------
void loadGameFromFile()
{
    showConsoleCursor(true);
    string filename;
    system("cls");
    setConsoleColor(CYAN);
    cout << "Enter the name of the file you want to load the game(Example: gameSave.txt): ";
    setConsoleColor(RED);
    cin >> filename;
    ifstream file(filename);
    if (file.is_open()){
        file >> thirdPreviousShapeId >> secondPreviousShapeId >> previousShapeId >> currentShapeId >> nextShapeId;
        file >> Score;
        file >> block;
        file >> durationTime;
        file >> difficultyLevel >> shapeFallTime >> shapeFreezeTime;
        file >> row >> column;
        initializeTable();
        for (int i = 0; i < row + 1 ; i++){
            for (int j = 0; j < column + 2 ; j++){
                file >> table[i][j].id;
            }
        }
        file.close();
        setConsoleColor(LIGHTGREEN);
        cout << "\nYour game have been loaded successfully.";
        Sleep(1000);
    }
    else {
        setConsoleColor(RED);
        cout << "\nUnable to load the player data!";
        Sleep(1000);
        displayMainMenu();
    }
    showConsoleCursor(false);
}
//----------------------------------------------------
int compare(const LeaderBoardMembers& one , const LeaderBoardMembers& two)
{
    if (one.score > two.score) {
        return 1;
    }
    else if (one.score == two.score){
        if (one.time <= two.time){
            return 1;
        }
    }
    return 0;
}
//----------------------------------------------------
void getPlayersInfo()
{
    string skip;
    system("cls");
    LeaderBoardMembers data;
    showConsoleCursor(true);
    setConsoleColor(CYAN);
    cout << "\nEnter your nickname to save your score(maximum of 6 characters) or type skip\nto skip saving data: ";
    setConsoleColor(LIGHTPURPLE);
    cin >> data.name;
    skip = data.name;
    transform(skip.begin(), skip.end(), skip.begin(), ::toupper);
    if (skip == "SKIP")
        return;
    if (data.name.size() > 6){
        setConsoleColor(RED);
        cout << "More characters that maximum limit are entered! \nPlease enter a maximum of 6 character";
        Sleep(1000);
        getPlayersInfo();
    }
    data.score = Score;
    data.time = calculateTime(endOfGame, startOfGame);
    switch (difficultyLevel) {
        case 1://easy
            saveScoreDataToFile(data, "easy_leaderboard.txt");
            break;
        case 2://normal
            saveScoreDataToFile(data, "normal_leaderboard.txt");
            break;
        case 3://hard
            saveScoreDataToFile(data, "hard_leaderboard.txt");
            break;
    }
}
//----------------------------------------------------
string outputTime(int time)
{
    string convertedTime;
    if (time / 3600 < 10)
        convertedTime = convertedTime + "0" + to_string(time / 3600) + ":";
    else
        convertedTime = convertedTime + to_string(time / 3600) + ":";
    if ((time % 3600) / 60 < 10)
        convertedTime = convertedTime + "0" + to_string((time % 3600) / 60) + ":";
    else
        convertedTime = convertedTime + to_string((time % 3600) / 60) + ":";
    if ((time % 3600) % 60 < 10)
        convertedTime = convertedTime + "0" + to_string((time % 3600) % 60);
    else
        convertedTime = convertedTime + to_string((time % 3600) % 60);
    return convertedTime;
}
//----------------------------------------------------
void findAndReplacePlayer()
{
    for (int i = 0 ; i < leaderboard.size() ; i++){
        for (int j = 0 ; j < leaderboard.size() ; j++){
            if (leaderboard[i].name == leaderboard[j].name){
                if (leaderboard[i].score < leaderboard[j].score){
                    leaderboard.erase(leaderboard.begin() + i);
                }
                else if (leaderboard[i].score == leaderboard[j].score){
                    if (leaderboard[i].time > leaderboard[j].time){
                        leaderboard.erase(leaderboard.begin() + i);
                    }
                }
            }
        }
    }
}
//----------------------------------------------------
void resetGame()
{
    Score = 0;
    thirdPreviousShapeId = 0;
    secondPreviousShapeId = 0;
    previousShapeId = 0;
    currentShapeId = 0;
    nextShapeId = 0;
    durationTime = 0;
    blockHasMoved = true;
    shapeIsFreezed = false;
    shapeIsGoingDown = false;
    delayForFreezeShapeIsOn = true;
}
//----------------------------------------------------
void displayOptionAndSettings()
{
    system("cls");
    setConsoleColor(RED);
    cout << "~~~~~~~~~~~~~ ";
    setConsoleColor(LIGHTYELLOW);
    cout << "Options & settings";
    setConsoleColor(RED);
    cout << " ~~~~~~~~~~~~~\n";
    setConsoleColor(RED);
    cout << "1 - ";
    setConsoleColor(LIGHTYELLOW);
    cout << "Blocks skin";
    setConsoleColor(RED);
    cout << "\n2 - ";
    setConsoleColor(LIGHTYELLOW);
    cout << "Credit";
    setConsoleColor(RED);
    cout << "\n3 - ";
    setConsoleColor(LIGHTYELLOW);
    cout << "Exit";
    switch (getch()) {
        case 49:
            changeBlocksSkin();
            displayOptionAndSettings();
            break;
        case 50:
            displayCredit();
            displayOptionAndSettings();
            break;
        case 51:
            displayMainMenu();
            break;
        default:
            setConsoleColor(CYAN);
            cout << "\nInvalid input!";
            Sleep(1000);
            displayOptionAndSettings();
            break;
    }
}
//----------------------------------------------------
void changeBlocksSkin()
{
    system("cls");
    setConsoleColor(LIGHTGREEN);
    cout << "Choose one of the following shape. Default shape is square\n";
    setConsoleColor(RED);
    cout << "1 - ";
    setConsoleColor(CYAN);
    cout << "@\n";
    setConsoleColor(RED);
    cout << "2 - ";
    setConsoleColor(CYAN);
    cout << "\u2B24\n";
    setConsoleColor(RED);
    cout << "3 - ";
    setConsoleColor(CYAN);
    cout << "\u25A0\n";
    setConsoleColor(RED);
    cout << "4 - ";
    setConsoleColor(CYAN);
    cout << "#\n";
    setConsoleColor(RED);
    cout << "5 - ";
    setConsoleColor(CYAN);
    cout << "*\n";
    setConsoleColor(RED);
    cout << "6 - ";
    setConsoleColor(CYAN);
    cout << "Exit\n";
    switch (getch()) {
        case 49:
            block = "@";
            setConsoleColor(LIGHTPURPLE);
            cout << "Block skin have been changed successfully!";
            Sleep(1000);
            break;
        case 50:
            block = "\u2B24";
            setConsoleColor(LIGHTPURPLE);
            cout << "Block skin have been changed successfully!";
            Sleep(1000);
            break;
        case 51:
            block = "\u25A0";
            setConsoleColor(LIGHTPURPLE);
            cout << "Block skin have been changed successfully!";
            Sleep(1000);
            break;
        case 52:
            block = "#";
            setConsoleColor(LIGHTPURPLE);
            cout << "Block skin have been changed successfully!";
            Sleep(1000);
            break;
        case 53:
            block = "*";
            setConsoleColor(LIGHTPURPLE);
            cout << "Block skin have been changed successfully!";
            Sleep(1000);
            break;
        case 54:
            break;
        default:
            setConsoleColor(CYAN);
            cout << "\nInvalid input!";
            Sleep(1000);
            changeBlocksSkin();
            break;
    }
}
//----------------------------------------------------
void displayCredit()
{
    system("cls");
    setConsoleColor(LIGHTYELLOW);
    cout << "Programmer: ";
    setConsoleColor(LIGHTGREEN);
    cout << "Seyed Mohammad mahdi Razavi\n";
    system("pause");
}
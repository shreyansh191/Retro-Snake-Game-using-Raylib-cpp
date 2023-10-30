#include "./headers/raylib.h"
#include "./headers/raymath.h"
#include<iostream>
#include<deque>
#include<fstream>
#include<string>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51 ,24, 255};


int cellSize = 45;
int cellCount = 16;
int offset = 75;

double lastUpdateTime = 0;


typedef struct{                 //Not using typedef will give error in 'getline(fin,hgscore.m1);' as: 'no matching arguments' since it takes datatype of Score and not string then!
    string m1= "0";
    string m2= "0";
} Score;                    

bool ElementinDeque (Vector2 element, deque<Vector2> deque) {
    for (unsigned int i=0; i< deque.size(); i++) {
        if(Vector2Equals(element, deque[i])) {
            return true;
        }
    }
    return false;
}

bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval){
        lastUpdateTime= currentTime;
        return true;
    }
    return false;
}

class Snake {
public:
    deque<Vector2> body = {Vector2{6,8}, Vector2{5,8}, Vector2{4,8}};
    Vector2 direction = {1,0};
    bool addSegment = false;

    void Draw() {
        for(unsigned int i=0; i<body.size(); i++){
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset+ x*cellSize, offset+ y*cellSize, (float)cellSize, (float)cellSize};
            if(i==0) DrawRectangleRounded(segment, 0.5, 6, BLUE);
            else DrawRectangleRounded(segment, 0.5, 6, BLACK);
        }
    }

    void Update() {
        body.push_front(Vector2Add(body[0],direction));
        if(addSegment == true){
            addSegment = false;
        } else {
            body.pop_back();
        }
    }

    void Reset() {
        body = {Vector2{6,8}, Vector2{5,8}, Vector2{4,8}};
        direction = {1,0};
    }

};


class Food {
public:
    Vector2 position;
    Texture2D texture;                                            //Uses redering gpu
    
    Food(deque<Vector2> snakeBody){                                                     //Constructor
        Image image = LoadImage("./images/mushroom.png");
        ImageResize(&image, cellSize,cellSize);
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPosition(snakeBody);
    }

    ~Food(){                                                    //Deconstructor
        UnloadTexture(texture);
    }

    void Draw(){
        DrawTexture(texture, offset+ (position.x) * cellSize , offset+ (position.y) * cellSize, WHITE);
    }


    Vector2 GenerateRandomCell() {
        float x = GetRandomValue(0,cellCount - 1);
        float y = GetRandomValue(0,cellCount - 1);
        return Vector2{x, y};
    }

    Vector2 GenerateRandomPosition(deque<Vector2> snakeBody) {
        Vector2 position = GenerateRandomCell();
        while(ElementinDeque(position, snakeBody)) position = GenerateRandomCell();
        return position;
    }                   
};


class Game {
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;
    Score hgscore;
    bool method= 1;

    Sound eatSound;
    Sound wallSound;
    ofstream fout;
    ifstream fin;

    Game(){
        InitAudioDevice();
        eatSound = LoadSound("./sounds/eat.mp3");
        wallSound = LoadSound("./sounds/wall.mp3");

        fin.open("./logs/rtsnake.txt");
        if (!fin) {
            cerr << "error: open file for input failed!" << endl;
        }
        if(!is_empty(fin)){
            getline(fin,hgscore.m1,'\n');
            getline(fin,hgscore.m2,'\n');
        }
        fin.close();
    }

    ~Game() {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    bool is_empty(std::ifstream& pFile){
        return pFile.peek() == std::ifstream::traits_type::eof();
    }

    void Draw() {
        snake.Draw();
        food.Draw();
    }

    void Update() {
        if(running){
            snake.Update();
            CheckCollisionWithEdges();
            CheckCollisionWithFood();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood() {
        if (Vector2Equals(snake.body[0], food.position)) {
            PlaySound(eatSound);
            food.position = food.GenerateRandomPosition(snake.body);
            snake.addSegment = true;
            score++;
            if( score> stoi((method) ? hgscore.m1 : hgscore.m2)){
                ((method) ? hgscore.m1 : hgscore.m2)= to_string(score);
                fout.open("./logs/rtsnake.txt", std::ofstream::out | std::ofstream::trunc);         
                if(!fout.is_open()) cerr << "error: open file for output failed!" <<endl;
                fout <<hgscore.m1 << endl << hgscore.m2;
                fout.close();
            } 
        }
    }

    void CheckCollisionWithEdges() {
        if(method){
        //Snake passes through the edges
        if(snake.body[0].x == cellCount) snake.body[0].x -= cellCount;
        if(snake.body[0].x == -1) snake.body[0].x += cellCount;
        
        if(snake.body[0].y == cellCount) snake.body[0].y -= cellCount;
        if(snake.body[0].y == -1) snake.body[0].y += cellCount;
        }

        else{
        //Snake cannot pass through the edges
        if(snake.body[0].x == cellCount || snake.body[0].x == -1){
            GameOver();
        }
        if(snake.body[0].y == cellCount || snake.body[0].y == -1){
            GameOver();
        }
        }

    }

    void GameOver() {
        PlaySound(wallSound);
        snake.Reset();
        food.position = food.GenerateRandomPosition(snake.body);
        running = false;
        score=0;
    }

    void CheckCollisionWithTail() {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if(ElementinDeque(snake.body[0], headlessBody)){
            GameOver();
        }
    }
};

int main(void){
    cout << "Starting the game ... " << endl;
    InitWindow(2*offset+cellCount*cellSize, 2*offset+cellCount*cellSize,"Retro Snake Game");
    SetTargetFPS(60);       //(60frames per second)--> without this, computer will run the game as fast as it can!
    Game game = Game();
    bool activateKeys =true;
    double eventTriggeringTime = 0.2;       //After every 0.2 seconds, there will be an update.

    while(WindowShouldClose() == false){
        
        if (eventTriggered(eventTriggeringTime)) {
            game.Update();
            if(game.method) activateKeys = true;
        }
        
        if(IsKeyPressed(KEY_M))
        { 
            game.method=!game.method;
            game.GameOver();
        }
        
        if(game.method){
        if(activateKeys){
            if(IsKeyPressed(KEY_UP) && game.snake.direction.y!=1)
            {
                game.snake.direction = {0, -1};
                game.running = true;
                activateKeys=false;
            } 
            else if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y!=-1)
            {
                game.snake.direction = {0, 1};
                game.running = true;  
                activateKeys=false;
            } 
            else if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x!=-1)
            {
                game.snake.direction = {1, 0};
                game.running = true;  
                activateKeys=false;
            } 
            else if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x!=1)
            {
                game.snake.direction = {-1, 0};
                game.running = true; 
                activateKeys=false;
            } 
        }

        }else{
        if(IsKeyPressed(KEY_UP) && game.snake.direction.y!=1)
        {
            game.snake.direction = {0, -1};
            game.running = true;
            game.Update();
            lastUpdateTime = GetTime();// This variable will only effect eventTriggered function!!
        } 
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y!=-1)
        {
            game.snake.direction = {0, 1};
            game.running = true;  
            game.Update();
            lastUpdateTime = GetTime();
        } 
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x!=-1)
        {
            game.snake.direction = {1, 0};
            game.running = true;  
            game.Update();
            lastUpdateTime = GetTime();
        } 
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x!=1)
        {
            game.snake.direction = {-1, 0};
            game.running = true;  
            game.Update();
            lastUpdateTime = GetTime();
        } 
        }

        //Drawing-
        BeginDrawing();//creates a blank canvas!(and the window that appears!)

        ClearBackground(RAYWHITE);
        DrawRectangleLinesEx(Rectangle{(float)offset-5 , (float)offset-5,(float)cellCount*cellSize+10, (float)cellCount*cellSize+10}, 5 , darkGreen);   // -5 and +10 so that the object and border still have a little gap of 5 pixels each side!
        DrawText("Retro Snake Game", offset-5, 20, 40, darkGreen);
        DrawText(TextFormat("%i", game.score) , offset-5, offset+cellCount*cellSize+10, 40, darkGreen);
        DrawText( ((game.method) ? game.hgscore.m1 : game.hgscore.m2).c_str() , offset+cellCount*cellSize+5, offset+cellCount*cellSize+10, 40, darkGreen);
        DrawText( ((game.method) ? "X" : "O") , offset+cellCount*cellSize+10, 20, 20, darkGreen);
        

        game.Draw();
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
#define RAYGUI_IMPLEMENTATION

#include "raylib.h"
#include "raygui.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include <vector>
#include <fstream>
#include <iostream>

typedef enum GameScreen
{
    TITLE = 0,
    GAME,
    EXIT
} GameScreen;

void initializeGame();
void initializeCamera(Camera * camera);
void handleInput(Player * player);
void handleCamera(Camera * camera, Player * player);
void moveEnemies();
void moveBullets();
void drawBullets();
void scoreCheck();

std::vector<Bullet> bullets;
std::vector<Enemy> enemies;

int lives = 3;
int score = 0;

int main()
{
    const int width = 1280;
    const int height = 720;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Space Havoc");
    // SetTargetFPS(60);

    GameScreen currentScreen = TITLE;
    bool isQuittingGame = false;

    Player player;
    Enemy enemy((Vector3) {0.0f, 0.0f, 10.0f});
    enemies.push_back(enemy);

    Enemy enemy2((Vector3) {0.0f, 0.0f, 20.0f});
    enemies.push_back(enemy2);

    Enemy enemy3((Vector3) {0.0f, 0.0f, 30.0f});
    enemies.push_back(enemy3);

    Camera camera;
    initializeCamera(&camera);

    while (!WindowShouldClose())
    {        
        // Update
        switch (currentScreen)
        {
            case TITLE:
            {
                DrawText("SPACE HAVOC", GetScreenWidth() / 2 - MeasureText("SPACE HAVOC", 40) / 2, GetScreenHeight() / 4, 40, LIGHTGRAY);

                float buttonX = GetScreenWidth() / 2.0 - 200.0 / 2.0;
                float playButtonY = GetScreenHeight() / 2.0 - 70.0 / 2.0;
                float quitButtonY = playButtonY + 70.0 + 20.0;

                GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

                if (GuiButton((Rectangle){buttonX, playButtonY, 200, 70}, "PLAY"))
                {
                    DisableCursor();
                    initializeGame();
                    currentScreen = GAME;
                }

                if (GuiButton((Rectangle){buttonX, quitButtonY, 200,70}, "QUIT"))
                {
                    isQuittingGame = true;
                    break;
                }
        } break;

            case GAME:
            {
                handleInput(&player);
                handleCamera(&camera, &player);
                moveEnemies();
                moveBullets();
            } break;
        }

        if (isQuittingGame)
        {
            break;
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10, 10);

        switch (currentScreen)
        {
            case TITLE:
            {
            } break;

            case GAME:
            {
                DrawText(TextFormat("Lives: %d", lives), 10, 35, 20, WHITE);
                DrawText(TextFormat("Score: %d", score), 10, 60, 20, WHITE);

                BeginMode3D(camera);

                DrawModel(player.getModel(), player.getPosition(), 1.0f, WHITE);
                DrawBoundingBox(player.getBoundingBox(), LIME);

                // Draw the enemies
                for (int i = 0; i < enemies.size(); i++)
                {
                    // Draw the current enemy
                    DrawModel(enemies[i].getModel(), enemies[i].getPosition(), enemies[i].getScale(), WHITE);
                    DrawBoundingBox(enemies[i].getBoundingBox(), LIME);

                    // Check if the player collides with an enemy
                    if (CheckCollisionBoxes(player.getBoundingBox(), enemies[i].getBoundingBox()))
                    {
                        enemies[i].destroy();
                        enemies.erase(enemies.begin() + i);
                        lives--;
                    }
                }

                DrawGrid(10, 10.0f);

                drawBullets();

                if(lives <= 0){
                    EnableCursor();
                    currentScreen = EXIT;
                }

                EndMode3D();
            } break;
            case EXIT:
            {
                scoreCheck();

                float buttonY = GetScreenHeight() / 2.0 + 300.0 / 2.0;
                float continueButtonX = GetScreenWidth() / 2.0 - 650.0 / 2.0;
                float quitButtonX = continueButtonX + 450.0 + 20.0;
                GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

                if (GuiButton((Rectangle){continueButtonX, buttonY, 200, 70}, "CONTINUE?"))
                {
                    currentScreen = TITLE;
                    score = 0;
                    lives = 2;
                }

                if (GuiButton((Rectangle){quitButtonX, buttonY, 200,70}, "QUIT"))
                {
                    isQuittingGame = true;
                    break;
                }
            }
        }

        EndDrawing();
    }

    player.destroy();
    // enemy.destroy();
    CloseWindow();
    return 0;
}

void initializeGame() {
    lives = 3;
    score = 0;
}

void initializeCamera(Camera * camera) {
    *camera = {0};
    camera->position = (Vector3){0.0f, 0.0f, 0.0f};
    camera->target = (Vector3){0.0f, 0.0f, 0.0f};
    camera->up = (Vector3){0.0f, 1.0f, 0.0f};
    camera->fovy = 30.0f;
    camera->projection = CAMERA_PERSPECTIVE;
}

void handleInput(Player * player) {
    if (IsKeyDown(KEY_LEFT))
    {
        player->moveLeft();
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        player->moveRight();
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        bullets.push_back(Bullet(player->getPosition()));
    }
}

void handleCamera(Camera * camera, Player * player) {
    // UpdateCamera(camera, CAMERA_THIRD_PERSON);

    // Make the camera follow the player when they move
    camera->target = (Vector3){player->getPosition().x + 1.7f, 0.0f, 0.0f};
    camera->position = (Vector3){(player->getPosition().x + 1.7f), 4.0f, player->getPosition().z - 20.0f};
}

void moveEnemies() {
    for(int i = 0; i < enemies.size(); i++) {
        enemies[i].move();

        // Check if the enemy goes behind the player
        if(enemies[i].getPosition().z <= -60.0f) {
            enemies.erase(enemies.begin() + i);
            score--;
            continue;
        }
    }
}

void moveBullets() {
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].move();

        // Check if the bullet goes beyond the grid
        if(bullets[i].getPosition().z >= 250.0) {
            bullets.erase(bullets.begin() + i);
            continue;
        }

        // Check for a enemy bullet collision
        for(int j = 0; j < enemies.size(); j++) {
            if(CheckCollisionBoxSphere(enemies[j].getBoundingBox(), bullets[i].getPosition(), bullets[i].getRadius())) {
                enemies[j].destroy();
                enemies.erase(enemies.begin() + j);
                bullets.erase(bullets.begin() + i);

                score++;
            }
        }
    }
}

void drawBullets() {
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].draw();
    }
}

void scoreCheck(){
    int a;
    std::fstream highestScore("highestScore.txt", std::ios_base::in);
    DrawText("GAME OVER", GetScreenWidth() / 2 - MeasureText("GAME OVER", 40) / 2, GetScreenHeight() / 4, 40, LIGHTGRAY);
    DrawText(TextFormat("SCORE: %d", score),  GetScreenWidth() / 2 - MeasureText("SCORE", 40) / 2.25, GetScreenHeight() / 2, 30, WHITE);

    while(highestScore >> a){
        std::cout << "I got here" << std::endl;
        if(a <= score)
        {
            std::cout << "I got here" << std::endl;
            DrawText(TextFormat("HIGH SCORE: %d", a),  GetScreenWidth() / 2 - MeasureText("HIGH SCORE", 40) / 2, GetScreenHeight() / 1.75, 30, WHITE);
        }

        //If the score is higher than the high score, change that value in the file
        if(a > score)
        {
            highestScore.close(); 
            std::ofstream highestScoreOut("highestScore.txt", std::ios_base::out);

            if (highestScoreOut.is_open()) {
                highestScoreOut << score;
                highestScoreOut.close();
            }

            DrawText(TextFormat("HIGH SCORE: %d", score),  GetScreenWidth() / 2 - MeasureText("HIGH SCORE", 40) / 2, GetScreenHeight() / 1.75, 30, WHITE);
        }

    }
}